#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdint.h>        
#include <stdlib.h>
typedef uint8_t QUEUE_DATA;

typedef struct QueueNode{
    QUEUE_DATA data; //��������
    struct QueueNode * next;  //ָ����һ��
}QNode;

typedef struct QueueStack{
    QNode* phead;
    QNode* ptail;
    int8_t size;  //��¼����
}Queue;


//init 
void CreatQueue(struct QueueStack ** stack);

//insert
void QueuePush(struct QueueStack * stack,QUEUE_DATA);
//pop
QUEUE_DATA QueuePop(struct QueueStack * stack);


//**************************************************
//使用数组作为buffer
extern uint8_t * buffer;
#define BUFSIZE  256

#define BUFFERINIT()	buffer = (uint8_t *)malloc(BUFSIZE);
extern uint8_t WriteIndex; //当前buffer写入的位置
extern uint8_t ReadIndex; //当前buffer读取的位置

//write to buffer
void WriteBuf(uint8_t *data,uint8_t count);

//read buffer
void ReadBuf(uint8_t *data,uint8_t count);
uint8_t ReadBufOne(void);
#endif
