#include "oled.h"

#include "oled_font.h"

#define BLOCK		0xFF



void IicSendByte(uint8_t data){
	
	for(int i = 0; i < 8; i++){
		if( (data&(0x80 >> i)) == (0x80 >> i) )
		{
			IICSEND1;
		}else{
			IICSEND0;
		}
	}
	READREPLY;
}
/**
  * @brief  OLED send commend
  * @param  command 
  */
void OledWriteCommand(uint8_t Command)
{
	IICSTART;
	IicSendByte(0x78);		//从机地址
	IicSendByte(0x00);		//写命令
	IicSendByte(Command); 
	IICSTOP;
}
/**
  * @brief  OLED send data
  * @param  data 
  */
void OledWriteData(uint8_t data){
	IICSTART;
	IicSendByte(0x78);		//从机地址
	IicSendByte(0x40);		//写命令
	IicSendByte(data); 
	IICSTOP;
}

/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OledSetCursor(uint8_t Y, uint8_t X)
{
	OledWriteCommand(0xB0 | Y);					//设置Y位置
	OledWriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OledWriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}

void OledClear(void){
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OledSetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OledWriteData(0x00);
		}
	}
}
/*
	初始化I2C接口，使用GPIOA,GPIOAP6为SDA，GPIOAP2为SCL
*/
void Oled_Init(void){
	//enable GPIOA 
	RCC_EN_GPIOA;
	uint32_t tempreg = GPIOA->CRL;
	tempreg |= (uint32_t)0x33000000;
	GPIOA->CRL = tempreg;

	SETSCL;
	SETSDA;
	
	OledWriteCommand(0xAE);	//关闭显示
	
	OledWriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OledWriteCommand(0x80);
	
	OledWriteCommand(0xA8);	//设置多路复用率
	OledWriteCommand(0x3F);
	
	OledWriteCommand(0xD3);	//设置显示偏移
	OledWriteCommand(0x00);
	
	OledWriteCommand(0x40);	//设置显示开始行
	
	OledWriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OledWriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OledWriteCommand(0xDA);	//设置COM引脚硬件配置
	OledWriteCommand(0x12);
	
	OledWriteCommand(0x81);	//设置对比度控制
	OledWriteCommand(0xCF);

	OledWriteCommand(0xD9);	//设置预充电周期
	OledWriteCommand(0xF1);

	OledWriteCommand(0xDB);	//设置VCOMH取消选择级别
	OledWriteCommand(0x30);

	OledWriteCommand(0xA4);	//设置整个显示打开/关闭

	OledWriteCommand(0xA6);	//设置正常/倒转显示

	OledWriteCommand(0x8D);	//设置充电泵
	OledWriteCommand(0x14);

	OledWriteCommand(0xAF);	//开启显示
		
	OledClear();				//OLED清屏
}

/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OledShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OledSetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OledWriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OledSetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OledWriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}

void OledShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OledShowChar(Line, Column + i, String[i]);
	}
}

//显示相反的画面
void OledShowCharNo(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OledSetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OledWriteData(~OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OledSetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OledWriteData(~OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}

void OledShowStringNo(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OledShowCharNo(Line, Column + i, String[i]);
	}
}

uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}
void OledShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OledShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OledShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OledShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OledShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OledShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}


void OledShowNUM(uint8_t position,uint8_t num)
{
	for(uint8_t k = 2;k < 6;k++){
		OledSetCursor(k,position);
		for(uint8_t i = 0;i < 15; i++){
				OledWriteData(NUMLED[num][k-2][i]);	
		}
	}
}

void OledShowCOLON(uint8_t num)
{
	for(uint8_t k = 3;k < 5;k++){
		OledSetCursor(k,0x3E);
		for(uint8_t i = 0;i < 3; i++){
			OledWriteData(COLON[num][k-3][i]);
		}
	}
};
void OledShowNUMNo(uint8_t position,uint8_t num)
{
	for(uint8_t k = 2;k < 6;k++){
		OledSetCursor(k,position);
		for(uint8_t i = 0;i < 15; i++){
				OledWriteData(~NUMLED[num][k-2][i]);	
		}
	}
}

void OledShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
		uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OledShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OledShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
};

