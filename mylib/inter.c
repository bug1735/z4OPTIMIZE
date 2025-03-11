#include "inter.h"

uint16_t stepnum = 0;

/*
**@param bit is the state bit
*/
uint8_t StateRead(uint8_t bit){
	return (state >> bit)&0x01;
}

void StateClear(uint8_t bit){
	state &= ~(0x01<<bit);
}


//ensure
void EXTI0_IRQHandler(void)
{
	state |= 0x01;
	EXTI_ClearITPendingBit(EXTI_Line0);
	
	//DELAY;
    //if (EXTI_GetITStatus(EXTI_Line0) != RESET) // 检查是否为EXTI Line 0触发
    //{		
	//	if(working == APPCHOOSE){
	//		switch(status){
	//			case 2:
	//				working = TIMERRUN;
	//				break;
	//			case 3:
	//				working = COUNTRUN;
	//				break;
	//			case 4:
	//				working = STEPRUN;
	//				break;
	//			case 5:
	//				working = MP3RUN;
	//				break;
	//			default:
	//				break;
	//		}
	//		
	//	}
    //    EXTI_ClearITPendingBit(EXTI_Line0); // 清除中断标志位
	//	DELAY;
	//}
	//if (EXTI_GetITStatus(EXTI_Line0) != RESET) // 检查是否为EXTI Line 0触发
    //{
	//	status = 0;
	//	working = CLOCKRUN;
	//	EXTI_ClearITPendingBit(EXTI_Line0);
	//}
}

//return
void EXTI1_IRQHandler(void){
	state |= 0x02;
	EXTI_ClearITPendingBit(EXTI_Line1);
}

//up
void EXTI2_IRQHandler(void){
	state |= 0x08;
	EXTI_ClearITPendingBit(EXTI_Line2);
	
	//DELAY;
	//if (EXTI_GetITStatus(EXTI_Line2) != RESET) // 检查是否为EXTI Line 0触发
    //{		
	//	if(working == CLOCKRUN){
	//		working = APPCHOOSE;
	//	}
	//	if(working == APPCHOOSE){
	//		if(status == 0){
	//			status = 5;
	//		}else{
	//			status--;
	//			if(status == 1){
	//				status =5;
	//			}
	//		}
	//	}
    //    EXTI_ClearITPendingBit(EXTI_Line2); // 清除中断标志位
	//	DELAY;
	//}

}

//down
void EXTI4_IRQHandler(void){
	state |= 0x04;
	EXTI_ClearITPendingBit(EXTI_Line4);
	//DELAY;
	//if (EXTI_GetITStatus(EXTI_Line4) != RESET) // 检查是否为EXTI Line 0触发
    //{		
	//	if(working == CLOCKRUN){
	//		working = APPCHOOSE;
	//		status = 1;
	//	}
	//	if(working == APPCHOOSE){
	//		status++;	
	//		if(status == 6){
	//			status = 2;
	//		}
	//	}
    //    EXTI_ClearITPendingBit(EXTI_Line4); // 清除中断标志位
	//	DELAY;
	//}
}

#include "MPU6050.h"
void EXTI9_5_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line8) != RESET)
    {
		// 检查MPU6050中断来源
		uint8_t int_status = MPU6050_ReadReg(0x3A);
		if (int_status & 0x40) { // 运动检测中断标志位
			stepnum++;	
		}
		// 清除MPU6050中断标志
		MPU6050_ReadReg(0x3A); // 读取INT_STATUS会自动清除中断
		// 清除EXTI中断标志
	

        EXTI_ClearITPendingBit(EXTI_Line8);
    }
}
