/*	A part of Enigma
	Copyright 2014 Ixil
	Copyright 2015 libdll.so

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef CLIENTSLIST_H
#define CLIENTSLIST_H
#include <QObject>
#include <QVector>
#include <QVariant>
#include <QAbstractSocket>
struct ClientNode;
class QTcpServer;
class QTcpSocket;
class QString;
class QByteArray;
struct ClientInfo
{
    /************************************************/
    //显式内部成员
    QTcpSocket *client;

    /************************************************/
    QString batchScreenBuffer;
    bool    batchRunning;
    QString time;
    QString ip;
    QByteArray uniqueID;
    QString computerName;
    QString userName;
    QString remarks;
    QString getRemarks()
    {
	return remarks.isEmpty()?computerName:remarks;
    }
};



class Server : public QObject
{
    Q_OBJECT
public:
    typedef enum{
        Refresh_Add,
        Refresh_Remove,
        Refresh_Update
    }RefreshType;
    explicit Server(QObject *parent = 0);
    ~Server();
    void startListen();

    void pauseListen();

    bool initServer(ushort port);

    ushort getClientsCount();

    ClientInfo *getCurrentClientInfo();

    ClientInfo *getClientInfo(QString client);

    void setCurrentIndex(short index);

    short getCurrentIndex();

    bool sendToCurrentClient(QByteArray);

    bool sendToClient(QByteArray *,QString client);

    QVector<ClientInfo*> clientsList;

    //QString currentClientIp;

private:
     short existClient(QString objectName);		// 找到返回下标，否则-1

     bool keepListenning;

     QTcpServer *server;

     //QVector<QTcpSocket*> socketsList;

     short currentClientIndex;

signals:
    void dataReceived(short idx,QByteArray*);

    void needRefreshAll();

    void needRefreshItem(ushort item, Server::RefreshType type);

private slots:
    void dataReceived();

    void acceptConnection();

    void disconnected();

    void displayError(QAbstractSocket::SocketError err);


};

#endif // CLIENTSLIST_H
