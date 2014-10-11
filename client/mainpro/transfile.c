#include "transfile.h"
#include "common.h"
#include "socket.h"
#include <windows.h>
#include <limits.h>
#include <stdint.h>
#include <assert.h>

HANDLE hSendFile;
HANDLE hRecvFile;
HANDLE send_thread;
__int64 bytesRead=0;
__int64 bytesWriten=0;

/*
__int64 GetFileSizeByHandle(HANDLE fh) {
	BY_HANDLE_FILE_INFORMATION info
	if(!GetFileInformationByHandle(fd, &info)) return -1;
}*/

__int64 getFileSize(const char *fileName)
{
	fprintf(stderr, "getFileSize(%p<%s>)\n", fileName, fileName);
    //char saveFileName[MAX_PATH];
    __int64 fileSize;
    HANDLE hFind;
    //strcpy(saveFileName,selfDirectory);
    //strcat(saveFileName,"download/");
    //strcat(saveFileName,fileName);			// Don't concatenate selfDirectory in this function
    //if(*fileName != '/')
    //fprintf(stderr, "saveFileName: \"%s\"\n", saveFileName);
    WIN32_FIND_DATAA fileInfo;
    hFind=FindFirstFileA(fileName,&fileInfo);
    //fprintf(stderr, "getFileSize: LastError: %lu\n", GetLastError());
/*
	unsigned long int we = GetLastError();
	char *msg = NULL;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, we, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), &msg, 0, NULL);
	fprintf(stderr, "Windows Error Code: %s (%lu)\n", msg, we);
	free(msg);
*/
    if(hFind==INVALID_HANDLE_VALUE) return -1;
    CloseHandle(hFind);
    fileSize=fileInfo.nFileSizeLow;
    fileSize|=((__int64)fileInfo.nFileSizeHigh)<<sizeof(DWORD)*8;
    return fileSize;
}

char *CombineApplicationPath(const char *pathname) {
	static char buffer[PATH_MAX];
	static size_t appath_len = 0;
	if(!appath_len) appath_len = strlen(selfDirectory);
	if(appath_len + strlen(pathname) + 1 > PATH_MAX) return NULL;
	strcpy(buffer, selfDirectory);
	strcat(buffer, pathname);
	return buffer;
}

__int64 uploadRequest(const char *fileName)
{
    if(hRecvFile!=INVALID_HANDLE_VALUE)
        CloseHandle(hRecvFile);
    hRecvFile=INVALID_HANDLE_VALUE;
    bytesWriten=0;

    return getFileSize(CombineApplicationPath(fileName));

}

__int64 uploadTrunc(const char *fileName)
{
    char saveFileName[MAX_PATH];
    if(hRecvFile!=INVALID_HANDLE_VALUE)
        CloseHandle(hRecvFile);
    hRecvFile=INVALID_HANDLE_VALUE;
    bytesWriten=0;

    strcpy(saveFileName,selfDirectory);
    //strcat(saveFileName,"download/");
    strcat(saveFileName,fileName);
    hRecvFile = CreateFileA(saveFileName,
                        GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL,
                        CREATE_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);
    if(hRecvFile==INVALID_HANDLE_VALUE)
        return -1;
    else
        return 0;
}

__int64 uploadAppend(const char *fileName)
{
    __int64 fileSize;
    char saveFileName[MAX_PATH];

    if(hRecvFile!=INVALID_HANDLE_VALUE)
        CloseHandle(hRecvFile);
    hRecvFile=INVALID_HANDLE_VALUE;
    bytesWriten=0;

    strcpy(saveFileName,selfDirectory);
    //strcat(saveFileName,"download/");
    strcat(saveFileName,fileName);
    bytesWriten=fileSize=getFileSize(CombineApplicationPath(fileName));
    hRecvFile = CreateFileA(saveFileName,
                        GENERIC_WRITE,
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);
    // The type of the argument 4 of this function is DWORD (unsigned long int), not pointer, so it should be 0 not NULL.
    SetFilePointer(hRecvFile, (long int)fileSize, (long int *)((char *)&fileSize+sizeof(long)), 0);	// No DWORD at arg 2, 3
    if(hRecvFile==INVALID_HANDLE_VALUE)
        return -1;
    else
        return fileSize;
}

int uploadSaveData(const char *data,int size)
{
    DWORD realWrite;
    BOOL ret;
    ret=WriteFile(hRecvFile,data,size,&realWrite,0);
    bytesWriten+=realWrite;
    return ret?bytesWriten:-1;
}

void uploadTerminate()
{
    if(hRecvFile!=INVALID_HANDLE_VALUE)
        CloseHandle(hRecvFile);
    hRecvFile=INVALID_HANDLE_VALUE;
    bytesWriten=0;
}

__int64 uploadWritenSize()
{
    return bytesWriten;
}

int64_t downloadRequest(const char *pathname) {
	fprintf(stderr, "function: downloadRequest(%p<%s>)\n", pathname, pathname);
	hSendFile = CreateFileA(pathname, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	fprintf(stderr, "CreateFileA returned: %p\n", hSendFile);
	if(hSendFile == (void *)-1) return -1;
	return getFileSize(pathname);
}

static unsigned long int __stdcall downloadReadData_internal(void *a) {
	//unsigned char buffer[BUF_SIZE];
	unsigned long int s = 0;
	do {
		Sleep(1);
		DownloadFeedback *feedback = malloc(sizeof(DownloadFeedback) + BUF_SIZE);
		feedback->flag = TR_Data;
		fprintf(stderr, "TR_Data = %d, feedback->flag = %d\n", TR_Data, feedback->flag);
		//feedback->size = BUF_SIZE;
		if(!ReadFile(hSendFile, feedback->data, BUF_SIZE, &s, NULL)) {
			fprintf(stderr, "ReadFile: %lu\n", GetLastError());
			return -1;
		}
		feedback->size = s;
		sendToServer(ToServer_DownloadFileBack, (unsigned char *)feedback);
	} while(s == BUF_SIZE);
	CloseHandle(hSendFile);
	return 0;
}

int downloadReadData() {
	assert(hSendFile && hSendFile != (void *)-1);
	//Sleep(5000);
	send_thread = CreateThread(NULL, 0, downloadReadData_internal, NULL, 0, NULL);
	return send_thread ? 0 : -1;
}

void downloadTerminate() {
	assert(hSendFile && hSendFile != (void *)-1);
	assert(send_thread);
	TerminateThread(send_thread, 0);
	CloseHandle(hSendFile);
}
