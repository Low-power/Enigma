#include "upload.h"
#include "model.h"
#include <QFile>
#include <QtNetwork/QTcpSocket>
#include <QtGui/QTreeWidget>
#include <QtGui/QMessageBox>
#include <QtCore/QFileInfo>
Upload::Upload(Server *server)
{
//this->sleep();
    this->server=server;
    readOffset=0;

}
bool Upload::requestTruncFile()
{
    QByteArray effData;
    char flag = Tr::Trunc;
    effData.append(&flag,sizeof(char));
    effData.append(fileName);
    effData.append((char)0);
    this->readOffset=0;
    QByteArray*sendData= DataAnalysis::modulateData(DataAnalysis::ToClient_UploadFile,&effData);
    if(server->sendToClient(sendData,uniqueName)==false)
    {
        delete sendData;
        return false;
    }
    delete sendData;
    return true;
}

bool Upload::requestAppendFile(qint64 offset)
{
    if(offset>=QFileInfo(fullFileName).size())
    {
        QMessageBox::information(0,tr("提示"),tr("文件上传完毕"));
        return true;
    }
    this->readOffset=offset;
    QByteArray effData;
    char flag = Tr::Append;
    effData.append(&flag,sizeof(char));
    effData.append(fileName);
    effData.append('\0');
    //effData.append((char*)&offset,sizeof(qint64));
    //effData.append((char)0);
    QByteArray*sendData= DataAnalysis::modulateData(DataAnalysis::ToClient_UploadFile,&effData);
    if(server->sendToClient(sendData,uniqueName)==false)
    {
        delete sendData;
        return false;
    }
    delete sendData;
    return true;
}

bool Upload::requestUpload(QString clientName,QString fileName)
{
    QByteArray effData;

    uniqueName=clientName;
    this->fileName=QFileInfo(fileName).fileName();
    this->fullFileName=fileName;
    this->fileSize=QFileInfo(fileName).size();
    char flag = Tr::Request;
    effData.append(&flag,sizeof(char));
    effData.append(this->fileName);
    effData.append((char)0);

    QByteArray*sendData= DataAnalysis::modulateData(DataAnalysis::ToClient_UploadFile,&effData);
    if(server->sendToClient(sendData,uniqueName)==false)
    {
        delete sendData;
        return false;
    }
    delete sendData;
    return true;
}

void Upload::requestSendData()
{
    start();
}

bool Upload::requestTerminate()
{
    QByteArray effData;
    char flag = Tr::Terminate;
    effData.append(&flag,sizeof(char));
    QByteArray*sendData= DataAnalysis::modulateData(DataAnalysis::ToClient_UploadFile,&effData);
    if(server->sendToClient(sendData,uniqueName)==false)
    {
        delete sendData;
        return false;
    }
    delete sendData;
    return true;
}

void Upload::run()
{
    //char buffer[2100];
    QByteArray readBuf,*sendBuf;
    //qint64 realRead;
    QFile file(this->fullFileName);

    char flag;
    qDebug("start upload thread");
    qDebug()<<file.open(QIODevice::ReadOnly);
    file.seek(readOffset);
    while(readBuf=file.read(2048),!readBuf.isEmpty())
    {
        qDebug("read");
	flag = Tr::Data;
        readBuf.insert(0,&flag,sizeof(char));
        sendBuf=DataAnalysis::modulateData(DataAnalysis::ToClient_UploadFile,&readBuf);
        bool ret=this->server->sendToClient(sendBuf,uniqueName);
        if(!ret)
        {
            QMessageBox::information(0,"error","发送失败");
            return;
        }
        //qDebug("sended");
        delete sendBuf;
	msleep(5);
    }
    flag = Tr::Terminate;
    QByteArray terminate;
    terminate.append(&flag,sizeof(char));
    sendBuf=DataAnalysis::modulateData(DataAnalysis::ToClient_UploadFile,&terminate);
    this->server->sendToClient(sendBuf,uniqueName);

    delete sendBuf;
    //QMessageBox::information(0,"提示","数据推送完毕");
    qDebug("数据推送完毕");
}

/*
void Upload::setUploadInfo(QString uniqueName,QString fileName,qint64 offset)
{
    this->server=server;
    this->uniqueName=uniqueName;
    this->fileName=fileName;
    this->readOffset=offset;
}
*/
