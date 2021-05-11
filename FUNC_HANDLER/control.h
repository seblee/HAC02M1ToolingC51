/**
 * @file control.h
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
#ifndef __CONTROL_H_
#define __CONTROL_H_
#include "sys.h"

#define TOUCH_EVENT_FLAG 0x5014

#define POWER_SWITCH_EVENT 0Xa000
#define ALARM_CONFIRM_EVENT 0Xa026
#define PASSWORD_CONFIRM_EVENT 0Xa400
#define PASSWORD_CANCLE_EVENT 0Xa401
#define PASSWORD_PAGEJUMP_START 0xa4a0
#define PASSWORD_PAGEJUMP_00_EVENT 0xA4A0
#define PASSWORD_PAGEJUMP_01_EVENT 0xA4A1
#define PASSWORD_PAGEJUMP_02_EVENT 0xA4A2
#define PASSWORD_PAGEJUMP_03_EVENT 0xA4A3
#define PASSWORD_PAGEJUMP_04_EVENT 0xA4A4
#define PASSWORD_PAGEJUMP_05_EVENT 0xA4A5
#define PASSWORD_PAGEJUMP_06_EVENT 0xA4A6
#define PASSWORD_PAGEJUMP_07_EVENT 0xA4A7
#define PASSWORD_PAGEJUMP_08_EVENT 0xA4A8
#define PASSWORD_PAGEJUMP_09_EVENT 0xA4A9
#define PASSWORD_PAGEJUMP_0A_EVENT 0xA4AA
#define PASSWORD_PAGEJUMP_0B_EVENT 0xA4AB
#define PASSWORD_PAGEJUMP_0C_EVENT 0xA4AC
#define PASSWORD_PAGEJUMP_0D_EVENT 0xA4AD
#define PASSWORD_PAGEJUMP_0E_EVENT 0xA4AE
#define PASSWORD_PAGEJUMP_0F_EVENT 0xA4AF
#define PASSWORD_PAGEJUMP_10_EVENT 0xA4B0
#define PASSWORD_PAGEJUMP_11_EVENT 0xA4B1
#define PASSWORD_PAGEJUMP_12_EVENT 0xA4B2
#define PASSWORD_PAGEJUMP_13_EVENT 0xA4B3
#define PASSWORD_PAGEJUMP_14_EVENT 0xA4B4
#define PASSWORD_PAGEJUMP_15_EVENT 0xA4B5
#define PASSWORD_PAGEJUMP_16_EVENT 0xA4B6
#define PASSWORD_PAGEJUMP_17_EVENT 0xA4B7
#define PASSWORD_PAGEJUMP_18_EVENT 0xA4B8
#define PASSWORD_PAGEJUMP_19_EVENT 0xA4B9
#define PASSWORD_PAGEJUMP_1A_EVENT 0xA4BA
#define PASSWORD_PAGEJUMP_1B_EVENT 0xA4BB
#define PASSWORD_PAGEJUMP_1C_EVENT 0xA4BC
#define PASSWORD_PAGEJUMP_1D_EVENT 0xA4BD
#define PASSWORD_PAGEJUMP_1E_EVENT 0xA4BE
#define PASSWORD_PAGEJUMP_1F_EVENT 0xA4BF
#define PASSWORD_PAGEJUMP_20_EVENT 0xA4C0
#define PASSWORD_PAGEJUMP_21_EVENT 0xA4C1
#define PASSWORD_PAGEJUMP_22_EVENT 0xA4C2
#define PASSWORD_PAGEJUMP_23_EVENT 0xA4C3
#define PASSWORD_PAGEJUMP_24_EVENT 0xA4C4
#define PASSWORD_PAGEJUMP_25_EVENT 0xA4C5
#define PASSWORD_PAGEJUMP_26_EVENT 0xA4C6
#define PASSWORD_PAGEJUMP_27_EVENT 0xA4C7
#define PASSWORD_PAGEJUMP_28_EVENT 0xA4C8
#define PASSWORD_PAGEJUMP_29_EVENT 0xA4C9
#define PASSWORD_PAGEJUMP_2A_EVENT 0xA4CA
#define PASSWORD_PAGEJUMP_2B_EVENT 0xA4CB
#define PASSWORD_PAGEJUMP_2C_EVENT 0xA4CC
#define PASSWORD_PAGEJUMP_2D_EVENT 0xA4CD
#define PASSWORD_PAGEJUMP_2E_EVENT 0xA4CE
#define PASSWORD_PAGEJUMP_2F_EVENT 0xA4CF
#define PASSWORD_PAGEJUMP_30_EVENT 0xA4D0
#define PASSWORD_PAGEJUMP_31_EVENT 0xA4D1

#define PASSWORD_FUN_00_EVENT 0xA4F0
#define PASSWORD_FUN_01_EVENT 0xA4F1
#define PASSWORD_FUN_02_EVENT 0xA4F2
#define PASSWORD_FUN_03_EVENT 0xA4F3

#define PASSWORD_CHANGE_CONFIRM_EVENT 0xA600
#define PASSWORD_CHANGE_CANCLE_EVENT 0xA601

#define ALARM_CLEAR_EVENT 0Xac00
#define CUR_ALARM_CLEAR_EVENT 0Xac01
#define RESET_EVENT 0Xc500
#define IN_MAINTAIN_MOD_EVENT 0Xc600
#define OUTPUT_EVENT 0Xc700
#define OUT_MAINTAIN_MOD_EVENT 0Xc701
#define CLEAR_RUNTIME_EVENT_0D 0Xc90d
#define CLEAR_RUNTIME_EVENT_07 0Xc907
#define CLEAR_RUNTIME_EVENT_01 0Xc901
#define CLEAR_RUNTIME_EVENT_0E 0Xc90e
#define CLEAR_RUNTIME_EVENT_11 0Xc911
#define REST_ORIGINAL_PARA 0xcf00
#define SAVE_FACTORY_CFG 0xcf01

#define LEVEL_NUM 6
#define PASSWORD_FLASH_START 0x3000
#define PASSWORD_FLASH_LENTH (LEVEL_NUM - 1) * 2

typedef enum password_mode
{
    PWM_PAGEJUMP = 0x0000,
    PWM_FUN      = 0x0001,
} _password_mode_t;

enum
{
    FUN00 = 0X00,
    FUN01 = 0X01,
    FUN02 = 0X02,
    FUN03 = 0X03,
};

extern u8 passwordGotLevel;

void touchHandler(void);
void resetEventHandle(void);
void clearRunTimeHandle(u16 eventId);
void powerSwitchEventHandle(void);
void inMaintainModEventHandle(void);
void outMaintainModEventHandle(void);
void resetOriginalPara(void);
void saveFactoryCFG(void);

void passwordConfirmEventHandle(void);
void passwordCancleEventHandle(void);
void passwordPageJumpEventHandle(u16 event);
void passwordFunEventHandle(u16 event);
void passwordOperation(void);
void passwordFunOPThandle(u16 fun);
u8 getPasswordLevel(u16 event);
u8 checkPassword(u8 level, u8* input);
void passwordInit(void);
void passwordTask(void);
void savePassword(void);
void passwordChangeConfirmEventHandle(void);
void pageHandle(u16 page);

#endif
