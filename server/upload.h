/*	A part of Enigma
	Copyright 2014 Ixil
	Copyright 2015 libdll.so

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef _UPLOAD_H
#define _UPLOAD_H
#include "server.h"
#include "mainwindow.h"
#include "common.h"
#include <QtCore/QThread>

/*
#ifndef __GNUC__
#define __attribute__(E)
#endif
*/

class QTreeWidget;


class Upload : public QThread
{
    Q_OBJECT
public:
    explicit Upload(Server *server);
#pragma pack(push,1)
    struct Feedback
    {
       char flag;
       qint64 size;
    } __attribute__((packed));
#pragma pack(pop)
    bool requestTruncFile();
    bool requestAppendFile(qint64 offset);
    bool requestUpload(QString clientName,QString fileName);
    void requestSendData();
    bool requestTerminate();

    void setUploadInfo(QString uniqueName,
                       QString fileName,
                       qint64 offset);

    qint64 fileSize;

private:
    UploadInfo state;
    Server *server;
    QString uniqueName;
    QString fileName;
    QString fullFileName;
    qint64 readOffset;
    void run();
signals:

public slots:

};

#endif
