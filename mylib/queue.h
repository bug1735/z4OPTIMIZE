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


//**************************************************
//浣跨敤鏁扮粍浣滀负buffer
extern uint8_t * buffer;
#define BUFSIZE  256

#define BUFFERINIT()	buffer = (uint8_t *)malloc(BUFSIZE);
extern uint8_t WriteIndex; //褰撳墠buffer鍐欏叆鐨勪綅缃�
extern uint8_t ReadIndex; //褰撳墠buffer璇诲彇鐨勪綅缃�

//write to buffer
void WriteBuf(uint8_t *data,uint8_t count);

//read buffer
void ReadBuf(uint8_t *data,uint8_t count);
uint8_t ReadBufOne(void);
#endif
