
#include "common.h"
#include "queue_dataitem.h"

void init(Queue_DataItem *);
int push_queue(Queue_DataItem *,DataItem);
int full_queue(Queue_DataItem *);
int pop_queue(Queue_DataItem *,DataItem *);
int empty_queue(Queue_DataItem *);
int length(Queue_DataItem *pQ);

/*
void init(Queue_DataItem *pQ)
{
    pQ->pBase=(int *)malloc(sizeof(DataItem)*100);
    pQ->front=0;
    pQ->rear=0;
}
int full_queue(Queue_DataItem *pQ)
{
    if((pQ->rear+1)%5==pQ->front)
        return 1;
    else
        return 0;
}
int push_queue(Queue_DataItem *pQ,DataItem item)
{
    if(full_queue(pQ))
    {
        printf("队列已满，入队失败！\n");
        return 1;
    }
    pQ->pBase[pQ->rear]=item;
    pQ->rear=(pQ->rear+1)%5;//队尾加1
    return 0;
}
int pop_queue(Queue_DataItem *pQ,DataItem *pVal)
{
    if(empty_queue(pQ))
        return 1;
    *pVal=pQ->pBase[pQ->front];
    pQ->front=(pQ->front+1)%5;
    return 0;
}
int empty_queue(Queue_DataItem *pQ)
{
    if(pQ->rear==pQ->front)//因为队列不为空时，rear和front肯定不相等
        return 1;
    else
        return 0;
}
int traverse_queue(Queue_DataItem *pQ)
{
    int i=pQ->front;
    if(empty_queue(pQ))
    {
        return 1;
    }
    printf("队列元素有：");
    while(i!=pQ->rear)
    {
        printf("%d  ",pQ->pBase[i]);
        i=(i+1)%5;
    }
    printf("\n");
    return 0;
}
int length(Queue_DataItem *pQ)
{
    int len=0;
    int i=pQ->front;;
    if(empty_queue(pQ))
        return 0;//队列为空，长度为0
    while(i!=pQ->rear)
    {
        i=(i+1)%5;
        ++len;
    }
    return len;
}
*/
