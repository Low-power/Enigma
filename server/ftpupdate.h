/*	A part of Enigma
	Copyright 2014 Ixil

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef FTPUPDATE_H
#define FTPUPDATE_H
#include <QObject>
class QFtp;
class FtpUpdate : public QObject
{
    Q_OBJECT
private:
    QString host;
    QString userName;
    QString passWord;
    int port;
    QFtp *ftp;

public:
    FtpUpdate(QString host,QObject *parent=0);
    ~FtpUpdate();
    void setAccount(QString userName,QString passWord);
    void setPort(int port=21);
    void setHost(QString host);
    bool upDate(QString data,QString fileName);
private slots:
    void ftpDone(bool error);
    void updateDataTranferProgress(qint64 readBytes, qint64 totalBytes);

};

#endif // FTPUPDATE_H
