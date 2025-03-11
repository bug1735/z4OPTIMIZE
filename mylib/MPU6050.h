#ifndef __MPU6050_H
#define __MPU6050_H
/*
	This is a drive to get information about accelerater and gyroscope from MPU6050
	Using IIC software communication protocol ,using software IIC
PORT：
	GPIOB 6 as SCL
	GPIOB 7 as SDA
	GPIOA 8 as interuput
*/

#define 	MPU6050_ADDRESS		(uint8_t)0xD0

/*
**	MPU6050 write and read
*/
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);


/*
**	motion detection interuput receive port choose GPIOA 8 
**	you can view interuput function in "inter.h"
*/
void MPUInteruput_Init(void);

void MPU6050_Init(void);  //init MPU6050 but no others function, include IICinit
void MPUInit(void); //using software IIC and open motion detection, include IICinit

/*
**	MPU base encapulated function
*/
uint8_t MPU6050_GetID(void);
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);



#endif
