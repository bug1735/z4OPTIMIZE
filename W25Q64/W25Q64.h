#ifndef __W25Q64_H
#define __W25Q64_H
#include "stm32f10x.h"
#define SPIGPIOCMD	   RCC_APB2PeriphClockCmd
#define SPISPICMD	   RCC_APB1PeriphClockCmd
#define SPIAPBGPIOx    RCC_APB2Periph_GPIOB
#define SPIAPBSPIx     RCC_APB1Periph_SPI2
#define SPIGPIOx       GPIOB
#define SPISPIx        SPI2
#define SPINSS      GPIO_Pin_12
#define SPISCK      GPIO_Pin_13
#define SPIMISO     GPIO_Pin_14
#define SPIMOSI     GPIO_Pin_15

void SpiChose(uint8_t BitValue);
void SpiInit(void);
uint8_t SpiSwapByte(uint8_t Data);
void W25Q64ReadID(uint8_t * MID,uint16_t *DID);
void W25Q64WriteEnable(void);
void W25Q64WaitBusy(void);
void W25Q64PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count);
void W25Q64SectorErase(uint32_t Address);
void W25Q64ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count);
#endif
