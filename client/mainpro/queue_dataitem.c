
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
        printf("�������������ʧ�ܣ�\n");
        return 1;
    }
    pQ->pBase[pQ->rear]=item;
    pQ->rear=(pQ->rear+1)%5;//��β��1
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
    if(pQ->rear==pQ->front)//��Ϊ���в�Ϊ��ʱ��rear��front�϶������
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
    printf("����Ԫ���У�");
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
        return 0;//����Ϊ�գ�����Ϊ0
    while(i!=pQ->rear)
    {
        i=(i+1)%5;
        ++len;
    }
    return len;
}
*/
