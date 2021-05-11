/******************************************************************************
文 件 名   : sys.h
版 本 号   : V1.0
作    者   : pinot
生成日期   : 2019年10月21日
功能描述   : 系统寄存器配置相关
修改历史   :
日    期   :
作    者   :
修改内容   :
******************************************************************************/

#ifndef _SYS_H_
#define _SYS_H_

/*****************************************************************************
系统库*/

/*****************************************************************************
自定义头文件*/
#include "T5LOS8051.H"

/*****************************************************************************
宏定义变量*/
#ifndef uint8_t
typedef unsigned char uint8_t;
#endif
#ifndef uint16_t
typedef unsigned short uint16_t;
#endif
#ifndef uint32_t
typedef unsigned int uint32_t;
#endif
#ifndef int8_t
typedef char int8_t;
#endif
#ifndef int16_t
typedef short int16_t;
#endif
#ifndef int32_t
typedef int int32_t;
#endif
#ifndef u8
typedef unsigned char u8;
#endif
#ifndef u16
typedef unsigned short u16;
#endif
#ifndef u32
typedef unsigned long u32;
#endif

typedef struct
{
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 1;
    unsigned char b5 : 1;
    unsigned char b6 : 1;
    unsigned char b7 : 1;
} _FLAG_bits;

typedef union
{
    _FLAG_bits bits;
    unsigned char byte;
} _TKS_FLAGA_type;

#define DWIN_OK (0x00)
#define DWIN_ERROR (0xFF)
#define DWIN_NULL_POINT (0x02) /* 空指针 */
#define DWIN_PARAM_ERROR (0x03)
#define NULL ((void *)0)                /* 数据NULL */
#define FOSC (206438400UL)              /* T5L主频=晶体频率(11.0592M)*56/3 */
#define T1MS (65536 - FOSC / 12 / 1000) /* 10MS定时器 */

/*****************************************************************************
宏定义函数*/
#define WDT_ON() MUX_SEL |= 0x02  /******开启看门狗*********/
#define WDT_OFF() MUX_SEL &= 0xFD /******关闭看门狗*********/
#define WDT_RST() MUX_SEL |= 0x01 /******喂狗*********/

//电源开关宏定义
#define POWER_ON() P2_1 = 1;
#define POWER_OFF() P2_1 = 0;
//电源开关引脚
sbit P2_1 = P2 ^ 1;
//电压反馈，低电平表示按键有效
sbit KEY1_Vin_F = P2 ^ 2;

#define SOFTWARE_VER 0x0102

extern data _TKS_FLAGA_type MSFlag;
#define MS1msFlag MSFlag.bits.b0
#define MS100msFlag MSFlag.bits.b1
#define MS250msFlag MSFlag.bits.b2
#define MS500msFlag MSFlag.bits.b3

#define VP_TEMP 0x3000
/*****************************************************************************
对外函数声明*/
void InitSys(void); /* 寄存器配置初始化 */
// void DelayUs(uint16_t n);
void DelayMs(uint16_t n);

#endif
