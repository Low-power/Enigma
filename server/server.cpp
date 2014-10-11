#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QEventLoop>
#include <QtCore/QList>
#include <QtCore/QVector>
#include <QtCore/QTime>
#include <QtCore/QTimer>
//#include <QtCore/QThread>
#include "server.h"
#define NET_HEADERMARK "START"
Server::Server(QObject *parent) :
    QObject(parent)
{
    keepListenning=false;
}
void Server::acceptConnection()
{
    qDebug("acceptConnection()");
    QTcpSocket *clientConnection;

    clientConnection = server->nextPendingConnection();
    //qDebug() << clientConnection->peerName();
    clientConnection->setObjectName(QDateTime::currentDateTime().toString("ddhhmmsszzz"));

    qDebug()<<QDateTime::currentDateTime().toString("ddhhmmsszzz");
    if(!keepListenning)
    {
	//if(keepListenning)qDebug("1");else qDebug("0");
        clientConnection->write("Fuck U!");
        clientConnection->close();
        return;
    }
    qDebug("新服务端.");
    ClientInfo *clientInfo=new ClientInfo;
    clientInfo->client=clientConnection;
    clientInfo->time=QDateTime::currentDateTime().toString("MM-dd hh:mm:ss");
    clientInfo->ip=clientConnection->peerAddress().toString();
    clientInfo->batchScreenBuffer="!>>>";
    clientInfo->batchRunning=false;
    clientInfo->remarks="";
    clientsList.append(clientInfo);

    qDebug("有新连接！");

    emit needRefreshItem(clientsList.count()-1,Refresh_Add);
    //if(currentClientIndex<0) currentClientIndex=0;
    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    connect(clientConnection, SIGNAL(disconnected()),this,SLOT(disconnected()));
    //connect(clientConnection,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
}
Server::~Server()
{
    for(int i=0;i<clientsList.count();i++)
    {
        clientsList[i]->client->close();
        delete clientsList[i]->client;
    }
}

void Server::startListen()
{
    keepListenning=true;
}

void Server::pauseListen()
{
    keepListenning=false;
}

bool Server::initServer(ushort port)
{
    server = new QTcpServer();
    if(!server->listen(QHostAddress::Any,port))
    {
        server->close();
        return false;
    }
    connect(server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    return true;
}


ushort Server::getClientsCount()
{
    return clientsList.count();
}

bool Server::sendToCurrentClient(QByteArray data)
{
    if(currentClientIndex<0) return false;
    int32_t bufferSize=data.size();
    data.insert(0, (const char *)&bufferSize, sizeof(int32_t));
    clientsList[currentClientIndex]->client->write(data);
    return true;
}

bool Server::sendToClient(QByteArray *data,QString client)
{
	qDebug("function: sendToClient(%p, QString<%s>)", data, client.toLocal8Bit().data());
	int32_t bufferSize=data->size();
	data->insert(0, (const char *)&bufferSize, sizeof(int32_t));
	ClientInfo *clientInfo = getClientInfo(client);
	return clientInfo && clientInfo->client->write(*data) >= 0;
}

void Server::dataReceived()
{
    qDebug("新数据");
    QTcpSocket *svr=(QTcpSocket*)sender();
    //qDebug() << svr->objectName();

    int idx=existClient(svr->objectName());
    if(idx<0)return ;

    int32_t bufferSize;

require:

    if((size_t)svr->bytesAvailable()<=sizeof(int32_t)) return;
    svr->read((char*)&bufferSize,sizeof(int32_t));
    if(svr->bytesAvailable()>=bufferSize)
    {
        qDebug("新消息");

        qDebug() << bufferSize;
        QByteArray* data;
        //data=new QByteArray();
        data=new QByteArray(svr->read(bufferSize),bufferSize);
        qDebug() << *data;
        emit dataReceived(idx,data);
        goto require;
    }
    else
    {
        for(int i=sizeof(bufferSize)-1;i>=0;i--)
        {
            svr->ungetChar(((char*)&bufferSize)[i]);
        }
    }
}

ClientInfo *Server::getCurrentClientInfo()
{
    return clientsList[currentClientIndex];
}

ClientInfo *Server::getClientInfo(QString client)
{
    short idx=existClient(client);
    //qDebug()<<idx;
    if(idx<0) return NULL;
    return clientsList[idx];
}

void Server::setCurrentIndex(short index)
{
    currentClientIndex=index;
}
short Server::getCurrentIndex()
{
    return currentClientIndex;
}

short Server::existClient(QString objectName)
{
    qDebug("existClient()");
    qDebug() << &clientsList;
    for(short i=0;i<clientsList.count();i++)
    {
        qDebug()<<clientsList[i]->client->objectName();
        if(clientsList[i]->client->objectName()==objectName)
            return i;
    }
    return -1;
}

void Server::disconnected()
{
    qDebug("slot: Server::disconnected()");
    for(int i=0; i<clientsList.count(); i++)
    {
	if(clientsList[i]->client->state() == QTcpSocket::UnconnectedState)
        {
	    clientsList.remove(i, 1);
	    emit needRefreshItem(i, Refresh_Remove);
            qDebug("删除项");
            i--;
        }
    }
    if(clientsList.count() ==0) {
        currentClientIndex = -1;
    }else if(currentClientIndex > clientsList.count()-1)
    {
        currentClientIndex =0;
    }
}

void Server::displayError(QAbstractSocket::SocketError err)
{
    qDebug("网络错误 %d",err);
}
