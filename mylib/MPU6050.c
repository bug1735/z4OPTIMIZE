#include "MPU6050.h"


/*
	using I2C1 GPIOB6 is SCL GPIOB7 is SDA 
*/
void HardWareIicInit()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef	GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; // Alternate Function Open Drain
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	I2C_InitTypeDef	I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C; // I2C mode
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; // 50% duty cycle (Standard mode)
	I2C_InitStructure.I2C_OwnAddress1 = 0x00; // Own address (not relevant for master mode)
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; // Enable acknowledge
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // 7-bit addressing
	I2C_InitStructure.I2C_ClockSpeed = 50000; // 100kHz clock speed
	I2C_Init(I2C1, &I2C_InitStructure);
	
	I2C_Cmd(I2C1,ENABLE);
}

void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;
		}
	}
}
#include "stm32f10x.h"
void MPUWrite(uint8_t RegAddress,uint8_t data)
{
	I2C_GenerateSTART(I2C1,ENABLE);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1,ADDRESS,I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

	I2C_SendData(I2C1,RegAddress);	
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING);

	I2C_SendData(I2C1,data);	
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	I2C_GenerateSTOP(I2C1,ENABLE);
}

void MPUInit()
{
	HardWareIicInit();
	
	MPUWrite(MPU6050_PWR_MGMT_1, 0x01);
	MPUWrite(MPU6050_PWR_MGMT_2, 0x00);
	MPUWrite(MPU6050_SMPLRT_DIV, 0x09);
	MPUWrite(MPU6050_CONFIG, 0x06);
	MPUWrite(MPU6050_GYRO_CONFIG, 0x18);
	MPUWrite(MPU6050_ACCEL_CONFIG, 0x18);
	
}

//debug
void MPUInitDebug()
{
	HardWareIicInit();
	uint8_t data = 0x01;
	MPUWriteBytes(0x68,MPU6050_PWR_MGMT_1,1,&data);
	data = 0x00;
	MPUWriteBytes(0x68,MPU6050_PWR_MGMT_2,1,&data);
	data = 0x09;
	MPUWriteBytes(0x68,MPU6050_SMPLRT_DIV,1,&data);
	data = 0x06;
	MPUWriteBytes(0x68,MPU6050_CONFIG,1,&data);
	data = 0x18;
	MPUWriteBytes(0x68,MPU6050_GYRO_CONFIG,1,&data);
	MPUWriteBytes(0x68,MPU6050_ACCEL_CONFIG,1,&data);
	
}

uint8_t MPUReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	I2C_GenerateSTART(I2C1, ENABLE);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, ADDRESS, I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C1, RegAddress);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTART(I2C1, ENABLE);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, ADDRESS, I2C_Direction_Receiver);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	I2C_GenerateSTOP(I2C1, ENABLE);
	
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED);
	Data = I2C_ReceiveData(I2C1);
	
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	
	return Data;
}

void MPUGetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	
	DataH = MPUReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPUReadReg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;
	
	DataH = MPUReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPUReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPUReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPUReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	DataH = MPUReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPUReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPUReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPUReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPUReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPUReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
}

/*using in MPU control DMP to write
*@param addr Is MPU6050's address
*@param teg Is the regester address that you want write data in
*@param len Is the length of the data array
*@param Is the data array first addresss
*@return 0 represent writing successfully
*/
uint8_t MPUWriteBytes(uint8_t addr, uint8_t reg, uint8_t len,uint8_t *buf)
{
	unsigned char i;
	addr = addr << 1;
	I2C_GenerateSTART(I2C1,ENABLE);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);

	I2C_Send7bitAddress(I2C1,addr,I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

	I2C_SendData(I2C1,reg);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

	for(i=0; i < len; i++){
		I2C_SendData(I2C1,*buf++);
		MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	}
	I2C_GenerateSTOP(I2C1,ENABLE);
	return 0;
}

/*using in MPU control DMP to read
*@param addr Is MPU6050's address
*@param reg Is the regester address that you want to read data
*@param len Is the length of the data array
*@param Is the data array first addresss
*@return 0 represent reading successfully
*/
#include "oled.h"
uint8_t MPUReadBytes(uint8_t addr, uint8_t reg, uint8_t len,uint8_t *buf)
{
	
	addr=addr<<1;

	I2C_GenerateSTART(I2C1,ENABLE);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);

	I2C_Send7bitAddress(I2C1,addr,I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C1,reg);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTART(I2C1,ENABLE);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1,addr,I2C_Direction_Receiver);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);

	
	while (len) {
		if (len == 1) {
		// 关闭ACK位，表示这是最后一个字节
		I2C_AcknowledgeConfig(I2C1, DISABLE);
		// 生成停止信号
		I2C_GenerateSTOP(I2C1, ENABLE);
		}
	
		// 等待数据接收完成
		MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED);

		*buf++ = I2C_ReceiveData(I2C1);
		len--;
	
		if (len > 0) {
		// 发送ACK
		I2C_AcknowledgeConfig(I2C1, ENABLE);
		}
	}
	
	return 0;
}

//debug
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	I2C_GenerateSTART(I2C1, ENABLE);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, 0xD0, I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C1, RegAddress);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTART(I2C1, ENABLE);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, 0xD0, I2C_Direction_Receiver);
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	I2C_GenerateSTOP(I2C1, ENABLE);
	
	MPU6050_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED);
	Data = I2C_ReceiveData(I2C1);
	
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	
	return Data;
}

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
}

