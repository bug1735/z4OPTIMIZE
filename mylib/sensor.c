#include "sensor.h"

void SensorInit(void)
{
    RCC_APB2PeriphClockCmd(SENSORAPBADCx, ENABLE);
	RCC_APB2PeriphClockCmd(SENSORAPBGPIOx, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = SENSORGPIOPIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SENSORGPIOx, &GPIO_InitStructure);
	
	ADC_RegularChannelConfig(SENSORADCx, SENSORADCCHANNEL, 1, ADC_SampleTime_55Cycles5);
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(SENSORADCx, &ADC_InitStructure);
	
	ADC_Cmd(SENSORADCx, ENABLE);
	
	ADC_ResetCalibration(SENSORADCx);
	while (ADC_GetResetCalibrationStatus(SENSORADCx) == SET);
	ADC_StartCalibration(SENSORADCx);
	while (ADC_GetCalibrationStatus(SENSORADCx) == SET);

};

uint16_t SensorGetValue(void)
{
    ADC_SoftwareStartConvCmd(SENSORADCx, ENABLE);
	while (ADC_GetFlagStatus(SENSORADCx, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(SENSORADCx);
};
