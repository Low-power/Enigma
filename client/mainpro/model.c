#include "model.h"
#include "transfile.h"
#include <stdlib.h>
#include <string.h>
const char *headerTable[30];
void initHeaderList();

DataType demodulateData(const unsigned char*src,
		size_t size,
		void **result);

void initHeaderList()
{

	headerTable[ToClient_CommandLine]=         "CMDLINE :";
	headerTable[ToClient_Message]=             "MESSAGE :";
	headerTable[ToClient_SystemInfo]=          "SYSINFO :";
	headerTable[ToClient_Verification]=        "VERIFCTN:";
	headerTable[ToClient_Command]=             "COMMAND :";
	headerTable[ToClient_Command_Shutdown]=    "SHUTDOWN:";
	headerTable[ToClient_Command_Logoff]=      "LOGOFF  :";
	headerTable[ToClient_Command_Restart]=     "RESTART :";

	headerTable[ToServer_CmdLineFeedBack]=     "CMDRSLT :";
	headerTable[ToServer_ConnectionInfo]=      "CONTINFO:";
	headerTable[ToServer_SystemInfo]=          "SYSINFO :";
	headerTable[ToServer_CloseMessageBox]=     "CLOSEMSG:";

	headerTable[ToClient_DownloadFile]=        "DNLDFILE:";
	headerTable[ToServer_DownloadFileBack]=    "DNLDBACK:";

	headerTable[ToClient_UploadFile]=          "UPLDFILE:";
	headerTable[ToServer_UploadFileBack]=      "UPLDBACK:";

}



DataType checkData(const unsigned char *src)
{
	return -1;
}

DataType demodulateData(const unsigned char*src,size_t size,void **result)
{
	char mark[10];
	int needResault=0;
	DataType type;
	*result=0;
	if(size<9) return Type_UnKnow;

	memcpy(mark,src,9);
	mark[9]=0;
	needResault=0;
	if(!strcmp(mark,headerTable[ToClient_CommandLine]))
	{
		type=ToClient_CommandLine;
		needResault=1;
	}
	else if(!strcmp(mark,headerTable[ToClient_Message]))
	{
		type=ToClient_Message;
		needResault=1;
	}
	else if(!strcmp(mark,headerTable[ToClient_UploadFile]))
	{
		puts("mark=uploadfile");
		type=ToClient_UploadFile;
		needResault=1;
	}
	else if(!strcmp(mark,headerTable[ToClient_DownloadFile]))
	{
		type=ToClient_DownloadFile;
		needResault=1;
	}
	else if(!strcmp(mark,headerTable[ToClient_SystemInfo]))
	{
		type=ToClient_SystemInfo;
		needResault=1;
	}
	else if(!strcmp(mark,headerTable[ToClient_Verification]))
	{
		type=ToClient_Verification;
		needResault=1;
	}
	else if(!strcmp(mark,headerTable[ToClient_Command_Shutdown]))
	{
		type=ToClient_Command_Shutdown;
		needResault=0;
	}
	else if(!strcmp(mark,headerTable[ToClient_Command_Logoff]))
	{
		type=ToClient_Command_Logoff;
		needResault=0;
	}
	else if(!strcmp(mark,headerTable[ToClient_Command_Restart]))
	{
		type=ToClient_Command_Restart;
		needResault=0;
	}
	else if(!strcmp(mark,headerTable[ToClient_Command_Uninstall]))
	{
		type=ToClient_Command_Uninstall;
		needResault=0;
	}

	else
	{
		puts("解析错误 .");
		type=Type_UnKnow;
		needResault=0;
	}
	if(needResault)
	{
		*result=(char*)src+9;
	}
	return type;
}

// 返回调制数据长度
int modulateData(DataType type,
		const unsigned char*src,
		char *dst,
		size_t max_size)
{
	int effSize=0;
	unsigned char *p;
	p = (unsigned char *)dst;
	switch(type)
	{
		case ToServer_CmdLineFeedBack:
		{
			int srcSize=strlen((const char *)src)+sizeof("");
			effSize=srcSize+9;
			if(effSize > max_size - sizeof(int)) {
				int drop_size = effSize - max_size;
				src += drop_size;
				srcSize -= drop_size;
				effSize -= drop_size;
			}
			if(dst) {
				memcpy(p,&effSize,sizeof(int)),p+=sizeof(int);
				memcpy(p,headerTable[ToServer_CmdLineFeedBack],9),p+=9;
				//printf("srcSize = %d\n*src = %hhu", srcSize, *src);
				memcpy(p,src,srcSize);
			}
			break;
		}
		case ToServer_ConnectionInfo:
		{
			int srcSize=sizeof(ConnectionInfo);
			effSize=srcSize+9;
			memcpy(p,&effSize,sizeof(int)),p+=sizeof(int);
			memcpy(p,headerTable[ToServer_ConnectionInfo],9),p+=9;
			memcpy(p,src,srcSize);
			break;
		}
		case ToServer_SystemInfo:
		{
			int srcSize=strlen((const char *)src)+sizeof("");
			effSize=srcSize+9;
			memcpy(p,&effSize,sizeof(int)),p+=sizeof(int);
			memcpy(p,headerTable[ToServer_SystemInfo],9),p+=9;
			memcpy(p,src,srcSize);
			break;
		}
		case ToServer_CloseMessageBox:
		{
			int srcSize=strlen((const char *)src)+sizeof("");
			effSize=srcSize+9;
			memcpy(p,&effSize,sizeof(int)),p+=sizeof(int);
			memcpy(p,headerTable[ToServer_CloseMessageBox],9),p+=9;
			memcpy(p,src,srcSize);
			break;
		}
		case ToServer_UploadFileBack:
		{
			int srcSize=sizeof(UploadFeedback);
			effSize=srcSize+9;
			memcpy(p,&effSize,sizeof(int)),p+=sizeof(int);
			memcpy(p,headerTable[ToServer_UploadFileBack],9),p+=9;
			memcpy(p,src,srcSize);
			break;
		}
		case ToServer_DownloadFileBack: {
			DownloadFeedback *feedback = (DownloadFeedback *)src;
			int srcSize = sizeof(DownloadFeedback);
			fprintf(stderr, "feedback->flag = %d\n", feedback->flag);
			if(feedback->flag == TR_Data) srcSize += feedback->size;
			effSize=srcSize+9;
			memcpy(p,&effSize,sizeof(int)),p+=sizeof(int);
			memcpy(p,headerTable[ToServer_DownloadFileBack],9),p+=9;
			memcpy(p,src,srcSize);
			break;
		}
		default:
			fprintf(stderr, __FILE__ ":%d: warning: modulateData: unknown type %d\n", __LINE__, type);
			return 0;
	}
	//printf("size=%d\n",effSize+sizeof(int));
	return effSize+sizeof(int);
}
