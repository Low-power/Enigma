#ifndef DATAANALYSIS_H
#define DATAANALYSIS_H

#include "common.h"
#include <QtCore/QObject>

/*
#ifndef __GNUC__
#define __attribute__(E)
#endif
*/

#pragma pack(push,1)
typedef struct _ConnectionInfo{
    char key[16];
    char uniqueID[16];
    char userName[65];
    char computerName[65];
} __attribute__((packed)) ConnectionInfo;
#pragma pack(pop)
#pragma pack(push,1)
typedef struct NetMessageStruct
{
    char mode;
    short time;
    char caption[64];
} __attribute__((packed)) NetMessage;
#pragma pack(pop)
class DataAnalysis
{
private:
    static const char *headerTable[50];
public:
    typedef enum _DataType{
        TypeError,

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

    static void initEnvironment();

    static DataType checkData(const QByteArray*src);

    static DataType demodulateData(const QByteArray*src,QByteArray *&result);

    static QByteArray *modulateData(DataType type,const void*src);//返回值需要delete


};
#endif // DATAANALYSIS_H
