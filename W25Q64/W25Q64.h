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

//加入扇区和写入地址,31-21的11位为页偏移2K，0-11的12位为页内地址偏移位4K
//uint32_t W_ADDR = 0x00;
#define	W_ADDR_CUL(x) (((x>>20)&0x7FF)<<12)+(x&0xFFF)//通过写入寄存器计算写入的地址
#define W_ADDR_RET(x)	(x&0xFFF)+(((x>>12)&0x7FF)<<20)//由物理地址反推虚拟地址

//蓝牙的buffer地址，可以通过页表重定向到其他的flash内存
//在系统中使用256B内存作为buffer 20KB

void SpiChose(uint8_t BitValue);
void SpiInit(void);
uint8_t SpiSwapByte(uint8_t Data);
void W25Q64ReadID(uint8_t * MID,uint16_t *DID);
void W25Q64WriteEnable(void);
void W25Q64WaitBusy(void);
void W25Q64PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count);
void W25Q64SectorErase(uint32_t Address);
void W25Q64ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count);

void MyWrite(uint32_t viraddr,uint8_t * buf, uint16_t count);
uint8_t* MyRead(uint32_t viraddr,uint32_t count);

#endif

