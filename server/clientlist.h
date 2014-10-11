#ifndef CLIENTLIST_H
#define CLIENTLIST_H
#include <QApplication>
class ClientList
{
public:
    enum DataType
    {
        CMDLINE=0,
        CTRL=1,
        MESSAGE=2
    };
public:
    ClientList();
    void startListen();
    void stopListen();
    void refreshClientList();
    void sendData(DataType type,QString *data);



private:


private slots:

};

#endif // CLIENTLIST_H
