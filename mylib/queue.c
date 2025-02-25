#include "queue.h"

void CreatQueue(struct QueueStack ** stack)
{
    *stack = (Queue*)malloc(sizeof(Queue));
    (*stack)->phead = NULL;
    (*stack)->ptail = NULL;
    (*stack)->size = 0;
}


//insert
void QueuePush(struct QueueStack * stack,QUEUE_DATA data){
    if(stack->size == 0){
        stack->phead =  (QNode*)malloc(sizeof(QNode));
        stack->phead->data = data;
        stack->ptail = stack->phead;
        stack->size = stack->size+1;
    }else if(stack->size <= 20){
        stack->ptail->next = (QNode*)malloc(sizeof(QNode));
        stack->ptail->next->data = data;
        stack->ptail = stack->ptail->next;
        stack->size = stack->size+1;
    }else{
		return;
	}
}

//pop
QUEUE_DATA QueuePop(struct QueueStack * stack)
{
    struct QueueNode * node;
    QUEUE_DATA data;
    if(stack->phead != NULL){
        node = stack->phead->next;
        data = stack->phead->data;
        free(stack->phead);
        stack->phead = node;
		stack->size = stack->size - 1;
        return data;
    }else{
        return 0;
    }
} 

//**************************************************
//使用数组作为buffer
uint8_t * buffer;
uint8_t WriteIndex = 0; //当前buffer写入的位置
uint8_t ReadIndex = 0; //当前buffer读取的位置
//write to buffer
void WriteBuf(uint8_t *data,uint8_t count){
	for(uint16_t i = 0;i<=count;i++){
		buffer[WriteIndex++]= data[i];
	}	
}

//read buffer
void ReadBuf(uint8_t *data,uint8_t count){
	for(uint16_t i= 0;i <= count;i++){
		data[i] = buffer[ReadIndex++];
	}
}

uint8_t ReadBufOne(){
	return buffer[ReadIndex++];
};
