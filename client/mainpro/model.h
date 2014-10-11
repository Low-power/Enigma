#ifndef MODEL_H
#define MODEL_H
#include <stdlib.h>

#ifndef __GNUC__
#define __attribute__(E)
#endif

typedef enum _DataType{
    Type_UnKnow,

    ToClient_CommandLine,
    ToClient_Message,
    ToClient_Verification,
    ToClient_SystemInfo,
    ToClient_Command,
    ToClient_Command_Shutdown,
    ToClient_Command_Logoff,
    ToClient_Command_Restart,
    ToClient_Command_Uninstall,


    ToServer_CmdLineFeedBack,
    ToServer_ConnectionInfo,
    ToServer_SystemInfo,
    ToServer_CloseMessageBox,

    ToClient_DownloadFile,
    ToClient_UploadFile,
    ToServer_DownloadFileBack,
    ToServer_UploadFileBack


}DataType;
#pragma pack(push,1)
typedef struct NetMessageStruct
{
    char mode;
    short time;
    char caption[64];

} __attribute__((packed)) NetMessage;
#pragma pack(pop)
#pragma pack(push,1)
typedef struct _ConnectionInfo{
    char key[16];
    char uniqueID[16];
    char userName[65];
    char computerName[65];
} __attribute__((packed)) ConnectionInfo;
#pragma pack(pop)
void initHeaderList();

//返回调制数据长度
int modulateData(DataType type,
                  const unsigned char*src,
                  char *dst, size_t);
DataType demodulateData(const unsigned char*src,
                        size_t size,
                        void **result);
#endif // MODEL_H
