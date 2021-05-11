/**
 * @file ui.c
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

#include "ui.h"
#include "timer.h"
u16 picNow           = 0;
static u16 testState = 0;
static u8 beepCount  = 0;
/**
 * @brief ui task
 */
void ui(void)
{
    if (MS1msFlag)
    {
        ReadDGUS(0x0014, (u8 *)(&picNow), 2);
    }
    if (MS250msFlag)
    {
        beepFun();
    }

    if (MS500msFlag)
    {
        if (picNow == 0x0000)
        {
            u16 cache = 0;
            ReadDGUS(0xa021, (u8 *)(&cache), 2);
            if ((testState != 1) && (cache == 1))
            {
                beepCount += 1;
            }
            if ((testState != 2) && (cache == 2))
            {
                beepCount += 2;
            }
            if ((testState != 3) && (cache == 3))
            {
                beepCount += 4;
            }
            testState = cache;
        }
        if (picNow == 0x000b)
        {
            caculateGroupCtrlPic();
        }
        if (picNow == 0x0029)
        {
            u8 i;
            u16 hour_reg;
            u16 mb_comp_runtime[14], sys_comp_runtime[14];
            ReadDGUS(0xc9b0, (u8 *)mb_comp_runtime, 28);

            for (i = 0; i < 7; i++)
            {
                hour_reg                = (mb_comp_runtime[2 * i] << 4) + (mb_comp_runtime[2 * i + 1] >> 12);
                sys_comp_runtime[i]     = hour_reg / 24;
                sys_comp_runtime[i + 7] = hour_reg % 24;
            }
            WriteDGUS(0xc920, (u8 *)sys_comp_runtime, 28);
        }
        if (picNow == 0x002c)
        {
            u16 cache[4];
            ReadDGUS(0xcca0, (u8 *)&cache[3], 2);
            cache[0] = ((cache[3] >> 4) & 0x0f00);
            cache[0] |= ((cache[3] >> 7) & 0x1f);
            cache[1] = ((cache[3] & 0x007f) << 8);
            cache[2] = SOFTWARE_VER;
            WriteDGUS(0xcc20, (u8 *)cache, 6);
        }
        {
            static u8 diagnosisPageInCount  = 0;
            static u8 diagnosisPageOutCount = 0;
            if (picNow == 0x0027)
            {
                if (diagnosisPageInCount < 5)
                {
                    u16 cache = 1;
                    WriteDGUS(0xa025, (u8 *)&cache, 2);
                    cache = 0x005a;
                    WriteDGUS(0xa085, (u8 *)&cache, 2);
                    diagnosisPageInCount++;
                }
                diagnosisPageOutCount = 0;
            }
            else
            {
                if (diagnosisPageOutCount < 5)
                {
                    u16 cache = 0;
                    WriteDGUS(0xa025, (u8 *)&cache, 2);
                    cache = 0x005a;
                    WriteDGUS(0xa085, (u8 *)&cache, 2);
                    diagnosisPageOutCount++;
                }
                diagnosisPageInCount = 0;
            }
        }

        {
            static u16 timerCounter = 0;
            u16 cache;
            ReadDGUS(0x0016, (u8 *)&cache, 2);
            if (cache != 0)
            {
                cache = 0;
                WriteDGUS(0x0016, (u8 *)&cache, 2);
                timerCounter = 0;
            }
            else
            {
                if (timerCounter < STANGBYTIME)
                    timerCounter++;
                else if (timerCounter == STANGBYTIME)
                {
                    timerCounter++;
                    if (picNow != 0)
                        JumpPage(0);
                    //用户等级
                }
            }
        }
    }
}
/**
 * @brief  jump tu id page
 * @param  pageId page od
 */
void JumpPage(uint16_t pageId)
{
    uint8_t temp[4] = {0x5A, 0x01, 0, 0};
    temp[2]         = (uint8_t)(pageId >> 8);
    temp[3]         = pageId;
    WriteDGUS(0x0084, temp, sizeof(temp));
    // do
    // {
    //     DelayMs(5);
    //     ReadDGUS(DHW_SPAGE, temp, 1);
    // } while (temp[0] != 0);
}

enum
{
    SYS_STS_FSM = 0,     // 140
    SYS_STS_MALFUN_0,    // 141
    SYS_STS_MALFUN_1,    // 142
    SYS_STS_ONLINE_0,    // 143
    SYS_STS_ONLINE_1,    // 144
    SYS_STS_FINALOUT_0,  // 145
    SYS_STS_FINALOUT_1,  // 146
    SYS_STS_STA_0_3,     // 147
    SYS_STS_STA_4_7,     // 148
    SYS_STS_STA_8_11,    // 149
    SYS_STS_STA_12_15,   // 150
    SYS_STS_STA_16_19,   // 151
    SYS_STS_STA_20_23,   // 152
    SYS_STS_STA_24_27,   // 153
    SYS_STS_STA_28_31,   // 154
    SYS_CONF_CNT,        // 155
    SYS_CONF_MODE = 26,  // 166
    MAX_SYS_STS

};
/**
 * @brief 计算图标状态
 */
void caculateGroupCtrlPic(void)
{
#define GROUP_DEVICE_COUNT 18
    u16 mb_teamwork_sts_regs[31] = {0};
    u8 i;
    u16 lw_teamwork_icon_sta[GROUP_DEVICE_COUNT] = {0};
    short status, temp;

    ReadDGUS(0xab20, (u8 *)mb_teamwork_sts_regs, sizeof(mb_teamwork_sts_regs));

    for (i = 0; i < 16; i++)
    {
        //在线状态
        if ((mb_teamwork_sts_regs[SYS_STS_ONLINE_0] & (1 << i)) != 0)
        {
            //运行
            if ((mb_teamwork_sts_regs[SYS_STS_FINALOUT_0] & (1 << i)) != 0)
            {
                // get equipment status
                temp   = SYS_STS_STA_0_3 + (i / 4);
                status = (mb_teamwork_sts_regs[temp] >> (4 * (i % 4))) & 0x000f;
                switch (status)
                {
                    case 0x00:
                        lw_teamwork_icon_sta[i] = 5;
                        break;
                    case 0x01:  //制热
                        lw_teamwork_icon_sta[i] = 6;
                        break;
                    case 0x02:  //制冷
                        lw_teamwork_icon_sta[i] = 7;
                        break;
                    case 0x04:  //加湿
                        lw_teamwork_icon_sta[i] = 8;
                        break;
                    case 0x05:  //加湿+制热
                        lw_teamwork_icon_sta[i] = 9;
                        break;
                    case 0x06:  //加湿+制冷
                        lw_teamwork_icon_sta[i] = 10;
                        break;
                    case 0x08:  //除湿
                        lw_teamwork_icon_sta[i] = 3;
                        break;
                    case 0x09:  //除湿+制热
                        lw_teamwork_icon_sta[i] = 4;
                        break;
                    default:
                        lw_teamwork_icon_sta[i] = 5;
                        break;
                }
                //故障
                if ((mb_teamwork_sts_regs[SYS_STS_MALFUN_0] & (1 << i)) != 0)
                {
                    lw_teamwork_icon_sta[i] += 8;
                }
                else  //正常
                {
                    //保持原值
                }
            }
            else  //备用
            {     //故障
                if ((mb_teamwork_sts_regs[SYS_STS_MALFUN_0] & (1 << i)) != 0)
                {
                    lw_teamwork_icon_sta[i] = 2;
                }
                else  //正常
                {
                    lw_teamwork_icon_sta[i] = 1;
                }
            }
        }
        else
        {
            lw_teamwork_icon_sta[i] = 0;
        }
    }
    for (i = 16; i < GROUP_DEVICE_COUNT; i++)
    {
        //在线状态
        if ((mb_teamwork_sts_regs[SYS_STS_ONLINE_1] & (1 << (i - 16))) != 0)
        {
            //运行
            if ((mb_teamwork_sts_regs[SYS_STS_FINALOUT_1] & (1 << (i - 16))) != 0)
            {
                // get equipment status
                temp   = SYS_STS_STA_0_3 + (i / 4);
                status = (mb_teamwork_sts_regs[temp] >> (4 * (i % 4))) & 0x000f;
                switch (status)
                {
                    case 0x00:
                        lw_teamwork_icon_sta[i] = 5;
                        break;
                    case 0x01:  //制热
                        lw_teamwork_icon_sta[i] = 6;
                        break;
                    case 0x02:  //制冷
                        lw_teamwork_icon_sta[i] = 7;
                        break;
                    case 0x04:  //加湿
                        lw_teamwork_icon_sta[i] = 8;
                        break;
                    case 0x05:  //加湿+制热
                        lw_teamwork_icon_sta[i] = 9;
                        break;
                    case 0x06:  //加湿+制冷
                        lw_teamwork_icon_sta[i] = 10;
                        break;
                    case 0x08:  //除湿
                        lw_teamwork_icon_sta[i] = 3;
                        break;
                    case 0x09:  //除湿+制热
                        lw_teamwork_icon_sta[i] = 4;
                        break;
                    default:
                        lw_teamwork_icon_sta[i] = 5;
                        break;
                }
                //故障
                if ((mb_teamwork_sts_regs[SYS_STS_MALFUN_1] & (1 << (i - 16))) != 0)
                {
                    lw_teamwork_icon_sta[i] += 8;
                }
                else  //正常
                {
                    //保持原值
                }
            }
            else  //备用
            {     //故障
                if ((mb_teamwork_sts_regs[SYS_STS_MALFUN_1] & (1 << (i - 16))) != 0)
                {
                    lw_teamwork_icon_sta[i] = 2;
                }
                else  //正常
                {
                    lw_teamwork_icon_sta[i] = 1;
                }
            }
        }
        else
        {
            lw_teamwork_icon_sta[i] = 0;
        }
    }
    WriteDGUS(0xaba0, (u8 *)lw_teamwork_icon_sta, sizeof(lw_teamwork_icon_sta));
}

void beepFun(void)
{
    if (beepCount)
    {
        u16 cache = 0x0005;
        beepCount--;
        WriteDGUS(0x00A0, (u8 *)&cache, 2);  // GET PAGENOW
    }
}
