是个人学习后的实践项目，并不是很专业

```c
├─DebugConfig
├─DMA
├─Library	//stm32系统库
├─Listings
├─mylib    //自己模块
├─Objects  //项目编译文件
├─Start  //stm32系统文件
├─USART //蓝牙通信的串口配置
├─User //main函数
└─W25Q64 //数据存储模块
```

```c
 #mylib 文件
 bluetooth.c
 bluetooth.h
 inter.c   //中断函数
 inter.h
 keyborad.c  //按键
 keyborad.h
 mainwindow.c  //主界面和状态机转换
 mainwindow.h
 MPU6050.c   //传感器用于计步
 MPU6050.h
 MPU6050_Reg.h
 oled.c		//显示接口
 oled.h
 oled_font.h
 queue.c	//消息队列
 queue.h
 sensor.c  //其他传感器
 sensor.h
 systick.c  //系统时钟，分频得到
 systick.h
```





踩坑：

MPU6050有运动检测功能，不用设置DMP和FIFO就能达到计步的功能。

我在配置MPU6050的过程中使用了硬件IIC。因为项目中很多中断，进入中断很频繁，会在硬件IIC的过程中进入中断，导致硬件IIC的一些冲突，使得之后的IIC传输相位不正确。使用软件IIC可以解决这个问题。

在使用软件IIC配置的过程中，中断导致系统时钟计时器出现bug