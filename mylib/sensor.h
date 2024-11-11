#ifndef __SENSOR_H
#define __SENSOR_H

#include "stm32f10x.h"

#define SENSORADCx  ADC1
#define SENSORGPIOPIN   GPIO_Pin_0
#define SENSORGPIOx     GPIOA
#define SENSORADCCHANNEL ADC_Channel_3
#define SENSORAPBADCx   RCC_APB2Periph_ADC1
#define SENSORAPBGPIOx  RCC_APB2Periph_GPIOA

void SensorInit(void);
uint16_t SensorGetValue(void);

#endif