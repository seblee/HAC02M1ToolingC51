/******************************************************************************
文 件 名   : handle.h
版 本 号   : V1.0
作    者   : pinot
生成日期   : 2019年11月27日
功能描述   : 逻辑处理
修改历史   :
日    期   :
作    者   :
修改内容   :
******************************************************************************/

#ifndef _HANDLE_H_
#define _HANDLE_H_

/*****************************************************************************
系统库*/

/*****************************************************************************
自定义头文件*/
#include "T5LOS8051.H"
#include "sys.h"
#include "dgus.h"
#include "uart.h"
#include "norflash.h"
#include "timer.h"
#include "crc16.h"

/*****************************************************************************
宏定义变量*/
//系统变量接口  T1MS
#define DHW_NPAGE 0x0014   //当前页面
#define DHW_XCOOR 0x0017   // X坐标
#define DHW_YCOOR 0x0018   // Y坐标
#define DHW_LEDNOW 0x0031  // LED背光状态
#define DHW_SYSCFG 0x0080  //系统配置
#define DHW_LEDCFG 0x0082  // LED背光配置
#define DHW_SPAGE 0x0084   //设置页面
#define DHW_MONI 0x00D4    //模拟触控
#define DHW_0F00 0x0F00    //变量改变指示

#define SOFT_VERSION 0x000F
#define RTC 0x0010
#define TP_STATUS 0x0016
#define AD_VALUE 0x0032
#define RTC_Set 0x009C
#define RTC_Set_Internet 0xF430

//显示变量
//--设置功能
#define DS_SCFG_WFLAG 0x1010     //配置数据-写入标志位
#define DS_SCFG_KGID 0x1011      //配置数据-开关ID
#define DS_SCFG_WKID 0x1012      //配置数据-温控ID
#define DS_SCFG_XPTIME 0x1013    //配置数据-息屏时间
#define DS_SCFG_VOICE 0x1014     //配置数据-按键音
#define DS_SCFG_LANGUAGE 0x1015  //配置数据-语言选择
#define DS_SCFG_MBGN1 0x1016     //配置数据-面板功能1
#define DS_SCFG_MBGN2 0x1017     //配置数据-面板功能2
#define DS_MBGN2_TEMP 0x101A     //面板功能2显示
#define DS_LOGIN 0x1020          //密码登录
//--入户面板页面
#define DS_WR 0x1100      //底图-勿扰
#define DS_NAV 0x1101     //底图-导航栏
#define DS_LED 0x1102     //底图-灯光控制
#define DS_LED_N 0x1103   //等级
#define DS_MASTER 0x1104  //总开关
//--组合面板页面
#define DSZ_LED 0x1200     //底图-灯光
#define DSZ_LED_T 0x1201   //底图-灯光文字
#define DSZ_LED_I 0x1202   //底图-灯光图标
#define DSZ_LED_N 0x1203   //底图-灯光等级
#define DSZ_CLJL 0x1210    //底图-窗帘卷帘
#define DSZ_FS 0x1220      //底图-风速
#define DSZ_FS_T 0x1221    //底图-风速文字
#define DSZ_FS_N 0x1223    //底图-风速文字
#define DSZ_WD 0x1230      //底图-温度
#define DSZ_WD_C 0x1233    //底图-温度符号
#define DSZ_WD_D 0x1234    //底图-温度数值
#define DSZ_WD_DP 0x12A0   //底图-温度数值指针
#define DSZ_WD_DP1 0x12C0  //底图-温度数值指针1

//触控变量
//--A
#define DT_A1 0x2101  //上报按键1
#define DT_A2 0x2102  //上报按键2
#define DT_A3 0x2103  //上报按键3
#define DT_A4 0x2104  //上报按键4
//--B
#define DT_B1 0x2201  //上报百分比1
#define DT_B2 0x2202  //上报百分比2
//--C
#define DTC_SAVE 0x2001  //保存数据
#define DTC_BACK 0x2002  //返回主界面
#define DTC_MSG 0x2003   //跳转到信息
#define DTC_WTH 0x2004   //跳转到天气
#define DTC_LNG 0x2005   //跳转到语言
#define DTC_MBGN 0x2006  //面板功能跳转
#define DTC_CLJL 0x2007  //窗帘切换
#define DTC_SSHS 0x2008  //温度切换
//--D
#define DTD_CNEN 0x2301  //语言切换
//--E
#define DTE_FS 0x2401    //风速开关
#define DTE_FS_L 0x2402  //风速左按键
#define DTE_FS_R 0x2403  //风速右按键
#define DTE_WD_L 0x2404  //风速左按键
#define DTE_WD_R 0x2405  //风速右按键

//参数配置
#define DD_IDT_TON 0x3000   //初始数据-触控按下亮度
#define DD_IDT_TOUT 0x3001  //初始数据-触控抬起亮度
#define BR_MIN 0            //息屏亮度
#define BR_MAX 0xFF         //亮屏亮度
#define LANG_CN 1           //语言-中文
#define LANG_EN 2           //语言-英文
#define COLORB 0x001F       //颜色-蓝色
#define COLORW 0xFFFF       //颜色-白色
#define COLORR 0xF800       //颜色-红色

//数据库地址
#define NAR_DATA 0x000000  //读写NorFlash数据的首地址
#define NAR_LENGTH 8       //读写NorFlash数据的字长度,必须是偶数

//调试助手
#define VERS 0x1F00       //版本号显示
#define DEBUG_SET 0x1F10  //调试设置
#define XCOOR_P 0x1F20    // X坐标显示指针
#define YCOOR_P 0x1F40    // Y坐标显示指针
#define VERS_P 0x1F60     //版本号显示指针

#define UDEBUG 0
#if UDEBUG
#define LOG(x, y)           \
    {                       \
        Uart2SendStr(x, y); \
    }
//#define LOG(x,y)    { Uart3SendStr(x,y); }
//#define LOG(x,y)    { Uart4SendStr(x,y); }
//#define LOG(x,y)    { Uart5SendStr(x,y); }
#else
#define LOG(x, y) \
    {             \
        ;         \
    }
#endif

/*****************************************************************************
对外函数声明*/
void InitUartSend(void);
void InitDgusData(void);
void GetConfig(void);
void SetIndex(void);
void HandleProc(void);
void JumpPage(uint16_t pageId);
void SetVoice(int8_t);

extern u8 Power_Flag;
extern u8 TP_sdata;

#endif
