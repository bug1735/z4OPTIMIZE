#ifndef __IIC_H
#define __IIC_H


#include "stm32f10x.h" // Include the appropriate header for your STM32 series

//this file is a IIC communicate portocol for MPU6050

// Define GPIO pins for SDA and SCL
#define MPUI2C_SDA_PORT GPIOB
#define MPUI2C_SDA_PIN  GPIO_Pin_7
#define MPUI2C_SCL_PORT GPIOB
#define MPUI2C_SCL_PIN  GPIO_Pin_6

// Timing delay (adjust based on your clock speed)

#define MPUI2C_DELAY() for (volatile int i = 0; i < 10; i++)
 


// Function prototypes
void MPUI2C_Init(void);
void MPUI2C_Start(void);
void MPUI2C_Stop(void);
void MPUI2C_CekAck(void);
void MPUI2C_Write(uint8_t data);
uint8_t MPUI2C_Read(uint8_t ack); //ack Representative the end and stop read
void MPUI2C_Ack(void);
void MPUI2C_Nack(void);
void MPUI2C_GPIO_Write(GPIO_TypeDef* port, uint16_t pin, uint8_t state);
uint8_t MPUI2C_GPIO_Read(GPIO_TypeDef* port, uint16_t pin);

#endif
