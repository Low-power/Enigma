#include <windows.h>
#include <windef.h>
#include <stdio.h>

#ifndef VER_PLATFORM_WIN32_CE
#define VER_PLATFORM_WIN32_CE 3
#endif

int getSystemVersion(void);
int GetReleaseName(char *buffer, const OSVERSIONINFO *os);

int getSystemVersion()
{
    OSVERSIONINFO version;
    version.dwOSVersionInfoSize = sizeof version;
    if(!GetVersionEx(&version)) {
	perror("GetVersionEx");
        return 1;
    }

    char vs[512];
    char *tmp = vs;
    tmp += sprintf(tmp, "Windows");
    tmp += GetReleaseName(tmp, &version);
    tmp += sprintf(tmp, " [Version: %lu.%lu.%lu]", version.dwMajorVersion, version.dwMinorVersion, version.dwBuildNumber);


    wchar_t wvs[tmp-vs+1];
    mbstowcs(wvs, vs, tmp-vs+1);
    MessageBoxW(NULL, wvs, L"Information", 0);
    return 0;
}

int GetReleaseName(char *buffer, const OSVERSIONINFO *os) {
    //int r = 0;
    switch(os->dwMajorVersion) {						//判断主版本号
        case 4:
            switch(os->dwMinorVersion) {
                case 0:
                    if(os->dwPlatformId == VER_PLATFORM_WIN32_NT)
                        return sprintf(buffer, " NT");				//1996年7月发布
                    else if(os->dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
                        return sprintf(buffer, " 95 (9x)");
                    break;
                case 10: return sprintf(buffer, " 98 (9x)");
                case 21: return sprintf(buffer, " CE Pocket PC 2003 Second Edition");
                case 90: return sprintf(buffer, " Me (9x)");
            }
            break;
        case 5:
            switch(os->dwMinorVersion) {
                case 0: return sprintf(buffer, " 2000 (NT)");				//1999年12月发布
                case 1: return sprintf(buffer, " XP (NT)");				//2001年8月发布

/*
                case 2:
                    if(os->wProductType==VER_NT_WORKSTATION && info.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
                        return sprintf(buffer, " XP 64-Bit (NT)");
                    else if(GetSystemMetrics(SM_SERVERR2)==0)
                        return sprintf(buffer, " Server 2003 (NT)");		//2003年3月发布
                    else if(GetSystemMetrics(SM_SERVERR2)!=0)
                        return sprintf(buffer, " Server 2003 R2 (NT)");
                    break;
*/
                case 2:
                    if(os->dwPlatformId == VER_PLATFORM_WIN32_CE) {
                        return sprintf(buffer, " Mobile 6.x (CE)");
                    }
                    break;
            }
            //break;
            return sprintf(buffer, " NT");
#if 0
        case 6:
/*
            switch(os->dwMinorVersion) {
                case 0:
                    if(os->wProductType==VER_NT_WORKSTATION)
                        return sprintf(buffer, " Vista (NT)");
                    else
                        return sprintf(buffer, " Server 2008 (NT)");		  //服务器版本
                    break;
                case 1:
                    if(os->wProductType==VER_NT_WORKSTATION)
                        return sprintf(buffer, " 7 (NT)");
                    else
                        return sprintf(buffer, " Server 2008 R2 (NT)");
                    break;
            }*/
            //break;
            return sprintf(buffer, " NT");
#endif
    }
    if(os->dwPlatformId == VER_PLATFORM_WIN32_NT) return sprintf(buffer, " NT");
    if(os->dwPlatformId == VER_PLATFORM_WIN32_CE) return sprintf(buffer, " CE");
    return 0;
}
