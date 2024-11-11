#ifndef __MPU6050_H
#define __MPU6050_H
#include "stm32f10x.h"
#include <stdint.h>
#include "MPU6050_Reg.h"
/*
	This is a drive to get information about accelerater and gyroscope from MPU6050
	Using IIC communication protocol ,using hardware IIC
	Using standard lib
*/

#define 	ADDRESS		(uint8_t)0xD0

void HardWareIicInit(void);
void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
void MPUInit(void);
void MPUWrite(uint8_t RegAddress,uint8_t data);
uint8_t MPUReadReg(uint8_t RegAddress);
void MPUGetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);
uint8_t MPUWriteBytes(uint8_t addr, uint8_t reg, uint8_t len,uint8_t *buf);
uint8_t MPUReadBytes(uint8_t addr, uint8_t reg, uint8_t len,uint8_t *buf);

//debug
void MPUInitDebug(void);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);
#endif
