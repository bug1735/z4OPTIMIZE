#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdint.h>        
#include <stdlib.h>
typedef uint8_t QUEUE_DATA;

typedef struct QueueNode{
    QUEUE_DATA data; //Êı¾İÀàĞÍ
    struct QueueNode * next;  //Ö¸ÏòÏÂÒ»¸ö
}QNode;

typedef struct QueueStack{
    QNode* phead;
    QNode* ptail;
    int8_t size;  //¼ÇÂ¼¸öÊı
}Queue;


//init 
void CreatQueue(struct QueueStack ** stack);

//insert
void QueuePush(struct QueueStack * stack,QUEUE_DATA);
//pop
QUEUE_DATA QueuePop(struct QueueStack * stack);


//**************************************************
//ä½¿ç”¨æ•°ç»„ä½œä¸ºbuffer
extern uint8_t * buffer;
#define BUFSIZE  256

#define BUFFERINIT()	buffer = (uint8_t *)malloc(BUFSIZE);
extern uint8_t WriteIndex; //å½“å‰bufferå†™å…¥çš„ä½ç½®
extern uint8_t ReadIndex; //å½“å‰bufferè¯»å–çš„ä½ç½®

//write to buffer
void WriteBuf(uint8_t *data,uint8_t count);

//read buffer
void ReadBuf(uint8_t *data,uint8_t count);
uint8_t ReadBufOne(void);
#endif
