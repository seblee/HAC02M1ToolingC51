/**
 * @file alarm.h
 * @brief
 * @author  xiaowine (xiaowine@sina.cn)
 * @version 01.00
 * @date    2020-10-12
 *
 * @copyright Copyright (c) {2020}  xiaowine
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2020-10-12 <td>1.0     <td>wangh     <td>内容
 * </table>
 * ******************************************************************
 * *                   .::::
 * *                 .::::::::
 * *                ::::::::::
 * *             ..:::::::::::
 * *          '::::::::::::
 * *            .:::::::::
 * *       '::::::::::::::..        女神助攻,流量冲天
 * *            ..::::::::::::.     永不宕机,代码无bug
 * *          ``:::::::::::::::
 * *           ::::``:::::::::'        .:::
 * *          ::::'   ':::::'       .::::::::
 * *        .::::'      ::::     .:::::::'::::
 * *       .:::'       :::::  .:::::::::' ':::::
 * *      .::'        :::::.:::::::::'      ':::::
 * *     .::'         ::::::::::::::'         ``::::
 * * ...:::           ::::::::::::'              ``::
 * *```` ':.          ':::::::::'                  ::::.
 * *                   '.:::::'                    ':'````.
 * ******************************************************************
 */
#ifndef __ALARM_H_
#define __ALARM_H_

/*****************************************************************************
系统库*/
#include "string.h"
#include "sys.h"

#define ALARM_FLASH_START 0X2000
#define ALARM_FLASH_END 0X2000

#define ALARM_LEN 512

#define alarmTemp 0x3000
#define ALARMDATALAG 0xa55a
#define ALARMINFOFLAG 0x5aa5

#define CURRENTALARMPAGE 13
#define ALARMHISTORYAGE 14
#define ALARMPAGEADDR 0xae01
#define alarmStateVP 0xaea0  // 8个
#define alarmVPStart 0x3500  // 24*116
// show alarm list description
#define SHOWTIMESTART0 0x4000
#define SHOWTIMESTART1 0x4010
#define SHOWTIMESTART2 0x4020
#define SHOWTIMESTART3 0x4030
#define SHOWTIMESTART4 0x4040
#define SHOWTIMESTART5 0x4050
#define SHOWTIMESTART6 0x4060
#define SHOWTIMESTART7 0x4070
#define SHOWTIMESTART8 0x4080
#define SHOWTIMESTART9 0x4090

#define SHOWTIMEEND0 0x40a0
#define SHOWTIMEEND1 0x40b0
#define SHOWTIMEEND2 0x40c0
#define SHOWTIMEEND3 0x40d0
#define SHOWTIMEEND4 0x40e0
#define SHOWTIMEEND5 0x40f0
#define SHOWTIMEEND6 0x4100
#define SHOWTIMEEND7 0x4110
#define SHOWTIMEEND8 0x4120
#define SHOWTIMEEND9 0x4130

#define SHOWSTRING0 0x4140
#define SHOWSTRING1 0x4150
#define SHOWSTRING2 0x4160
#define SHOWSTRING3 0x4170
#define SHOWSTRING4 0x4180
#define SHOWSTRING5 0x4190
#define SHOWSTRING6 0x41a0
#define SHOWSTRING7 0x41b0
#define SHOWSTRING8 0x41c0
#define SHOWSTRING9 0x41d0

#define HISTORYSHOWVP0 0x41e0
#define HISTORYSHOWVP1 0x41f8
#define HISTORYSHOWVP2 0x4210
#define HISTORYSHOWVP3 0x4228
#define HISTORYSHOWVP4 0x4240
#define HISTORYSHOWVP5 0x4258
#define HISTORYSHOWVP6 0x4270
#define HISTORYSHOWVP7 0x4288
#define HISTORYSHOWVP8 0x42a0
#define HISTORYSHOWVP9 0x42b8

#define WAE_PALY_ADDR 0x00A0
#define ALARM_BEEP_FLAG 0xa026

typedef struct alarmDataStruct
{
    u16 flag;
    u8 startY;
    u8 startM;
    u8 startD;
    u8 start_h;
    u8 start_m;
    u8 start_s;
    u8 endY;
    u8 endM;
    u8 endD;
    u8 end_h;
    u8 end_m;
    u8 end_s;
    u16 alarmCode;
} alarmDataStrc_t;

typedef struct alarmInfoStruct
{
    u16 flag;
    u16 head_ptr;
    u16 tail_ptr;
    u16 length;
} alarmInfoStrc_t;

void alarmInit(void);
void alarmTask(void);
void saveAlarmHistory(void);
void setAlarmDisplay(u8 index, u16 vp, u8 page);
void resetAlarmDisplay(u8 index);
void alarmClearHandle(void);
void curAlarmClearHandle(void);
void alarmConfirmEventHandle(void);

#endif
