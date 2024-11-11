#include "keyborad.h"

#ifdef LIB
/*
	*使用GPIOA的0，2,4，接口进行按键输入
	*2为向上，4为向下，0为返回退出
*/
void KeyBoradInit(void)
{
	RCC_EN_GPIOA;
	uint32_t tempreg = GPIOA->CRL;
	tempreg |= (uint32_t)0x00080808;
	GPIOA->CRL = tempreg;
	GPIOA->BSRR = (uint32_t)0x00000015;
	
	//初始化中断AFIO EXTI  NVIC 
	RCC_EN_AFIO;
	AFIO->EXTICR[0] = (uint32_t)0x00000000; 
	AFIO->EXTICR[1] = (uint32_t)0x00000000;

	//初始化EXTI（中断屏蔽寄存器）   
	EXTI->IMR |= (uint32_t)0x00000015;
	//EXTI->EMR |= (uint32_t)0x00000013;
	EXTI->RTSR |= (uint32_t)0x00000015;  //上升沿触发
	EXTI->FTSR |= (uint32_t)0x00000015;
	//初始化NVIC
	tempreg = SCB->AIRCR;
	
	tempreg |= (uint32_t)0x00000700;
	SCB->AIRCR = tempreg;
	NVIC->ISER[0] |= (uint32_t)0x00000540;
	
	NVIC->IP[1] |= (uint32_t)0x00000000;
	NVIC->IP[2]	|= (uint32_t)0x00200010;
	
};
#else
void KeyBoradInit2(void)
{
	//开启GPIOA0的输入中断
	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;          
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;            
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_4; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;          
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; // 设置优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; // 可以设置不同的子优先级
    NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; // 可以设置不同的子优先级
    NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; // 可以设置不同的子优先级
    NVIC_Init(&NVIC_InitStructure);
	
	
	EXTI_InitTypeDef EXTI_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // 配置 GPIOA Pin 0 连接到 EXTI Line 0
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 或 EXTI_Trigger_Falling / EXTI_Trigger_Rising_Falling
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;    
    EXTI_Init(&EXTI_InitStructure);
	
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;    
    EXTI_Init(&EXTI_InitStructure);
	
	// 配置 GPIOA Pin 4 连接到 EXTI Line 4
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_Init(&EXTI_InitStructure);

};
void GPIOA0_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//开启GPIOA0的输入中断
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;            // 选择引脚0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // 设置为浮空输入 (或其他合适的模式，如上拉/下拉)
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // 设置速度 (可选)
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;         // 选择EXTI Line 0中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 设置抢占优先级 (0为最高)
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      // 设置子优先级 (0为最高)
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 使能中断通道
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI_InitTypeDef EXTI_InitStructure;

// 连接GPIOA Pin 0 到 EXTI Line 0
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); 
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;             // 选择EXTI Line 0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;      // 设置为中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 设置触发方式 (上升沿、下降沿或双边沿)
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;               // 使能EXTI Line
	EXTI_Init(&EXTI_InitStructure);
}

#endif


