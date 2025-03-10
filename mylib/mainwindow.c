#include "mainwindow.h"
#include "inter.h"
#include "MPU6050.h"
#include "inv_mpu.h"
/*
**	status=0 CLOCK
	status=1 APPCHOSE
	status=2 TIMER
	status=3 COUNTDOWN
	status=4 STEP
*/

//debug
#define DEBUGPRO		
#define OLEDSHOWNUM(NUM)		OledShowNUM(NUM4,NUM%10);	\
						OledShowNUM(NUM3,NUM%60/10);	\
						OledShowNUM(NUM2,NUM/60%10);	\
						OledShowNUM(NUM1,NUM/60/10);	

char appstr[5][16] = {"timer","counttimer","stepnum","music","sorage"};

#include "W25Q64.h"
#include "queue.h"
extern struct QueueStack  *JdyStack;//defined in uart.c
//*******************************************
//处理buffer用的api  
//TODO在串口接收到一个结束信号后就可以开启此中断
#define STARTSIGN (uint8_t)0xFF
#define ENDSIGN (uint8_t)0xFF
void dateDeal(void); //处理日期数据
void weatherDeal(void);//处理天气信息
void musicDeal(void); //处理音乐相关的信息 
void Buffer(void){
	uint8_t data;
	//数据段开始  使用链表实现的buffer
	/*if(QueuePop(JdyStack) == STARTSIGN){
		//数据段结束
		data = QueuePop(JdyStack);
		if(data == 'd'){
			//处理日期数据
			dateDeal();
		}else if(data == 't'){
			//处理天气情况
			weatherDeal();
		}else if(data == 'm'){
			//是音乐信息
		}else{
			//未定义的信息
		}	
	}
	*/
	
	//使用数组实现的链表
	while(ReadBufOne() == STARTSIGN);
	data = ReadBufOne();
	if(data == 'd'){
		//处理日期数据
		dateDeal();
	}else if(data == 't'){
		//处理天气情况
		weatherDeal();
	}else if(data == 'm'){
		//是音乐信息
	}else{
		//未定义的信息
	}
}
//*******************************************

/*
	桌面程序，用于选择当前执行的权限，在应用程序中�?
	返回和退出的过程没有操作系统，所以在设定程序的时候有程序返回
*/
void AppShow(void)
{
	state = 0;
	switch(status){
		case 0:
			ClockShow();
			break;
		case 1:
			AppChoose();
			break;
		case 2:
			Timer();
			break;
		case 3:
			CountdownTimer();
			break;
		case 4:
			StepNum();
			break;
		case 5:
			Mp3Switch();
			break;
		case 6:
			Storage();
			break;
		default:
			status = 0;
			break;
	}
	
};

	
void ClockShow(void)
{
	OledClear();
	uint8_t num = 0; //用于时钟冒号的闪烁延�?
	uint8_t time = 0;
	while(status == CLOCKRUN){	
		OledShowNum(1,1,DateYear,4);
		OledShowChar(1,5,'-');
		OledShowNum(1,6,DateMonth,2);
		OledShowChar(1,8,'-');
		OledShowNum(1,9,DateDay,2);
		OledShowNum(1,12,Weather,1);
		OledShowCOLON(num);		
		OledShowNUM(NUM4,ClockTime%10);	
		OledShowNUM(NUM3,ClockTime%60/10);
		OledShowNUM(NUM2,ClockTime/60%10);
		OledShowNUM(NUM1,ClockTime%3600/60/10);
		if(StateRead(LEFT) ==  0x01 || StateRead(RIGHT) == 0x01){
			status = 1;
			state = 0;
		}
		if(time > 8){
			num = 0;
			time=0;
			
		}else{
			num = 1;
			time++;
		};
	}
};

//app choose window ,status = 1
void AppChoose(void){
	OledClear();
	status = 2;
	while(status != 0){
		OledShowNum(4,6,status,2);
		if(StateRead(RIGHT) == 1){
			status ++;
			if(status == APPEND+1){
				status = APPSTART;
			}
			StateClear(RIGHT);
		}else if(StateRead(LEFT) == 1){
			if(status == APPSTART){
				status = APPEND;
			}else{
				status --;
			}
			StateClear(LEFT);
		}else if(StateRead(ENSURE) == 1){
			StateClear(ENSURE);
			return;
		}else if(StateRead(RETURN) == 1){
			StateClear(RETURN);
			status = CLOCKRUN;
			return;
		}
		
		if(status-2  < 4){
			for(uint8_t i = 0; i < 4; i++){
				if(i != status-2){
					OledShowString(i+1,1,appstr[i]);
				}else{
					OledShowStringNo(i+1,1,appstr[status-2]);
				}
			}
		}else{			
			OledShowString(1,1,appstr[status-5]);
			OledShowString(2,1,appstr[status-4]);
			OledShowString(3,1,appstr[status-3]);
			OledShowStringNo(4,1,appstr[status-2]);		
		}
	}
};
//app1 is 计时器status = 1 ,ensure = 1 
void Timer(void){
	OledClear();
	uint32_t timer = SysTime;
	uint8_t timerstate = 0;
	uint32_t time2;
	//state = 0;
	OledShowString(1,1,"start to timer");
	while(status == TIMERRUN){
		if(StateRead(ENSURE) == 1){
			timerstate = 1;
			OledShowString(1,1,"              ");
			StateClear(ENSURE);
		}else if(StateRead(RETURN) == 1){
			if(timerstate == 0){
				status = 1;
				return;
			}else{
				timerstate = 0;
				StateClear(RETURN);
			}
		}
		if(timerstate == 1){
			time2 = SysTime - timer;
			OledShowNUM(NUM4,(time2)%10);	
			OledShowNUM(NUM3,(time2)%60/10);
			OledShowNUM(NUM2,(time2)/60%10);
			OledShowNUM(NUM1,(time2)/60/10);
		}else{
			timer = SysTime - time2;
			OledShowString(1,1,"stop");		
		}
	}
};

//app2 is 倒计时闹钟status = 2 ,ensure = 1 
void CountdownTimer(void){
	OledClear();
	uint32_t localtimer = 0;
	uint8_t appstate = 0; /* 0 主页面，时间选择界面 , 1 时间设置 , 2 开始计�? 3 时间暂停*/
	uint8_t timerset = 0;  
	
	while(status == COUNTRUN){
		if(appstate == 0){
			if(StateRead(RETURN) == 1){
				status = 1;
				StateClear(RETURN);
				return;
			}else if(StateRead(RIGHT) == 1){
				timerset++;
				if(timerset == 4){
					timerset = 0;
				}
				StateClear(RIGHT);
			}else if(StateRead(LEFT) == 1){
				if(timerset == 0){
					timerset = 3;
				}else{
					timerset--;
				}
				StateClear(LEFT);
			}else if(StateRead(ENSURE) == 1){
				appstate = 1;
				StateClear(ENSURE);
			}
		}else if(appstate == 1){
				if(StateRead(RETURN) == 1){
					appstate = 0;
					StateClear(RETURN);
				}else if(StateRead(RIGHT) == 1){
					switch(timerset){
						case 0:
							if(localtimer > 600)
								localtimer -= 600;
							break;
						case 1:
							if(localtimer > 60)
								localtimer -= 60;
							break;
						case 2:
							if(localtimer > 10)
								localtimer -= 10;
							break;
						case 3:
							if(localtimer > 1)
								localtimer -= 1;
							break;
						default:
							break;
					}
					StateClear(RIGHT);
				}else if(StateRead(LEFT) == 1){
					switch(timerset){
						case 0:
							localtimer += 600;
							break;
						case 1:
							localtimer += 60;
							break;
						case 2:
							localtimer += 10;
							break;
						case 3:
							localtimer += 1;
							break;
						default:
							break;
					}
					StateClear(LEFT);
				}else if(StateRead(ENSURE) == 1){
					appstate = 2;
					StateClear(ENSURE);
				}
			}else if(appstate == 2){
				if(StateRead(RETURN) == 1){
					appstate = 3;
					StateClear(RETURN);
				}else{
					localtimer--;
				}			
			}else if(appstate == 3){
				if(StateRead(RETURN) == 1){
					status = 1;
					StateClear(RETURN);
					return;
				}else if(StateRead(ENSURE) == 1){
					appstate = 2;
					StateClear(ENSURE);
				}
			}
		if(appstate != 2 && appstate != 3){
			switch(timerset){
				case 0:
					OledShowNUMNo(NUM1,(localtimer)/600%6);
					break;
				case 1:				
					OledShowNUMNo(NUM2,(localtimer)/60%10);
					break;
				case 2:	
					OledShowNUMNo(NUM3,(localtimer)%60/10);
					break;
				case 3:			
					OledShowNUMNo(NUM4,(localtimer)%10);
					break;
				default:
					break;
			}	
			if(timerset != 0){
				OledShowNUM(NUM1,(localtimer)/600%6);
			}
			if(timerset != 1){
				OledShowNUM(NUM2,(localtimer)/60%10);
			}
			if(timerset != 2){
				OledShowNUM(NUM3,(localtimer)%60/10);
			}
			if(timerset != 3){
				OledShowNUM(NUM4,(localtimer)%10);
			}
		}else{
			OledShowNUM(NUM4,(localtimer)%10);	
			OledShowNUM(NUM3,(localtimer)%60/10);
			OledShowNUM(NUM2,(localtimer)/60%10);
			OledShowNUM(NUM1,(localtimer)/600%6);
		}
		
	}	
};
//app3 is 显示步数status = 3 ,ensure = 1 
void StepNum(void){
	OledClear();
	//float pitch,yaw,roll;
	//int16_t AX, AY, AZ;
	int16_t BX,BY,BZ, GX, GY, GZ;
	//short data[3];
	//unsigned long timestamp;
	//uint8_t DataH, DataL;
	while(status == STEPRUN){
		if(StateRead(RETURN) == 1){
			status = 1;
			return;
		}else{
			//MPU6050_DMP_Get_Data(&pitch,&roll,&yaw);
			//OledShowNum(1,1,pitch,4);
			//OledShowNum(2,1,roll,4);
			//OledShowNum(3,1,roll,4);
			
			MPU6050_GetData(&BX, &BY, &BZ, &GX, &GY, &GZ);
			OledShowSignedNum(2, 8, BX, 5);
			OledShowSignedNum(3, 8, BY, 5);
			OledShowSignedNum(4, 8, BZ, 5);		
		}
	}
};
#include "usart.h"
#include "systick.h"
extern struct QueueStack * JdyStack;
//app4 is 手机音乐切换器status = 4 ,ensure = 1 
void Mp3Switch(void){
	OledClear();
	OledShowString(4,1,"nihao");
	//UsartSendString(JDY_USARTx,"AT+VERSION\r\n");
	//DelayUs(5);
	//for(int8_t i = 1;i < 5;i++){
	//			for(int8_t j = 1; j < 15;j++){
	//				if(JdyStack->size != 0){
	//					OledShowChar(i,j,QueuePop(JdyStack));
	//				}else{
	//					break;
	//				}
	//			}
	//		}
	
	//设置分割符，0xff为分割符
	uint8_t data;
	uint8_t lock = 0;
	int8_t j = 0;
	while(status == MP3RUN){
		if(StateRead(RETURN) == 1){
			status = 1;
			return;
		}else{
			
			if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET)
			{
				if(QueuePop(JdyStack) == 0xFF){
				
				}
				DelayUs(5);
			}					
		}
	}
};


//app5 is storage
void Storage(void)
{
	OledClear();
	uint8_t MID;
	uint8_t flag_write = 0; //flag of write data to flash
	uint16_t DID;
	
	
	OledShowString(1, 1, "MID:   DID:");
	
	W25Q64SectorErase(0x00);
	W25Q64ReadID(&MID, &DID);
	OledShowHexNum(1, 5, MID, 2);
	OledShowHexNum(1, 12, DID, 4);	

	//将蓝牙接收到的数据转发到外部flash中
	while(1){
		if(StateRead(RETURN) == 1){
			status = 1;
			return;
		}else{
			if(flag_write == 0){
				SpiChose(0);
				W25Q64WriteEnable();
				SpiSwapByte(W25Q64_PAGE_PROGRAM);
				SpiSwapByte(0x00 >> 16);
				SpiSwapByte(0x00 >> 8);
				SpiSwapByte(0x00);
				for(uint8_t i =0;i<JdyStack->size;i++){
					if(JdyStack->phead == NULL)
					{	
						flag_write = 1;
						i=16;
						SpiChose(1);
						break;
					}
					SpiSwapByte(QueuePop(JdyStack));
				}
			}
		}
	}
};



//蓝牙转串口发送来的各种信息
#include "systick.h" //
void dateDeal(void){
	uint32_t data = 0;
	data |= (QueuePop(JdyStack)<<24);
	data |= (QueuePop(JdyStack)<<16);
	data |= (QueuePop(JdyStack)<<8);
	data |= (QueuePop(JdyStack));
	ClockTime = data;
	//更新全局时间
	DateCalculate();
	while(QueuePop(JdyStack) != 0xFE);
};

void weatherDeal(void)
{
	Weather = QueuePop(JdyStack);
	while(QueuePop(JdyStack) != 0xFE);
};

void musicDeal(void)
{
	uint8_t data = 0;
	//数据量比较大的存储在W25Q64中
	while(data == ENDSIGN){
		data = ReadBufOne();
		//TODO();将文本存放在flash中
	}
}; 
