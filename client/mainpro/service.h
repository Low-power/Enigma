#ifndef SERVICE_H
#define SERVICE_H
#include "common.h"

typedef enum _RunType{RUNAS,MAPPING} RunType;

extern int createProcess(char *szFileName,RunType type);

#endif // SERVICE_H
