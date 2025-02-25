#include "W25Q64.h"
#include "W25Q64_Ins.h"
/*
* value is SET or RESET
*/
void SpiChose(uint8_t BitValue)
{
    GPIO_WriteBit(SPIGPIOx,SPINSS,(BitAction)BitValue);
}

/*
*hardware init with spi2
*
*/
void SpiInit(void)
{

    SPIGPIOCMD(SPIAPBGPIOx, ENABLE);
	SPISPICMD(SPIAPBSPIx, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = SPINSS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPIGPIOx, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = SPIMOSI | SPISCK;
	GPIO_Init(SPIGPIOx, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = SPIMISO;
	GPIO_Init(SPIGPIOx, &GPIO_InitStructure);
	
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPISPIx, &SPI_InitStructure);
	
	SPI_Cmd(SPISPIx, ENABLE);

    SpiChose(1);
}

uint8_t SpiSwapByte(uint8_t Data)
{
    while(SPI_I2S_GetFlagStatus(SPISPIx,SPI_I2S_FLAG_TXE) != SET);

    SPI_I2S_SendData(SPISPIx, Data);
	
	while (SPI_I2S_GetFlagStatus(SPISPIx, SPI_I2S_FLAG_RXNE) != SET);
	
	return SPI_I2S_ReceiveData(SPISPIx);
};



void W25Q64ReadID(uint8_t * MID,uint16_t *DID)
{
    SpiChose(0);
    SpiSwapByte(W25Q64_JEDEC_ID);
	*MID = SpiSwapByte(W25Q64_DUMMY_BYTE);
	*DID = SpiSwapByte(W25Q64_DUMMY_BYTE);
	*DID <<= 8;
	*DID |= SpiSwapByte(W25Q64_DUMMY_BYTE);
    SpiChose(1);
};

void W25Q64WriteEnable(void)
{
	SpiChose(0);
	SpiSwapByte(W25Q64_WRITE_ENABLE);
	SpiChose(1);
}

void W25Q64WaitBusy(void)
{
	uint32_t Timeout;
	SpiChose(0);
	SpiSwapByte(W25Q64_READ_STATUS_REGISTER_1);
	Timeout = 100000;
	while ((SpiSwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;
		}
	}
    SpiChose(1);
}

void W25Q64PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
    uint16_t i;
	
	W25Q64WriteEnable();
	
	SpiChose(0);
	SpiSwapByte(W25Q64_PAGE_PROGRAM);
	SpiSwapByte(Address >> 16);
	SpiSwapByte(Address >> 8);
	SpiSwapByte(Address);
	for (i = 0; i < Count; i ++)
	{
		SpiSwapByte(DataArray[i]);
	}
	SpiChose(1);
	
	W25Q64WaitBusy();
};

void W25Q64SectorErase(uint32_t Address)
{
    W25Q64WriteEnable();
	
	SpiChose(0);
	SpiSwapByte(W25Q64_SECTOR_ERASE_4KB);
	SpiSwapByte(Address >> 16);
	SpiSwapByte(Address >> 8);
	SpiSwapByte(Address);
	SpiChose(1);
	
	W25Q64WaitBusy();
};

void W25Q64ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
   	uint32_t i;
	SpiChose(0);
	SpiSwapByte(W25Q64_READ_DATA);
	SpiSwapByte(Address >> 16);
	SpiSwapByte(Address >> 8);
	SpiSwapByte(Address);
	for (i = 0; i < Count; i ++)
	{
		DataArray[i] = SpiSwapByte(W25Q64_DUMMY_BYTE);
	}
	SpiChose(1);    
};


void MyWrite(uint32_t viraddr,uint8_t * buf, uint16_t count){
	uint32_t addr = W_ADDR_CUL(viraddr);
	W25Q64PageProgram(addr,buf,count);
}
#include <stdlib.h>
uint8_t* MyRead(uint32_t viraddr,uint32_t count){
	uint32_t addr = W_ADDR_CUL(viraddr);
	uint8_t * data = (uint8_t *)malloc(8*count);
	W25Q64ReadData(addr,data,count);
	return data;
}
