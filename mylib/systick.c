#include "systick.h"



void SysTickInit(uint32_t time){
	SYSTIM->LOAD = 720000 ;				//设置定时器重装值
	SYSTIM->VAL = 0x00;					//清空当前计数值
	SYSTIM->CTRL = 0x00000007;				//设置时钟源为HCLK，启动定时器
	SCB->SHP[11] = 0x00;
	//NVIC_SetPriority(SysTick_IRQn, 0);
	
	//while(!(SYSTIM->CTRL & 0x00010000));	//等待计数到0
	//SYSTIM->CTRL = 0x00000004;
}

/*
	*timer
*/
void Delay_us(uint32_t xus)
{
	SYSTIM->LOAD = 120 * xus;				//设置定时器重装值
	SYSTIM->VAL = 0x00;					//清空当前计数值
	SYSTIM->CTRL = 0x00000005;				//设置时钟源为HCLK，启动定时器
	while(!(SYSTIM->CTRL & 0x00010000));	//等待计数到0
	SYSTIM->CTRL = 0x00000004;				//关闭定时器
}


void DelayUs(uint32_t time)
{
	time+= SysTime;
	while(SysTime < time);
};


//该函数需要引入全局变量
void DateCalculate(void)
{
    int16_t day = ClockTime/(24*60*60);
    
    //four years multiple
    int16_t year = 4 * (day / (365*3+366));
    int16_t year_surday = day % (365*3 + 366);

    if(year_surday < 366){
    }else if(year_surday < 366+365){
        year+=1;
        year_surday-=(366);
    }else if(year_surday < 366+365*2){
        year+=2;
        year_surday-=(366+365);
    }else{
        year+=3;
        year_surday-=(366+365*2);
    }


    
    DateYear += year;

    if(year/4 == 0){
        if(31 >= year_surday && year_surday> 0){
            DateMonth= 1;
        }else if(60 >= year_surday && year_surday > 31){
            DateMonth =2 ;
            year_surday-= 31;
        }else if(91 >= year_surday && year_surday  > 60){
            DateMonth =3;
            year_surday-= 60;
        }else if(121>= year_surday && year_surday  > 91){
            DateMonth =4 ;
            year_surday-= 91;
        }else if(152>= year_surday && year_surday  > 121){
            DateMonth =5 ;
            year_surday-= 121;
        }else if(182>= year_surday && year_surday  > 152){
            DateMonth =6 ;
            year_surday-= 152;
        }else if(213>= year_surday && year_surday  > 182){
            DateMonth =7 ;
            year_surday-= 182;
        }else if(244>= year_surday && year_surday > 213){
            DateMonth =8 ;
            year_surday-= 213;
        }else if(274>= year_surday && year_surday  > 244){
            DateMonth =9 ;
            year_surday-= 244;
        }else if(305>= year_surday && year_surday  > 274){
            DateMonth =10 ;
            year_surday-= 274;
        }else if(335>= year_surday && year_surday  > 305){
            DateMonth =11 ;
            year_surday-= 305;
        }else if(366>= year_surday && year_surday  > 335){
            DateMonth =12 ;
            year_surday-= 335;
        }
    }else{
        if(31 >= year_surday > 0){
            DateMonth= 1;
        }else if(59 >= year_surday && year_surday  > 31){
            DateMonth =2 ;
            year_surday-= 31;
        }else if(90 >= year_surday && year_surday  > 59){
            DateMonth =3;
            year_surday-= 59;
        }else if(120>= year_surday && year_surday  > 90){
            DateMonth =4 ;
            year_surday-= 90;
        }else if(151>= year_surday && year_surday  > 120){
            DateMonth =5 ;
            year_surday-= 120;
        }else if(181>= year_surday && year_surday  > 151){
            DateMonth =6 ;
            year_surday-= 151;
        }else if(212>= year_surday && year_surday  > 181){
            DateMonth =7 ;
            year_surday-= 181;
        }else if(243>= year_surday && year_surday  > 212){
            DateMonth =8 ;
            year_surday-= 212;
        }else if(273>= year_surday && year_surday  > 243){
            DateMonth =9 ;
            year_surday-= 243;
        }else if(304>= year_surday && year_surday  > 273){
            DateMonth =10 ;
            year_surday-= 273;
        }else if(334>= year_surday && year_surday  > 304){
            DateMonth =11 ;
            year_surday-= 304;
        }else if(365>= year_surday && year_surday  > 334){
            DateMonth =12 ;
            year_surday-= 334;
        } 
    }
    DateDay = year_surday;    
}
