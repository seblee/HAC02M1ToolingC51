/******************************************************************************
 * 文 件 名   : timer.c
 * 版 本 号   : V1.0
 * 作    者   : pinot
 * 生成日期   : 2019年11月18日
 * 功能描述   : 定时器应用
 * 修改历史   :
 * 日    期   :
 * 作    者   :
 * 修改内容   :
 ******************************************************************************/

/*****************************************************************************
自定义头文件*/
#include "timer.h"

/*****************************************************************************
全局变量*/
uint16_t Wait_Count    = 0;
uint16_t Key_Count     = 0;
uint16_t Toset_Count   = 0;
unsigned long ctr_inc  = 0;
unsigned int ctr1_inc  = 0;
unsigned long ctr2_inc = 0;
u16 data SysTick_RTC   = 0;
u32 data SysTick       = 0;
//计算key延时
uint16_t Key2_Count            = 0;
data _TKS_FLAGA_type timerFlag = {0};
/*****************************************************************************
定时器0*/
void T0_Init(void)
{
    TMOD |= 0x01;
    TH0 = T1MS >> 8;  // 1ms定时器
    TL0 = T1MS;
    ET0 = 1;  //开启定时器0中断
    TR0 = 1;  //开启定时器
}
void T0_ISR_PC(void) interrupt 1
{
    EA  = 0;
    TH0 = T1MS >> 8;
    TL0 = T1MS;
    Wait_Count++;

    if (Wait_Count > 1000)
    {
        Wait_Count = 0;
        Key_Count++;
        Toset_Count++;
    }
    WDT_RST();

    ctr_inc++;
    ctr1_inc++;
    ctr2_inc++;
    if (Key2_Count < 65500)
    {
        Key2_Count++;
    }
    SysTick_RTC++;
    SysTick++;
    if ((!(SysTick % 100)) && SysTick)
        timer100msFlag = 1;
    if ((!(SysTick % 250)) && SysTick)
        timer250msFlag = 1;
    if ((!(SysTick % 500)) && SysTick)
        timer500msFlag = 1;
    timer1msFlag = 1;
    EA           = 1;
}
/*****************************************************************************
定时器2*/
void T2_Init(void)
{
    T2CON = 0x70;
    TH2   = 0x00;
    TL2   = 0x00;
    //  TRL2H=0xBC;
    //  TRL2L=0xCD;        //1ms的定时器
    TRL2H = 0xFF;
    TRL2L = 0xEF;  // 1us的定时器
    IEN0 |= 0x20;  //开启定时器2
    TR2 = 0x01;
}
void T2_ISR_PC(void) interrupt 5
{
    TF2 = 0;
    // WDT_RST();
}
/*****************************************************************************
定时器总配置*///delayus
void InitTimer(void)
{
    T0_Init();
    T2_Init();
}
