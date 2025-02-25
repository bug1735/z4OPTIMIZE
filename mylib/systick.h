#ifndef __SYSTICK_H
#define __SYSTICK_H

#ifdef LIB
#include "lib.h"
#else
#include "stm32f10x.h"
#endif

#include "stdint.h"

extern int32_t ClockTime;
extern uint32_t SysTime;
extern int16_t DateYear;
extern int8_t DateDay;
extern int8_t DateMonth;
extern int8_t DateWeek;

//system timer
#define SYSTIM_BASE		(uint32_t)0xE000E010
typedef struct {
	volatile uint32_t CTRL;
	volatile uint32_t LOAD;
	volatile uint32_t VAL;
	volatile uint32_t CALIB;
}SYSTIM_Typedef;

#define SYSTIM	((SYSTIM_Typedef*)SYSTIM_BASE)

void SysTickInit(uint32_t time);

void Delay_us(uint32_t xus);

void DelayUs(uint32_t time);

//通过时钟ClockTime计算日期
void DateCalculate(void);
#endif
