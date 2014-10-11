#ifndef COMMON_H
#define COMMON_H
#include <windows.h>
#include <windef.h>
#include <tchar.h>
#include <stdio.h>
//#include <stdlib.h>
#include <limits.h>

extern char selfFileName[MAX_PATH];

extern char selfDirectory[MAX_PATH];

extern char clientUniqueId[17];

extern BOOL initBase();

extern char userName[65];

typedef DWORD (WINAPI  *Proc_WTSGetActiveConsoleSessionId)(VOID);
extern Proc_WTSGetActiveConsoleSessionId WTSGetActiveConsoleSessionId;
#endif // COMMON_H
