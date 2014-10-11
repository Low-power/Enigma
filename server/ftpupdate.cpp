#include "ftpupdate.h"
#include <QFtp>
FtpUpdate::FtpUpdate(QString host,QObject *parent)
    :QObject(parent)
{
    ftp=new QFtp(this);
    this->host=host;
    connect(ftp,SIGNAL(done(bool)),this,SLOT(ftpDone(bool)));
    connect(ftp,SIGNAL(dataTransferProgress(qint64,qint64)),this,SLOT(updateDataTranferProgress(qint64,qint64)));
}
FtpUpdate::~FtpUpdate()
{
    ftp->disconnect();
    ftp->close();
    delete ftp;
}

void FtpUpdate::setAccount(QString userName,QString passWord)
{
    this->userName=userName;
    this->passWord=passWord;
}

void FtpUpdate::setPort(int port)
{
    this->port=port;
}

void FtpUpdate::setHost(QString host)
{
    this->host=host;
}

bool FtpUpdate::upDate(QString data,QString fileName)
{
    ftp->connectToHost(host,port);
    if(!ftp->login(userName,passWord))
    {
        qDebug("ftp login error");
        return false;
    }
    else
    {
        ftp->cd("/");
        ftp->put(data.toUtf8(),fileName,QFtp::Binary);
        return true;
    }

}

void FtpUpdate::ftpDone(bool error)
{
    if(error)
        qDebug("ftp upload error");
    else
        qDebug("success");
}

void FtpUpdate::updateDataTranferProgress(qint64 readBytes, qint64 totalBytes)
{
    //qDebug()<<"updateDataTranferProgress";

    qDebug(QString("%1").arg(readBytes/totalBytes*100).toUtf8().data());
}
