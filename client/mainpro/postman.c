#include <windows.h>
#include <string.h>
#include "model.h"
#include "service.h"
#include "socket.h"

char msgBoxAppFileName[MAX_PATH];

int initPostman()
{

    char *p,*q=NULL;
    //msgBoxAppFileName[0]='\"';
    GetModuleFileNameA(NULL,msgBoxAppFileName,MAX_PATH);
    for(q=p=msgBoxAppFileName;*p;p++) if(*p=='\\')q=p;
    *q = 0;
    strcat(msgBoxAppFileName,"/msgr.exe");
    return 0;
}
DWORD WINAPI ShowMessageThread(LPVOID pMsgData)
{
    char callMsgApp[400];
    NetMessage *netMsg;
    netMsg=(NetMessage*)pMsgData;
    strcpy(callMsgApp,"\"");
    strcat(callMsgApp,msgBoxAppFileName);
    strcat(callMsgApp,"\"");
    if(netMsg->mode)
        strcat(callMsgApp," -m");
    if(netMsg->time>0)
    {
        char timeParam[8];
        sprintf(timeParam," -t %hd",netMsg->time);
        strcat(callMsgApp,timeParam);
    }
    if(netMsg->caption[0])
    {
        strcat(callMsgApp," -c \"");
        strcat(callMsgApp,netMsg->caption);
        strcat(callMsgApp,"\"");

    }
    strcat(callMsgApp, " \"");
    strcat(callMsgApp, (char*)pMsgData+sizeof(NetMessage));
    strcat(callMsgApp, "\"");

    //STARTUPINFO si = {sizeof(STARTUPINFO)};		// postman.c:45: warning: missing initializer
    STARTUPINFO si = { .cb = sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;
    if (!CreateProcessA(NULL, (char *)callMsgApp, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
    {
        return -1;
    }
    WaitForSingleObject(pi.hProcess,INFINITE);
    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess,&exitCode);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    char backString[64];
    switch(exitCode)
    {
    case 0:
        strcpy(backString,"��Ϣ���ڱ��ر�.");
        break;
    case 1:
        strcpy(backString,"��Ϣ����ȷ����ť�����.");
        break;
    case 2:
        strcpy(backString,"��Ϣ���ڵ�������ʱ�䣬�Զ����ر�.");
        break;
    default:
        strcpy(backString,"��Ϣ�������.");
    }
    sendToServer(ToServer_CloseMessageBox, (unsigned char *)backString);
    return 0;
}

void showNetMessage(const BYTE *pMsgData)
{
    DWORD dwThreadID;

    CreateThread(NULL, 0, (unsigned long int (__stdcall *)(void *))ShowMessageThread, (void *)pMsgData, 0, &dwThreadID);
}
