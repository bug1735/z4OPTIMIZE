#ifndef _LIB_H
#define _LIB_H

#include <stdint.h>
#define __IO volatile


#define GPIOA_BASE  (uint32_t)0x40010800
#define PERIPH_BASE			  ((uint32_t)0x40000000)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x20000)
#define RCC_BASE              (AHBPERIPH_BASE + 0x1000)


#define FLASH_BASE			  ((uint32_t)0x08000000)
#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x20000)
#define FLASH_R_BASE          (AHBPERIPH_BASE + 0x2000) /*!< Flash registers base address */
#define FLASH               ((FLASH_TypeDef *) FLASH_R_BASE) //0x40022000

typedef struct{
	//CRL  **CNF **MODE
	volatile  uint32_t 	CRL;
	//CRH **CNF **MODE
	volatile  uint32_t 	CRH;
	//IDR 低16位 
	volatile  uint32_t 	IDR;
	//ODR 低16位
	volatile  uint32_t 	ODR;
	volatile  uint32_t	BSRR;
	volatile  uint32_t	BRR;
	volatile  uint32_t 	LCKR;
}GPIO_DEFINE;

typedef struct
{
  __IO uint32_t CR;  //4Byte
  __IO uint32_t CFGR;
  __IO uint32_t CIR;
  __IO uint32_t APB2RSTR;
  __IO uint32_t APB1RSTR;
  __IO uint32_t AHBENR;
//APB2ENR
  __IO uint32_t APB2ENR;
  __IO uint32_t APB1ENR;
  __IO uint32_t BDCR;
  __IO uint32_t CSR;  //40Byte

#ifdef STM32F10X_CL  
  __IO uint32_t AHBRSTR;
  __IO uint32_t CFGR2;
#endif /* STM32F10X_CL */ 

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)   
  uint32_t RESERVED0;
  __IO uint32_t CFGR2;
#endif /* STM32F10X_LD_VL || STM32F10X_MD_VL || STM32F10X_HD_VL */ 
} RCC_TypeDef;


#define GPIOA	((GPIO_DEFINE*)GPIOA_BASE)
#define GPIOB 	((GPIO_DEFINE*)(GPIOA_BASE + 0x400))
#define RCC                   ((RCC_TypeDef *) RCC_BASE)  // 0x4002 1000位于片上外设的位带区,end 0x4002 1140


//RCC cmd enable
#define RCC_EN_GPIOA  RCC->APB2ENR |= (uint32_t)0x00000004;
#define RCC_EN_GPIOB  RCC->APB2ENR |= (uint32_t)0x00000008;
#define RCC_EN_GPIOC  RCC->APB2ENR |= (uint32_t)0x00000010;
#define RCC_EN_AFIO   RCC->APB2ENR |= (uint32_t)0x00000001;
//GPIO CRL
#define GPIO_BASE  (uint32_t)0x40010800

//AFIO MAP
#define AFIO_BASE	(uint32_t)0x40010000

typedef struct{
	volatile uint32_t EVCR;
	volatile uint32_t MAPR;
	volatile uint32_t EXTICR1;
	volatile uint32_t EXTICR2;
	volatile uint32_t EXTICR3;
	volatile uint32_t EXTICR4;
}AFIO_Typdef;

#define AFIO	((AFIO_Typdef*)AFIO_BASE)

//EXTI  memory map
#define EXTI_BASE  (uint32_t)0x40010400

typedef struct{
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;
}EXTI_Typdef;

#define EXTI	((EXTI_Typdef*)EXTI_BASE)

//NVIC map
#define NVIC_BASE	(uint32_t)0xE000E100

typedef struct
{
  volatile uint32_t ISER[8];        
       uint32_t RESERVED0[24];  
  volatile uint32_t ICER[8];        
       uint32_t RSERVED1[24];   
  volatile uint32_t ISPR[8];        
       uint32_t RESERVED2[24];  
  volatile uint32_t ICPR[8];        
       uint32_t RESERVED3[24];  
  volatile uint32_t IABR[8];        
       uint32_t RESERVED4[56];  
  volatile uint8_t  IP[240];        
       uint32_t RESERVED5[644]; 
  volatile  uint32_t STIR;           
}NVIC_Typdef;   

#define NVIC	((NVIC_Typdef*)NVIC_BASE)

//SCB map
#define SCB_BASE	(uint32_t)0xE000ED00
typedef struct
{
  volatile uint32_t CPUID;                        /*!< Offset: 0x00  CPU ID Base Register                                  */
  volatile uint32_t ICSR;                         /*!< Offset: 0x04  Interrupt Control State Register                      */
  volatile uint32_t VTOR;                         /*!< Offset: 0x08  Vector Table Offset Register                          */
  volatile uint32_t AIRCR;                        /*!< Offset: 0x0C  Application Interrupt / Reset Control Register        */
  volatile uint32_t SCR;                          /*!< Offset: 0x10  System Control Register                               */
  volatile uint32_t CCR;                          /*!< Offset: 0x14  Configuration Control Register                        */
  volatile uint8_t  SHP[12];                      /*!< Offset: 0x18  System Handlers Priority Registers (4-7, 8-11, 12-15) */
  volatile uint32_t SHCSR;                        /*!< Offset: 0x24  System Handler Control and State Register             */
  volatile uint32_t CFSR;                         /*!< Offset: 0x28  Configurable Fault Status Register                    */
  volatile uint32_t HFSR;                         /*!< Offset: 0x2C  Hard Fault Status Register                            */
  volatile uint32_t DFSR;                         /*!< Offset: 0x30  Debug Fault Status Register                           */
  volatile uint32_t MMFAR;                        /*!< Offset: 0x34  Mem Manage Address Register                           */
  volatile uint32_t BFAR;                         /*!< Offset: 0x38  Bus Fault Address Register                            */
  volatile uint32_t AFSR;                         /*!< Offset: 0x3C  Auxiliary Fault Status Register                       */
  volatile uint32_t PFR[2];                       /*!< Offset: 0x40  Processor Feature Register                            */
  volatile uint32_t DFR;                          /*!< Offset: 0x48  Debug Feature Register                                */
  volatile uint32_t ADR;                          /*!< Offset: 0x4C  Auxiliary Feature Register                            */
  volatile uint32_t MMFR[4];                      /*!< Offset: 0x50  Memory Model Feature Register                         */
  volatile uint32_t ISAR[5];                      /*!< Offset: 0x60  ISA Feature Register                                  */
} SCB_Typdef;
#define SCB		((SCB_Typdef*)SCB_BASE)
#endif


	//对RCC->CR   
	// 1111 11 1PLLRDY 0PLLON 1111 0CSSON 1HSEBYP 1HSERDY 0HSEON 11111111HSICAL 11111HSITRIM 1保留  1HSIRDY 1HSION
	// 1111 11 1       1      1111 1      0       1       1      11111111       11111        1      1       1
	//							   1      1       1       0 
	//RCC->CFGR
	// 11111save 111MCO 1save 0USBPRE 0000PLLMUL 0PLLXTPRE 0PLLSRC 11ADCPRE 111PPRE2 111PPRE! 1111HPRE 11SWS 11SW
	// *****save ***MCO *save *USBPRE ****PLLMUL *PLLXTPRE *PLLSRC **ADCPRE ***PPRE2 ***PPRE1 ****HPRE **SWS **SW
	//  							   111       0         1				000      000      0000
	
	/* Reset HSEON, CSSON and PLLON bits */
	//RCC->CR &= (uint32_t)0xFEF6FFFF;

	/* Reset HSEBYP bit */
	//RCC->CR &= (uint32_t)0xFFFBFFFF;
	
	/* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
	//RCC->CFGR &= (uint32_t)0xF0FF0000;  
/*高16位
			1111 0 保留，始终读为0。
			000 没有时钟输出，微控制器时钟输出  100系统时钟(SYSCLK)输出
			1 保留位
			1 PLL时钟直接作为USB时钟 ,USB预分频 (USB prescaler) 
			11 11  PLL 16倍频输出,PLL倍频系数  
			1	HSE 2分频,HSE分频器作为PLL输入
			1 	PLL输入时钟源,HSE时钟作为PLL输入时钟
  低16位    00 	PCLK2 2分频后作为ADC时钟 ,ADC预分频 (ADC prescaler) 
			00 0/HCLK不分频,APB预分频(APB2) 
			000/HCLK不分频,低速APB预分频(APB1)  
			0000/SYSCLK不分频,AHB预分频 (AHB Prescaler)   
			00/HSi为系统时钟
			00/HSi为系统时钟，
*/
