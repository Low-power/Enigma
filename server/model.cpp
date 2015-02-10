/*	A part of Enigma
	Copyright 2014 Ixil
	Copyright 2015 libdll.so

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include "model.h"
#include "upload.h"
#include <QtCore/QTextCodec>

const char *DataAnalysis:: headerTable[];

DataAnalysis::DataType DataAnalysis::demodulateData(const QByteArray *src, QByteArray *&result)
{
    QByteArray mark;
    QByteArray *data=new QByteArray;
    DataType type;
    if(src->size()<9) return TypeError;
    mark=src->left(9);
    //mark.append("");
    //qDebug("src=%s",src->data());
    if(mark==headerTable[ToServer_CmdLineFeedBack])
    {
	type=ToServer_CmdLineFeedBack;
	QByteArray ba = src->right(src->size()-9);
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::ConverterState state;
	QString s = codec->toUnicode(ba.data(), ba.length(), &state);
	//qDebug() << state.invalidChars;
	if(state.invalidChars > 2) {
		codec = QTextCodec::codecForName("GB18030");
		s = codec->toUnicode(ba);
	}
	data->append(s.toUtf8());
    }
    else if(mark==headerTable[ToServer_SystemInfo])
    {
        type=ToServer_SystemInfo;
        data->append(src->right(src->size()-9));
    }
    else if(mark==headerTable[ToServer_CloseMessageBox])
    {
        type=ToServer_CloseMessageBox;
        data->append(src->right(src->size()-9));
    }
    else if(mark==headerTable[ToServer_ConnectionInfo])
    {
        type=ToServer_ConnectionInfo;
        //data->append(src->right(src->size()-9));
        *data=QByteArray::fromRawData(src->constData()+9,src->size()-9);
        qDebug("连接信息： computerName = %s, userName = %s",
		((ConnectionInfo*)data->constData())->computerName,
		((ConnectionInfo*)data->constData())->userName);
    }
    else if(mark==headerTable[ToServer_UploadFileBack])
    {
        type=ToServer_UploadFileBack;
        //data->append(src->right(src->size()-9));
        *data=QByteArray::fromRawData(src->constData()+9,src->size()-9);
        qDebug("UploadFileBack");
        //qDebug("flag=%d",((Upload::Feedback*)data->constData())->flag);
        //qDebug("size=%lx",((Upload::Feedback*)data->constData())->size);
    } else if(mark == headerTable[ToServer_DownloadFileBack]) {
		type = ToServer_DownloadFileBack;
		*data=QByteArray::fromRawData(src->constData()+9,src->size()-9);
    }
    else
    {
        delete data;
        result=NULL;
        return TypeError;
    }
    result=data;
    return type;

}

QByteArray *DataAnalysis::modulateData(DataType type,const void*src)
{
	qDebug("function: modulateData(%d, %p)", type, src);
    QByteArray *buffer =new QByteArray();
    switch(type)
    {
    case ToClient_CommandLine:
        buffer->append(headerTable[ToClient_CommandLine]);
        buffer->append(*(QByteArray*)src);
        buffer->append('\0');
        break;
    case ToClient_Message:
        buffer->append(headerTable[ToClient_Message]);
        buffer->append(*(QByteArray*)src);
        buffer->append('\0');
        break;
    case ToClient_Verification:
        buffer->append(headerTable[ToClient_Verification]);
        buffer->append(*(QByteArray*)src);
        break;
    case ToClient_SystemInfo:
        buffer->append(headerTable[ToClient_SystemInfo]);
        break;
    case ToClient_Command_Shutdown:
        buffer->append(headerTable[ToClient_Command_Shutdown]);
        break;
    case ToClient_Command_Logoff:
        buffer->append(headerTable[ToClient_Command_Logoff]);
        break;
    case ToClient_Command_Restart:
        buffer->append(headerTable[ToClient_Command_Restart]);
        break;
    case ToClient_DownloadFile:
        buffer->append(headerTable[ToClient_DownloadFile]);
        buffer->append(*(QByteArray*)src);
	buffer->append((char)0);
        break;
    case ToClient_UploadFile:
        buffer->append(headerTable[ToClient_UploadFile]);
        buffer->append(*(QByteArray*)src);
        //buffer->append('\0');
        break;
    default:
        delete buffer;
        return NULL;
    }
    return buffer;
}

void DataAnalysis::initEnvironment()
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

DataAnalysis::DataType DataAnalysis::checkData(const QByteArray*src)
{
    return (DataType)-1;
}


