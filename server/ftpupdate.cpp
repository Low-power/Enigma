/*	A part of Enigma
	Copyright 2014 Ixil

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

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
