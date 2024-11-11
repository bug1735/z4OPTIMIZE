#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdint.h>        
#include <stdlib.h>
typedef uint8_t QUEUE_DATA;

typedef struct QueueNode{
    QUEUE_DATA data; //数据类型
    struct QueueNode * next;  //指向下一个
}QNode;

typedef struct QueueStack{
    QNode* phead;
    QNode* ptail;
    int8_t size;  //记录个数
}Queue;


//init 
void CreatQueue(struct QueueStack ** stack);

//insert
void QueuePush(struct QueueStack * stack,QUEUE_DATA);
//pop
QUEUE_DATA QueuePop(struct QueueStack * stack);
#endif
