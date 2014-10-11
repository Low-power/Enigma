

#include <windows.h>
#include <stdio.h>
#define MAXBLOCKSIZE 1024
#define DOWNLOAD_REQ_SIZE    4096
#define INTERNET_OPEN_TYPE_DIRECT 1
#define INTERNET_INVALID_PORT_NUMBER 0
#define INTERNET_FLAG_NO_CACHE_WRITE 0x04000000
#define INTERNET_FLAG_DONT_CACHE INTERNET_FLAG_NO_CACHE_WRITE
typedef PVOID HINTERNET;
typedef HINTERNET (WINAPI *INTERNETOPENA)(LPCSTR,DWORD,LPCSTR,LPCSTR,DWORD);
typedef BOOL (WINAPI *INTERNETREADFILE)(HINTERNET,PVOID,DWORD,PDWORD);
typedef HINTERNET (WINAPI *INTERNETOPENURLA)(HINTERNET,LPCSTR,LPCSTR,DWORD,DWORD,DWORD);
typedef BOOL (WINAPI *INTERNETCLOSEHANDLE)(HINTERNET);
INTERNETOPENA InternetOpenA;
INTERNETREADFILE InternetReadFile;
INTERNETOPENURLA InternetOpenUrlA;
INTERNETCLOSEHANDLE InternetCloseHandle;
//int downLoad(char *szServer,char *szNetFileName,char *szLocalFileName);
int initINet()
{
    HMODULE hINet= LoadLibraryA("wininet.dll");
    if(!hINet)
    {
        return 1;
    }
    InternetOpenA = (INTERNETOPENA)GetProcAddress(hINet,"InternetOpenA");
    InternetReadFile=(INTERNETREADFILE)GetProcAddress(hINet,"InternetReadFile");
    InternetOpenUrlA=(INTERNETOPENURLA)GetProcAddress(hINet,"InternetOpenUrlA");
    InternetCloseHandle=(INTERNETCLOSEHANDLE)GetProcAddress(hINet,"InternetCloseHandle");
    if(!InternetOpenA || !InternetOpenUrlA || !InternetReadFile || !InternetCloseHandle)
    {
        return 2;
    }
    return 0;
}
int httpRead(const char *szURL,char *szBuffer,size_t size)
{
    HINTERNET hConnect;
    HINTERNET hSession;
    DWORD dwRealRead = 0;
    hSession = InternetOpenA("HTTP",
                             INTERNET_OPEN_TYPE_DIRECT,
                             NULL,
                             INTERNET_INVALID_PORT_NUMBER,
                             0);
    if(!hSession) return 0;
    hConnect = InternetOpenUrlA(hSession,
                                szURL,
                                NULL,
                                0,
                                INTERNET_FLAG_DONT_CACHE,
                                0);
    if(!hConnect)
    {
        InternetCloseHandle(hSession);
        return 0;
    }
    InternetReadFile(hConnect, szBuffer, size, &dwRealRead);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hSession);
    if(dwRealRead==0)return 0;

    return dwRealRead;
}

int resoveConnInfo(const char *connInfo,char *addr,unsigned short *port)
{
    int size;
    size=strlen(connInfo);
    if(size<10 || size>21) return 1;
    unsigned char _addr[4];
    unsigned short con_port;
    int ret=sscanf(connInfo,"%hhu.%hhu.%hhu.%hhu:%hu",_addr,_addr+1,_addr+2,_addr+3,&con_port);
    if(ret==5 && _addr[0]>0 && _addr[1]>0 && _addr[2]>0 && _addr[3]>0 && con_port>0)
    {
        int i;
        //sprintf(addr,"%hhd.%hhd.%hhd.%hhd",_addr,_addr+1,_addr+2,_addr+3);
        for(i=0;connInfo[i] && connInfo[i]!=':';i++);
        strncpy(addr,connInfo,i);
        addr[i]='\0';
        *port=con_port;
        return 0;
    }else{
        return 2;
    }

}
