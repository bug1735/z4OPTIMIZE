#ifndef __KEYBORAD_H
#define __KEYBORAD_H

#ifdef LIB
#include "lib.h"
#else
#include "stm32f10x.h"
#endif

//open input using interruput
void KeyBoradInit2(void);

//a test function
void GPIOA0_Init(void);
#endif
