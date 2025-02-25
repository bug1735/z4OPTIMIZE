																																																																																																																	#ifndef __MAINWINDOW_H
#define __MAINWINDOW_H

#ifdef LIB
#include "lib.h"
#else
#include "stm32f10x.h"
#endif

#include "oled.h"

extern uint8_t status;
extern uint8_t state;  //the state of input

extern uint32_t SysTime;
extern int32_t ClockTime;
extern int16_t DateYear;
extern int8_t DateDay;
extern int8_t DateMonth;
extern int8_t DateWeek;
extern int8_t Weather;

#define CLOCKRUN 	0
#define APPCHOOSE	1
#define TIMERRUN	2
#define COUNTRUN	3
#define STEPRUN		4
#define MP3RUN		5
#define STORAGE		6	

//app 的个数，从2开始  (**如果要增加程序，只用改APPEND，并且添加appstr[])
//并且在appshow中添加app相关程序
#define 	APPSTART	2
#define		APPEND	6 

//according to status and ensure to choose app
//app show window ,status = 1
void AppShow(void);
//mainwindow is a clock ,status = 0
void ClockShow(void);

//app choose window ,status = 1
void AppChoose(void);
//app1 is 计时器status = 1 ,ensure = 1 
void Timer(void); 
//app2 is 倒计时闹钟status = 2 ,ensure = 1 
void CountdownTimer(void);
//app3 is 显示步数status = 3 ,ensure = 1 
void StepNum(void);
//app4 is 手机音乐切换器status = 4 ,ensure = 1 
void Mp3Switch(void);
//app5 is storage
void Storage(void);

#endif
