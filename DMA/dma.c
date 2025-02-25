#include "dma.h"
#include "stm32f10x.h"
/*初始化DMA
*在启用USART_init时进行初始化，对usart寄存器的数据进行转存，转存64位8bit
*/
void DmaInit()
{
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&SPI2->DR); //DMA外设基地址 設置爲W25Q64的發送寄存器
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//设定了外设数据宽度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址寄存器是否自增
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&USART1->DR);//DMA内存基地址 設置爲藍牙usart的接受寄存器
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//外设数据位宽
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;//内存地址寄存器是否递增
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//外设作为数据传输目的地还是来源
	DMA_InitStructure.DMA_BufferSize = 1;//DMA通道的缓存大小
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//设置can的工作模式
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//使能内存到内存传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//设置DMA通道的软件优先级
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE);
	
	NVIC_InitTypeDef	NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_Init(&NVIC_InitStructure);	
	
	DMA_Cmd(DMA1_Channel5, ENABLE);
};

void DmaTransfer(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1, 1);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);
}; 


//在通道5的数据转发完成后会生成中断，
void DMA1_Channel5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC5) == SET)
	{
		
	};
}
