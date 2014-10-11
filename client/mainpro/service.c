#include "service.h"
#include <windows.h>

int createProcess(char *szFileName, RunType type)
{
	int ret = -1;		// returns -1 if the type is unknown
	switch(type)
	{
		case RUNAS:
			ret = (int)ShellExecuteA(NULL,
						"runas",
						szFileName,
						NULL,
						NULL,
						SW_HIDE);
			ret= ret>32 ? 0 : 1;
			break;
		case MAPPING:
			break;
	}
	return ret;
}
/*

#include <stdio.h>
#define LOGFILE "c:/log.txt"
DWORD _stdcall WTSGetActiveConsoleSessionId(void);
DWORD _stdcall CreateEnvironmentBlock(void*,HANDLE,BOOL);
int WriteToLog(char* str)
{
FILE* log;
log = fopen(LOGFILE, "a+");
if (log == NULL)
return -1;
fprintf(log, "%s\n", str);
fclose(log);
return 0;
}
VOID WINAPI ServiceHandler(DWORD dwControl)
{
switch (dwControl)
{
case SERVICE_CONTROL_STOP:
{
ExitProcess(0);
}
break;
}
}

DWORD WINAPI ThreadFunc(PVOID pv)
{
HANDLE hTokenThis = NULL;
HANDLE hTokenDup = NULL;
HANDLE hThisProcess;
DWORD dwSessionId;
STARTUPINFO si;
PROCESS_INFORMATION pi;
LPVOID pEnv;
DWORD dwCreationFlag;
hThisProcess = GetCurrentProcess();
OpenProcessToken(hThisProcess, TOKEN_ALL_ACCESS, &hTokenThis);
DuplicateTokenEx(hTokenThis, MAXIMUM_ALLOWED,NULL, SecurityIdentification, TokenPrimary, &hTokenDup);
dwSessionId = WTSGetActiveConsoleSessionId();
SetTokenInformation(hTokenDup, TokenSessionId, &dwSessionId, sizeof(DWORD));


ZeroMemory(&si, sizeof(STARTUPINFO));
ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
si.cb = sizeof(STARTUPINFO);
//si.lpDesktop = "WinSta0\\default";

pEnv = NULL;
dwCreationFlag = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE;

CreateEnvironmentBlock(&pEnv, hTokenDup, FALSE);

CreateProcessAsUser(
hTokenDup,
NULL,
(char *)"notepad.exe",
NULL,
NULL,
FALSE,
dwCreationFlag,
pEnv,
NULL,
&si,
&pi);
WriteToLog("runned");
Sleep(100 * 1000);
return TerminateProcess(GetCurrentProcess(), 1);
}

VOID WINAPI ServiceMain(DWORD  dwNumServicesArgs, PSTR *ppcServiceArgVectors)
{
	SERVICE_STATUS_HANDLE hService = NULL;
	SERVICE_STATUS SrvStatus = { 0 };
	OutputDebugStringA("service main.....");
	hService = RegisterServiceCtrlHandlerA("winservice", ServiceHandler);
	SrvStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	SrvStatus.dwCurrentState = SERVICE_RUNNING;
	SrvStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	SetServiceStatus(hService, &SrvStatus);
	CreateThread(0, 0, ThreadFunc, 0, 0, 0);
	return;
}

void startService()
{
	SERVICE_TABLE_ENTRYA scArrTable[] =
	{
		{ "winservice", ServiceMain },
		{ NULL, NULL }
	};
	StartServiceCtrlDispatcherA(scArrTable);
	return ;
}
*/
