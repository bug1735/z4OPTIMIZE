//using USART1 to drive jdy-31

#ifndef  __USART_H
#define	 __USART_H

#include "stm32f10x.h"
#include "oled.h"
#include "queue.h"
#include "W25Q64.h"
#include "W25Q64_Ins.h"

// 串口1-USART1  移植时修改
#define  JDY_USARTx                   USART1
#define  JDY_USART_CLK                RCC_APB2Periph_USART1
#define  JDY_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  JDY_USART_BAUDRATE           9600

// USART GPIO 引脚宏定义
#define  JDY_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  JDY_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd

#define  JDY_USART_TX_GPIO_PORT       GPIOA
#define  JDY_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  JDY_USART_RX_GPIO_PORT       GPIOA
#define  JDY_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  JDY_USART_IRQ                USART1_IRQn
#define  JDY_USART_IRQHandler         USART1_IRQHandler

//通过蓝牙链接后收到的数据存储在W25Q64中
//address 		data 
//0x000000(time)		uint32_t[31:0]  4Byte
//0x000003(date)		string "2024-11-9"	9Byte
//0x0000013(weather)	uint8_t 1Byte
//0x0000014(massage)	string

/**@defgroup BLUE_ADDR
   @{
*/
#define B_TIME_ADDR		0x00000000
#define B_DATE_ADDR		0x00000003
#define B_WEATHER_ADDR	0x0000000D
/** @} */

//usart hardware init
void UsartInit(void);

//jdy-31 init
void JdyInit(void);

void UsartSendByte( uint8_t ch);
void UsartSendBytes(uint8_t len,uint8_t *data);
void UsartSendString(char * str);
#endif
