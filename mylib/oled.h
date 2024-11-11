#ifndef __OLED_H
#define __OLED_H

#ifdef LIB
#include "lib.h"
#else
#include "stm32f10x.h"
#endif

//set SDA and SCL  bit
#define SETSDA	GPIOA->BSRR |= 0x00000040; 
#define SETSCL	GPIOA->BSRR |= 0x00000080;	
#define RESETSDA	GPIOA->BSRR |= 0x00400000;
#define RESETSCL	GPIOA->BSRR |= 0x00800000;

//set I2C sign
//#define IICSTART	RESETSCL SETSDA SETSCL RESETSDA;
//#define IICSTOP		RESETSCL RESETSDA SETSCL SETSDA; 
//#define IICSEND1	RESETSCL SETSDA	SETSCL;
//#define IICSEND0	RESETSCL SETSDA	SETSCL;
//#define READREPLY   RESETSCL SETSCL;


#define IICSTART	SETSDA SETSCL RESETSDA RESETSCL; 
#define IICSTOP		RESETSDA SETSCL SETSDA; 
#define IICSEND1	SETSDA	SETSCL RESETSCL;
#define IICSEND0	RESETSDA	SETSCL RESETSCL;
#define READREPLY   SETSCL RESETSCL;
/*
	向屏幕打印
*/

/*
	初始化I2C接口，使用GPIOA,GPIOAP0为SDA，GPIOAP2为SCL
*/
void Oled_Init(void);
void IicSendByte(uint8_t data);
void OledWriteCommand(uint8_t Command);
void OledWriteData(uint8_t data);
void OledClear(void);
void OledSetCursor(uint8_t Y, uint8_t X);
void OledClear(void);
void OledShowChar(uint8_t Line, uint8_t Column, char Char);
void OledShowString(uint8_t Line, uint8_t Column, char *String);
void OledShowStringNo(uint8_t Line, uint8_t Column, char *String);
void OledShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
//OLED show num 
#define NUM1	 	0x00
#define NUM2	 	0x20
#define NUM3	 	0x50
#define NUM4	 	0x70
/*
void OledShowNUM0(uint8_t position);
void OledShowNUM1(uint8_t position);
void OledShowNUM2(uint8_t position);
void OledShowNUM3(uint8_t position);
void OledShowNUM4(uint8_t position);
void OledShowNUM5(uint8_t position);
void OledShowNUM6(uint8_t position);
void OledShowNUM7(uint8_t position);
void OledShowNUM8(uint8_t position);
void OledShowNUM9(uint8_t position);
*/
void OledShowNUM(uint8_t position,uint8_t num);
void OledShowCOLON(uint8_t num);
void OledShowNUMNo(uint8_t position,uint8_t num);
void OledShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OledShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

#endif 
