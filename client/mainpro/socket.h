#ifndef SOCKET_H
#define SOCKET_H
#include "model.h"

extern int initSocket(void);

extern int uninitSocket(void);

extern void setConnectionAttrib(const char *address,const unsigned short port);

extern int socketDataHander(void);

extern int sendConnectionInfo(void);

extern int connectToServer(void);

extern int socketDataHander(void);

extern int sendToServer(DataType type,unsigned char *data);

#endif // SOCKET_H
