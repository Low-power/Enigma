#ifndef QUEUE_DATAITEM_H
#define QUEUE_DATAITEM_H
typedef struct _DataItem
{
    unsigned int size;
    void * data;
}DataItem;
typedef struct Queue
{
    DataItem *pBase;
    int front;
    int rear;
}Queue_DataItem;


#endif // QUEUE_DATAITEM_H
