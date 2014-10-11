
#include <string.h>
#define XQUEUE_MAXSIZE 1024
/*
typedef struct _XQueue
{
    unsigned char *pBase;
    int front;
    int rear;
    unsigned short cellSize;
}XQueue;

int xque_isEmpty(XQueue *pQ)
{
    if(pQ->rear==pQ->front)
        return 1;
    else
        return 0;
}
void init(XQueue *pQ,unsigned short cellSize)
{
    pQ->pBase=malloc(cellSize*XQUEUE_MAXSIZE);
    pQ->front=0;
    pQ->rear=0;
    pQ->cellSize=cellSize;
}
int xque_isFull(XQueue *pQ)
{
    if((pQ->rear+1)%XQUEUE_MAXSIZE==pQ->front)
        return 1;
    else
        return 0;
}
int xque_push(XQueue *pQ,void* pVal)
{
    if(xque_isFull(pQ))
    {
        return 1;
    }
    memcpy(pQ->pBase+pQ->rear*pQ->cellSize,pVal,pQ->cellSize);
    pQ->rear=(pQ->rear+1)%XQUEUE_MAXSIZE;
    return 0;
}
int xque_pop(XQueue *pQ,void *pVal)
{
    if(xque_isEmpty(pQ))
        return 1;
    if(pVal!=NULL) memcpy(pVal,pQ->pBase+pQ->front*pQ->cellSize,pQ->cellSize);
    pQ->front=(pQ->front+1)%XQUEUE_MAXSIZE;
    return 0;
}


int xque_length(XQueue *pQ)
{
    int len=0;
    int i=pQ->front;;
    if(xque_isEmpty(pQ))
        return 0;
    while(i!=pQ->rear)
    {
        i=(i+1)%XQUEUE_MAXSIZE;
        ++len;
    }
    return len;
}
*/
