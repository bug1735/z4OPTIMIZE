#include <stdint.h> 

#ifdef LIB
#include "lib.h"
#else
#include "stm32f10x.h"
#endif

#include "systick.h"
#include "oled.h"
#include "keyborad.h"
#include "mainwindow.h"
#include "inter.h"
#include "mpu6050.h"
#include "usart.h"
#include "W25Q64.h"
#include "queue.h"

extern void SystemInit(void){
	/* Set HSION bit */
	RCC->CR |= (uint32_t)0x00000001;
	/* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
	RCC->CFGR &= (uint32_t)0xF0FF0000;
	/* Reset HSEON, CSSON and PLLON bits */
	RCC->CR &= (uint32_t)0xFEF6FFFF;
	
	/* Reset HSEBYP bit */
	RCC->CR &= (uint32_t)0xFFFBFFFF;
	
	/* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
	RCC->CFGR &= (uint32_t)0xFF80FFFF;
	
	/* Disable all interrupts and clear pending bits  */
	RCC->CIR = 0x009F0000;

}
//system status
uint8_t status = 0;
uint8_t state = 0;

//usart receive data
char USART_RECEIVE[12];
//system time
uint32_t SysTime;
int32_t ClockTime = 2592006; //计时时间，一年有 365天的总秒数为31536000，所以计算机的计时周期是 2147483647/31536000=68.1年
uint32_t SysTime2;
int32_t Clock2 = -86401; // 用于秒速过一天，更新date数据
int16_t DateYear = 2024;
int8_t DateDay = 1;
int8_t DateMonth = 1;
int8_t DateWeek = 1;
int8_t Weather = 0; //天气数据
void SysTick_Handler(void){
	SysTime++;
	SysTime2++;
	Clock2 = ClockTime;
	if(SysTime2 == 12){
		ClockTime ++;
		SysTime2 = 0;
	}
	if(ClockTime - Clock2 > 86400) DateCalculate();
}

int main(void)
{
	status = 6;
	Oled_Init();
	SpiInit();
	SysTickInit(0);
	KeyBoradInit2();
	MPUInit();
	status = 0;
	state = 0;
	JdyInit();
	DateCalculate();
	
	BUFFERINIT();
	
	//uint32_t phyaddr = 0x800000;
	//uint32_t viraddr = W_ADDR_RET(phyaddr);
	//OledShowHexNum(2,1,phyaddr,10);
	//OledShowHexNum(3,1,viraddr,10);
	//char * data = "name is myworte is ok";
	//W25Q64SectorErase(phyaddr);
	//MyWrite(viraddr,(uint8_t*)data,21);
	//uint8_t * data2 = MyRead(viraddr,21);
	//OledShowString(1,1,(char*)data2);
	while(1){	
		AppShow();
	};
	
}

#ifdef LIB
void EXTI0_IRQHandler(void){
	if((EXTI->PR & 0x00000001)==1){
		status++;
		EXTI->PR &= 0xFFFFFFFE;
	}
}
#else

#endif

