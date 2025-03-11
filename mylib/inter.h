#ifndef __INTER_H
#define __INTER_H

#include "oled.h"

/* interuput priority grouping in keyborad.h 
**	the priority had been locked in group mode 2 bits for Preemption priority ,
**	2bits for subPriority
*/

extern uint8_t status;
extern uint8_t state;
extern uint32_t SysTime;
extern uint32_t SysTime2;
extern uint16_t stepnum;
#define ENSURE	0   
#define RETURN 	1
#define RIGHT	2
#define LEFT	3

#define STATEREAD(STATEBIT)	 (state >> STATEBIT)&0x01

#define DELAY		SysTime2 = SysTime; \
			while(SysTime < (SysTime2 + 4));
/*
**@param bit is the state bit
*/
uint8_t StateRead(uint8_t bit);

void StateClear(uint8_t bit);
//ensure
void EXTI0_IRQHandler(void);

//return
void EXTI1_IRQHandler(void);

//up
void EXTI2_IRQHandler(void);

//down
void EXTI4_IRQHandler(void);

//interuput for MPU6050 motion detection
void EXTI9_5_IRQHandler(void);


#endif 
