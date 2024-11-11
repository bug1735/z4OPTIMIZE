#include "systick.h"



void SysTickInit(uint32_t time){
	SYSTIM->LOAD = 720000 ;				//设置定时器重装值
	SYSTIM->VAL = 0x00;					//清空当前计数值
	SYSTIM->CTRL = 0x00000007;				//设置时钟源为HCLK，启动定时器
	SCB->SHP[11] = 0x00;
	//NVIC_SetPriority(SysTick_IRQn, 0);
	
	//while(!(SYSTIM->CTRL & 0x00010000));	//等待计数到0
	//SYSTIM->CTRL = 0x00000004;
}

/*
	*timer
*/
void Delay_us(uint32_t xus)
{
	SYSTIM->LOAD = 120 * xus;				//设置定时器重装值
	SYSTIM->VAL = 0x00;					//清空当前计数值
	SYSTIM->CTRL = 0x00000005;				//设置时钟源为HCLK，启动定时器
	while(!(SYSTIM->CTRL & 0x00010000));	//等待计数到0
	SYSTIM->CTRL = 0x00000004;				//关闭定时器
}

extern uint32_t SysTime;
void DelayUs(uint32_t time)
{
	time+= SysTime;
	while(SysTime < time);
};
