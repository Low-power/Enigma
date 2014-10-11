#undef UNICODE
#undef _UNICODE
#include "common.h"
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0501

#include <windows.h>
// Some older SDK didn't have this header
//#include <wtsapi32.h>
#include "wtsapi32.h"

char clientUniqueId[17];
char selfFileName[MAX_PATH];
char selfDirectory[MAX_PATH];
char profileFileName[MAX_PATH];
char systemDirectory[MAX_PATH];
char userName[65];
BOOL initVariables(void);
void createUniqueId(char *lpUniqueId);
void initAllModule(void);
Proc_WTSGetActiveConsoleSessionId WTSGetActiveConsoleSessionId;
BOOL getUserName(TCHAR *szUsername)
{
    BOOL bRet = FALSE;
    TCHAR *szLogName = NULL;
    DWORD dwSize = 0;

    if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
                                   WTSGetActiveConsoleSessionId(),
                                   WTSUserName,
                                   &szLogName,
                                   &dwSize))
    {
        memcpy(szUsername,szLogName,dwSize);
        WTSFreeMemory(szLogName);
        bRet = TRUE;
    }

    return bRet;
}

BOOL initBase()
{
    initVariables();
    SetCurrentDirectoryA(selfDirectory);

    HANDLE hInitFile;
    hInitFile = CreateFileA(profileFileName,
                        GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL,
                        OPEN_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);
    CloseHandle(hInitFile);
    /*
    if(GetPrivateProfileIntA("common","R",0,profileFileName)==0)
    {
        if(!addRegRunKey(selfFileName,MAX_PATH))
            WritePrivateProfileStringA("common","R","1",profileFileName);
    }
    */
    GetPrivateProfileStringA("BASE","UID","\0",clientUniqueId,17,profileFileName);
    if(clientUniqueId[0]=='\0')
    {
        createUniqueId(clientUniqueId);
        WritePrivateProfileStringA("BASE","UID",clientUniqueId,profileFileName);
    }
    if(!getUserName(userName))return FALSE;
    return TRUE;
}

BOOL initVariables()
{
    char *p,*q=NULL;
    GetModuleFileNameA(GetModuleHandleA(NULL),selfFileName,MAX_PATH);
    strcpy(selfDirectory,selfFileName);
    for(p=selfDirectory;*p;p++)
        if(*p=='\\')q=p+1;
    *q='\0';
    strcpy(profileFileName,selfDirectory);
    strcat(profileFileName,"config.ini");

    GetSystemDirectoryA(systemDirectory,MAX_PATH);

    WTSGetActiveConsoleSessionId  =  (Proc_WTSGetActiveConsoleSessionId)
            GetProcAddress(LoadLibrary(_T("Kernel32.dll")), "WTSGetActiveConsoleSessionId");
    if (!WTSGetActiveConsoleSessionId)
        return FALSE;
    return TRUE;
}

void createUniqueId(char *lpUniqueId)
{
    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);
    sprintf(clientUniqueId,"%4d%02d%02d%02d%02d%02d%02d",
            sysTime.wYear,
            sysTime.wMonth,
            sysTime.wDay,
            sysTime.wHour,
            sysTime.wMinute,
            sysTime.wSecond,
            sysTime.wMilliseconds/10);

}
/*
int addRegRunKey(char *fileName,size_t size)
{
    HKEY hRoot = HKEY_LOCAL_MACHINE;
    char*szSubKey ="Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    HKEY hKey;

    DWORD dwDisposition = REG_OPENED_EXISTING_KEY;
    LONG lRet = RegCreateKeyExA(hRoot, szSubKey, 0, NULL,
            REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
    printf("error=%d\n",lRet);
    if(lRet != ERROR_SUCCESS)
        return 1;
    lRet = RegSetValueExA(hKey, "Dr.com Service", 0, REG_SZ, (BYTE*)fileName, size);
    RegCloseKey(hKey);
    if(lRet == ERROR_SUCCESS)
    {
        printf("add Runkey succeed");
        return 0;
    }
    return 2;
}
*/
