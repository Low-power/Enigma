#ifndef TRANSFILE_H
#define TRANSFILE_H
#include "common.h"
#include <stdint.h>

#ifndef __GNUC__
#define __attribute__(E)
#endif

#define BUF_SIZE 2048

typedef enum _TransmatingFlag{
    TR_Request,
    TR_Trunc,
    TR_Append,
    TR_Data,
    TR_Terminate
} TransmatingFlag;

#pragma pack(push,1)

typedef struct UploadFeedbackStruct
{
   char flag;
   __int64 size;
} __attribute__((packed)) UploadFeedback;

typedef struct {
	unsigned char flag;
	int64_t size;
	unsigned char data[0];
} __attribute__((packed)) DownloadFeedback;

#pragma pack(pop)
extern HANDLE hSendFile;
extern HANDLE hRecvFile;
extern __int64 bytesRead;
extern __int64 bytesWriten;
extern __int64 uploadRequest(const char *fileName);
extern __int64 uploadTrunc(const char *fileName);
extern __int64 uploadAppend(const char *fileName);
extern int uploadSaveData(const char *data,int size);
extern void uploadTerminate(void);
extern __int64 uploadWritenSize(void);

extern int64_t downloadRequest(const char *);
extern int downloadReadData(void);
extern void downloadTerminate(void);

#endif
