/**
 * @file alarm.c
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

#include "sys.h"
#include "alarm.h"
#include "T5L_lib.h"
#include "dgus.h"
#include "ui.h"
#include "timer.h"
#include "ChineseCharacter.h"

#define START_Xs 105
#define START_Ys 100
#define START_Xe 285
#define START_Ye 125

#define END_Xs 295
#define END_Ys 100
#define END_Xe 475
#define END_Ye 125

#define STRING_Xs 485
#define STRING_Ys 100
#define STRING_Xe 685
#define STRING_Ye 125

#define END_X 295
#define STRING_X 485
const dgus_hex_t showStartDesConst = {
    0x3fc9,    //.VP
    START_Xs,  //.X
    START_Ys,  //.Y
    0,         //.Color
    0x86,      //.Byte_Num
    0x00,      //.Lib_ID
    0x0a,      //.Font_x
    0x2d,      //.String_Code[0]
    0x2d,      //.String_Code[1]
    0x20,      //.String_Code[2]
    0x3a,      //.String_Code[3]
    0x3a,      //.String_Code[4]
};
const dgus_hex_t showEndDesConst = {
    0x3fcc,  //.VP
    END_Xs,  //.X
    END_Ys,  //.Y
    0,       //.Color
    0x86,    //.Byte_Num
    0x00,    //.Lib_ID
    0x0a,    //.Font_x
    0x2d,    //.String_Code[0]
    0x2d,    //.String_Code[1]
    0x2f,    //.String_Code[2]
    0x3a,    //.String_Code[3]
    0x3a,    //.String_Code[4]
};
const dgus_string_t showStringDesConst = {
    0x3fd0,     //.VP
    STRING_Xs,  //.X
    STRING_Ys,  //.Y
    0,          //.Color
    STRING_Xs,  //.Xs
    STRING_Ys,  //.Ys
    STRING_Xe,  //.Xe
    STRING_Ye,  //.Ye
    0X0010,     //.Text_length
    0x00,       //.Font0_ID
    0X17,       //.Font1_ID
    0x18,       //.Font_X_Dots
    0x18,       //.Font_Y_Dots
    0x02,       //.Encode_Mode
    0x00,       //.HOR_Dis
    0x00,       //.VER_Dis
    0x00,       //.NC
};

const u16 alarmShowDescriptionVP[10][3] = {
    {SHOWTIMESTART0, SHOWTIMEEND0, SHOWSTRING0},  //
    {SHOWTIMESTART1, SHOWTIMEEND1, SHOWSTRING1},  //
    {SHOWTIMESTART2, SHOWTIMEEND2, SHOWSTRING2},  //
    {SHOWTIMESTART3, SHOWTIMEEND3, SHOWSTRING3},  //
    {SHOWTIMESTART4, SHOWTIMEEND4, SHOWSTRING4},  //
    {SHOWTIMESTART5, SHOWTIMEEND5, SHOWSTRING5},  //
    {SHOWTIMESTART6, SHOWTIMEEND6, SHOWSTRING6},  //
    {SHOWTIMESTART7, SHOWTIMEEND7, SHOWSTRING7},  //
    {SHOWTIMESTART8, SHOWTIMEEND8, SHOWSTRING8},  //
    {SHOWTIMESTART9, SHOWTIMEEND9, SHOWSTRING9},  //
};
const u16 historyShowVP[10] = {
    HISTORYSHOWVP0, HISTORYSHOWVP1, HISTORYSHOWVP2, HISTORYSHOWVP3, HISTORYSHOWVP4,
    HISTORYSHOWVP5, HISTORYSHOWVP6, HISTORYSHOWVP7, HISTORYSHOWVP8, HISTORYSHOWVP9,
};

alarmInfoStrc_t alarmInfomation = {0, 0, 0, 0};
static u16 alarmState[8];
static u16 pageBak     = 0;
static u16 showPage    = 0;
static u16 showPagebAK = 0;
u8 showIndex           = 0;
static u8 alarmCount   = 0;

const u16 alarmBeep = 0x0005;

/*******************************************************************
 * @description:
 * @param {type}
 * @return {type}
 ******************************************************************/
void alarmInit(void)
{
    u8 cache[32] = {0};
    u16 i;
    alarmDataStrc_t *alarmData = (alarmDataStrc_t *)cache;
    alarmInfoStrc_t *alarmInfo = (alarmInfoStrc_t *)cache;

    for (i = 0; i < ALARM_LEN; i++)
    {
        T5L_Flash(READRFLASH, alarmTemp, ALARM_FLASH_START + i * 8, 8);

        ReadDGUS(alarmTemp, (u8 *)&cache, sizeof(alarmDataStrc_t));
        if (alarmData->flag == ALARMDATALAG)
        {
            continue;
        }
        else if (alarmInfo->flag == ALARMINFOFLAG)
        {
            memcpy(&alarmInfomation, alarmInfo, sizeof(alarmInfoStrc_t));
            break;
        }
        else
        {
            break;
        }
    }

    for (i = 0; i < 10; i++)
    {
        u16 showTemp[16] = {0};
        memcpy(showTemp, &showStartDesConst, sizeof(dgus_hex_t));
        ((dgus_hex_t *)showTemp)->Y      = START_Ys + 27 * i;
        ((dgus_hex_t *)showTemp)->Lib_ID = 1;
        WriteDGUS(alarmShowDescriptionVP[i][0], (u8 *)showTemp, sizeof(showTemp));
        memcpy(showTemp, &showEndDesConst, sizeof(dgus_hex_t));
        ((dgus_hex_t *)showTemp)->Y      = END_Ys + 27 * i;
        ((dgus_hex_t *)showTemp)->Lib_ID = 1;
        WriteDGUS(alarmShowDescriptionVP[i][1], (u8 *)showTemp, sizeof(showTemp));
        memcpy(showTemp, &showStringDesConst, sizeof(dgus_string_t));
        ((dgus_string_t *)showTemp)->Y  = STRING_Ys + 27 * i;
        ((dgus_string_t *)showTemp)->Ys = STRING_Ys + 27 * i;
        ((dgus_string_t *)showTemp)->Ye = STRING_Ye + 27 * i;
        WriteDGUS(alarmShowDescriptionVP[i][2], (u8 *)showTemp, sizeof(showTemp));
    }
}
void alarmTask(void)
{
    u8 cache[50];
    u8 i, j;
    if (MS500msFlag != 1)
    {
        return;
    }
    ReadDGUS(0xa021, (u8 *)&cache, 12);  // GET PAGENOW
    if (alarmCount != (*((u16 *)&cache[0]) + *((u16 *)&cache[2])))
    {
        if (alarmCount < (*((u16 *)&cache[0]) + *((u16 *)&cache[2])))
        {
            *((u16 *)&cache[10]) = 1;
            WriteDGUS(ALARM_BEEP_FLAG, (u8 *)&cache[10], 2);
        }
        alarmCount = *((u16 *)&cache[0]) + *((u16 *)&cache[2]);
    }

    if ((*((u16 *)&cache[0]) > 0) || (*((u16 *)&cache[2]) > 0))
    {
        if (*((u16 *)&cache[10]))
            WriteDGUS(WAE_PALY_ADDR, (u8 *)&alarmBeep, 2);  // GET PAGENOW
    }

    if (picNow == CURRENTALARMPAGE)
    {
        if (pageBak != picNow)
        {
            showPage = 0;
            WriteDGUS(ALARMPAGEADDR, (u8 *)&showPage, 2);  // set
        }
        else
        {
            ReadDGUS(ALARMPAGEADDR, (u8 *)&showPage, 2);  // GET PAGENOW
        }

        ReadDGUS(alarmStateVP, (u8 *)cache, 16);
        cache[14] = 0;
        cache[15] &= 0x07;
        if ((memcmp(cache, alarmState, sizeof(alarmState))) || (showPage != showPagebAK) || (pageBak != picNow))
        {
            alarmDataStrc_t *alarmData;
            memcpy(alarmState, cache, sizeof(alarmState));
            showIndex = 0;

            for (j = 0; j < 8; j++)
            {
                for (i = 0; i < 16; i++)
                {
                    u8 alarmIndex = i + 16 * j;
                    ReadDGUS(alarmVPStart + alarmIndex * 24, (u8 *)&cache, 48);  // get memory
                    alarmData = (alarmDataStrc_t *)cache;
                    if (alarmState[j] & (1 << i))
                    {
                        if (alarmData->flag != ALARMDATALAG)
                        {
                            alarmData->flag = ALARMDATALAG;
                            ReadDGUS(0x0010, (u8 *)&cache[20], 8);  // read time
                            memcpy(&cache[2], &cache[20], 3);
                            memcpy(&cache[5], &cache[24], 3);
                            memset(&cache[8], 0, 8);
                            *((u16 *)&cache[14]) = alarmIndex;
                            strncpy(&cache[16], &alarmMessage[alarmIndex][0], 32);
                            WriteDGUS(alarmVPStart + alarmIndex * 24, (u8 *)&cache, 48);  // write memory
                        }
                        if ((showIndex >= (showPage * 10)) && (showIndex < (showPage * 10 + 10)))
                        {
                            setAlarmDisplay((showIndex - (showPage * 10)), (alarmVPStart + alarmIndex * 24),
                                            CURRENTALARMPAGE);
                        }
                        showIndex++;
                    }
                    else
                    {
                        if (alarmData->flag == ALARMDATALAG)
                        {
                            ReadDGUS(0x0010, (u8 *)&cache[20], 8);  // read time
                            memcpy(&cache[8], &cache[20], 3);
                            memcpy(&cache[11], &cache[24], 3);
                            *((u16 *)&cache[14]) = alarmIndex;
                            WriteDGUS(alarmTemp, (u8 *)&cache, 16);  // write memory
                            saveAlarmHistory();
                            *((u16 *)&cache[0]) = 0;
                            WriteDGUS(alarmVPStart + alarmIndex * 24, (u8 *)&cache, 2);  // write memory
                        }
                    }
                }
            }
            for (i = showIndex; i < (showPage * 10 + 10); i++)
            {
                resetAlarmDisplay(i - showPage * 10);
            }
            showPagebAK = showPage;
        }
    }
    else if (picNow == ALARMHISTORYAGE)
    {
        if (pageBak != picNow)
        {
            showPage = 0;
            WriteDGUS(ALARMPAGEADDR, (u8 *)&showPage, 2);  // set
            goto refreshHistory;
        }
        else
        {
            ReadDGUS(ALARMPAGEADDR, (u8 *)&showPage, 2);  // GET PAGENOW
            if (showPage != showPagebAK)
                goto refreshHistory;
        }
        goto alarmTaskExit;
    refreshHistory:
        showPagebAK = showPage;
        for (showIndex = 0; showIndex < 10; showIndex++)
        {
            u16 addressTemp;
            if ((10 * showPage + showIndex) >= alarmInfomation.length)
            {
                break;
            }
            if (alarmInfomation.head_ptr > (10 * showPage + showIndex))
            {
                addressTemp = alarmInfomation.head_ptr - (10 * showPage + showIndex + 1);
            }
            else
            {
                addressTemp = (ALARM_LEN + alarmInfomation.head_ptr) - (10 * showPage + showIndex + 1);
            }

            T5L_Flash(READRFLASH, alarmTemp, ALARM_FLASH_START + addressTemp * 8, 8);
            ReadDGUS(alarmTemp, cache, 16);
            strncpy(&cache[16], &alarmMessage[((alarmDataStrc_t *)cache)->alarmCode][0], 32);
            WriteDGUS(historyShowVP[showIndex], cache, 48);

            setAlarmDisplay(showIndex, historyShowVP[showIndex], ALARMHISTORYAGE);
        }

        for (showIndex; showIndex < 10; showIndex++)
        {
            resetAlarmDisplay(showIndex);
        }
    }
alarmTaskExit:
    pageBak = picNow;
}

void alarmClearHandle(void)
{
    memset(&alarmInfomation, 0, sizeof(alarmInfoStrc_t));
    WriteDGUS(alarmTemp, (u8 *)&alarmInfomation, sizeof(alarmInfoStrc_t));  // write memory
    T5L_Flash(WRITERFLASH, alarmTemp, ALARM_FLASH_START + alarmInfomation.head_ptr * 8, 4);
}
void curAlarmClearHandle(void)
{
    u16 cache = 0x005a;
    WriteDGUS(0xac20, (u8 *)&cache, 2);
    WriteDGUS(0xac80, (u8 *)&cache, 2);
}
void saveAlarmHistory(void)
{
    T5L_Flash(WRITERFLASH, alarmTemp, ALARM_FLASH_START + alarmInfomation.head_ptr * 8, 8);
    alarmInfomation.head_ptr++;
    if (alarmInfomation.head_ptr >= ALARM_LEN)
    {
        alarmInfomation.head_ptr = 0;
    }
    if (alarmInfomation.length >= ALARM_LEN)
    {
        alarmInfomation.length = ALARM_LEN;
        alarmInfomation.tail_ptr++;
        if (alarmInfomation.tail_ptr >= ALARM_LEN)
        {
            alarmInfomation.tail_ptr = 0;
        }
    }
    else
    {
        alarmInfomation.length++;
    }
    alarmInfomation.flag = ALARMINFOFLAG;

    WriteDGUS(alarmTemp, (u8 *)&alarmInfomation, sizeof(alarmInfoStrc_t));  // write memory
    T5L_Flash(WRITERFLASH, alarmTemp, ALARM_FLASH_START + alarmInfomation.head_ptr * 8, 4);
}

void setAlarmDisplay(u8 index, u16 vp, u8 page)
{
    u16 showTemp[16] = {0};
    memcpy(showTemp, &showStartDesConst, sizeof(dgus_hex_t));
    ((dgus_hex_t *)showTemp)->Y      = START_Ys + 27 * index;
    ((dgus_hex_t *)showTemp)->Lib_ID = 0;
    ((dgus_hex_t *)showTemp)->VP     = vp + 1;
    WriteDGUS(alarmShowDescriptionVP[index][0], (u8 *)showTemp, sizeof(showTemp));

    memcpy(showTemp, &showEndDesConst, sizeof(dgus_hex_t));
    ((dgus_hex_t *)showTemp)->Y      = END_Ys + 27 * index;
    ((dgus_hex_t *)showTemp)->Lib_ID = ALARMHISTORYAGE - page;
    ((dgus_hex_t *)showTemp)->VP     = vp + 4;
    WriteDGUS(alarmShowDescriptionVP[index][1], (u8 *)showTemp, sizeof(showTemp));

    *((u16 *)&showTemp[0]) = vp + 8;
    WriteDGUS(alarmShowDescriptionVP[index][2], (u8 *)&showTemp, 2);
}

void resetAlarmDisplay(u8 index)
{
    u16 showTemp[16] = {0};
    memcpy(showTemp, &showStartDesConst, sizeof(dgus_hex_t));
    ((dgus_hex_t *)showTemp)->Y      = START_Ys + 27 * index;
    ((dgus_hex_t *)showTemp)->Lib_ID = 1;
    WriteDGUS(alarmShowDescriptionVP[index][0], (u8 *)showTemp, sizeof(showTemp));
    memcpy(showTemp, &showEndDesConst, sizeof(dgus_hex_t));
    ((dgus_hex_t *)showTemp)->Y      = END_Ys + 27 * index;
    ((dgus_hex_t *)showTemp)->Lib_ID = 1;
    WriteDGUS(alarmShowDescriptionVP[index][1], (u8 *)showTemp, sizeof(showTemp));
    memcpy(showTemp, &showStringDesConst, sizeof(dgus_string_t));
    ((dgus_string_t *)showTemp)->Y  = STRING_Ys + 27 * index;
    ((dgus_string_t *)showTemp)->Ys = STRING_Ys + 27 * index;
    ((dgus_string_t *)showTemp)->Ye = STRING_Ye + 27 * index;
    WriteDGUS(alarmShowDescriptionVP[index][2], (u8 *)showTemp, sizeof(showTemp));
}

void alarmConfirmEventHandle(void)
{
    u16 cache = 0;
    WriteDGUS(ALARM_BEEP_FLAG, (u8 *)&cache, 2);
}
