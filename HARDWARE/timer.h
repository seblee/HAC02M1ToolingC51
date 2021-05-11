/******************************************************************************
文 件 名   : timer.h
版 本 号   : V1.0
作    者   : pinot
生成日期   : 2019年11月27日
功能描述   : 定时器应用
修改历史   :
日    期   :
作    者   :
修改内容   :
******************************************************************************/

#ifndef _TIMER_H_
#define _TIMER_H_

/*****************************************************************************
系统库*/

/*****************************************************************************
自定义头文件*/
#include "T5LOS8051.H"
#include "sys.h"

extern data _TKS_FLAGA_type timerFlag;
#define timer1msFlag timerFlag.bits.b0
#define timer100msFlag timerFlag.bits.b1
#define timer250msFlag timerFlag.bits.b2
#define timer500msFlag timerFlag.bits.b3
#define timerFlagClear timerFlag.byte = 0

/*****************************************************************************
宏定义变量*/
extern uint16_t Key_Count;
extern uint16_t Key2_Count;
extern uint16_t Toset_Count;

extern u16 data SysTick_RTC;
extern u32 data SysTick;

/*****************************************************************************
对外函数声明*/
void InitTimer(void);

#endif