#ifndef HTTP_H
#define HTTP_H
int httpRead(const char *szURL,char *szBuffer,size_t size);
int resoveConnInfo(const char *connInfo,char *addr,unsigned short *port);
int initINet();

#endif // HTTP_H
