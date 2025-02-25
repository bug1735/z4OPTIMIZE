#include "usart.h"

#include "dma.h"

/*
* GPIOA 8 = usart_CK
		9 = usart_TX
		10= usart_RX
		11= usart_CTS
		12= usart_RTS
*/
void UsartInit()
{
	JDY_USART_APBxClkCmd(JDY_USART_CLK,ENABLE);
	JDY_USART_GPIO_APBxClkCmd(JDY_USART_GPIO_CLK,ENABLE);
	
	//将usart的TX设置为推挽复用模式
	GPIO_InitTypeDef	GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  JDY_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(JDY_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	//将usaet的RX设置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin =  JDY_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(JDY_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	USART_InitTypeDef	USART_InitStructure;
	USART_InitStructure.USART_BaudRate = JDY_USART_BAUDRATE;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(JDY_USARTx,&USART_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = JDY_USART_IRQ; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(JDY_USARTx,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(JDY_USARTx,ENABLE);
	
}

void UsartInit_DMA(void)
{
	JDY_USART_APBxClkCmd(JDY_USART_CLK,ENABLE);
	JDY_USART_GPIO_APBxClkCmd(JDY_USART_GPIO_CLK,ENABLE);
	
	//将usart的TX设置为推挽复用模式
	GPIO_InitTypeDef	GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  JDY_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(JDY_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	//将usaet的RX设置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin =  JDY_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(JDY_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	USART_InitTypeDef	USART_InitStructure;
	USART_InitStructure.USART_BaudRate = JDY_USART_BAUDRATE;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(JDY_USARTx,&USART_InitStructure);
	
	//enable DMA translate
	USART_DMACmd(JDY_USARTx,USART_DMAReq_Rx,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = JDY_USART_IRQ; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(JDY_USARTx,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(JDY_USARTx,ENABLE);
	
	DmaInit();
	
}

struct QueueStack  *JdyStack;
void JdyInit(void)
{
	UsartInit();
	CreatQueue(&JdyStack);
}

void UsartSendByte(uint8_t ch)
{
    USART_SendData(JDY_USARTx,ch);
    while (USART_GetFlagStatus(JDY_USARTx, USART_FLAG_TXE) == RESET);
}

void UsartSendBytes(uint8_t len, uint8_t *data)
{
	for(uint8_t i = 0; i < len; i++){
		USART_SendData(JDY_USARTx,*data++);
		while(USART_GetFlagStatus(JDY_USARTx,USART_FLAG_TXE) == RESET);
	}	

	while(USART_GetFlagStatus(JDY_USARTx,USART_FLAG_TC) == RESET);
}

void UsartSendString(char * str)
{
	uint8_t k = 0;
	 do {
        UsartSendByte(  *(str + k) );
        k++;
    } while (*(str + k)!='\0');

    while (USART_GetFlagStatus(JDY_USARTx,USART_FLAG_TC)==RESET) {
    }
}
uint8_t i = 5;
uint8_t sta = 0;
#include "oled.h"
void JDY_USART_IRQHandler(void)
{	
	//在主程序中对DMA进行初始化
	//设置当前的缓冲计数
	//打开DMA进行传输
	//等待SPI发送完成
	//
	
	
	if(USART_GetITStatus(JDY_USARTx,USART_IT_RXNE) != RESET){
		uint8_t b = USART_ReceiveData(JDY_USARTx);				
		UsartSendByte(b);
		WriteBuf(&b,1);
		//QueuePush(JdyStack,b);
	}
	if(USART_GetFlagStatus(JDY_USARTx,USART_FLAG_RXNE) == SET)
	{
		OledShowString(4,12,"ok");
	}
}
	



