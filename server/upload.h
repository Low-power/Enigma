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
