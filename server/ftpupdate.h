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
