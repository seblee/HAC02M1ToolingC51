/**
 * @file rtc.c
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
#include "rtc.h"
// #include "handle.h"
#include "string.h"
#include "dgus.h"

//**********************RX8130接口程序，SDA 10K上拉到3.3V**************
//上电时运行一次initrtc()，然后0.5秒间隔运行一次rdtime()读取时间到DGUS相应系统接口

uint8_t RTC_temp[7] = {0};
//时间校准
uint8_t time_calibra[8]           = {0};
uint8_t prtc_set1[8]              = {0};
const uint8_t code table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};  //月修正数据表

void SDA_IN(void)
{
    P3MDOUT = P3MDOUT & 0xF7;
}

void SDA_OUT(void)
{
    P3MDOUT = P3MDOUT | 0x08;
}
void delayus(unsigned char t)
{
    char i;
    while (t)
    {
        for (i = 0; i < 50; i++)
        {
            i = i;
        }
        t--;
    }
}

void i2cstart(void)
{
    SDA_OUT();
    RTC_SDA = 1;
    RTC_SCL = 1;
    delayus(15);
    RTC_SDA = 0;
    delayus(15);
    RTC_SCL = 0;
    delayus(15);
}

void i2cstop(void)
{
    SDA_OUT();
    RTC_SDA = 0;
    RTC_SCL = 1;
    delayus(15);
    RTC_SDA = 1;
    delayus(15);
    SDA_IN();
}

void mack(void)
{
    SDA_OUT();
    RTC_SDA = 0;
    delayus(5);
    RTC_SCL = 1;
    delayus(5);
    RTC_SCL = 0;
    delayus(5);
}

void mnak(void)
{
    SDA_OUT();
    RTC_SDA = 1;
    delayus(5);
    RTC_SCL = 1;
    delayus(5);
    RTC_SCL = 0;
    delayus(5);
}

void cack(void)
{
    unsigned char i;
    SDA_IN();
    RTC_SDA = 1;
    delayus(5);
    RTC_SCL = 1;
    delayus(5);
    for (i = 0; i < 50; i++)
    {
        if (!RTC_SDA)
        {
            break;
        }
        delayus(5);
    }
    RTC_SCL = 0;
    delayus(5);
    SDA_OUT();
}

// I2C 写入1个字节
void i2cbw(unsigned char dat)
{
    char i;
    SDA_OUT();
    for (i = 0; i < 8; i++)
    {
        if (dat & 0x80)
        {
            RTC_SDA = 1;
        }
        else
        {
            RTC_SDA = 0;
        }
        dat = (dat << 1);
        delayus(5);
        RTC_SCL = 1;
        delayus(5);
        RTC_SCL = 0;
        delayus(5);
    }
    cack();
}

// i2c 读取1个字节数据
unsigned char i2cbr(void)
{
    char i;
    unsigned char dat;
    SDA_IN();
    for (i = 0; i < 8; i++)
    {
        delayus(5);
        RTC_SCL = 1;
        delayus(5);
        dat = (dat << 1);
        if (RTC_SDA)
        {
            dat = dat | 0x01;
        }
        else
        {
            dat = dat & 0xFE;
        }
        // dat=(dat<<1);
        RTC_SCL = 0;
        delayus(5);
    }
    return (dat);
}

//检查8130有没有掉电，掉电则初始化8130，设置时间为2017.01.01 星期一 00:00:00
void init_rtc(void)
{
    unsigned char i;
    //检查有没有掉电
    i2cstart();
    i2cbw(0x64);
    i2cbw(0x1d);
    i2cstop();
    i2cstart();
    i2cbw(0x65);
    i = i2cbr();
    mack();
    i2cbr();
    mnak();
    i2cstop();
    if ((i & 0x02) == 0x02)
    {                //重新配置时间
        i2cstart();  // 30=00
        i2cbw(0x64);
        i2cbw(0x30);
        i2cbw(0x00);
        i2cstop();
        i2cstart();  // 1C-1F=48 00 40 10
        i2cbw(0x64);
        i2cbw(0x1C);
        i2cbw(0x48);
        i2cbw(0x00);
        i2cbw(0x40);
        i2cbw(0x10);
        i2cstop();
        i2cstart();  // 10-16=RTC设置值 BCD格式
        i2cbw(0x64);
        i2cbw(0x10);
        i2cbw(0x00);  //秒
        i2cbw(0x00);  //分
        i2cbw(0x00);  //时
        i2cbw(0x01);  //星期
        i2cbw(0x01);  //日
        i2cbw(0x01);  //月
        i2cbw(0x17);  //年
        i2cstop();
        i2cstart();  // 1E-1F 00 10
        i2cbw(0x64);
        i2cbw(0x1E);
        i2cbw(0x00);
        i2cbw(0x10);
        i2cstop();
    }
}

void RTC_Set_Time(u8* prtc_set)
{
    i2cstart();  // 30=00
    i2cbw(0x64);
    i2cbw(0x30);
    i2cbw(0x00);
    i2cstop();
    i2cstart();  // 1C-1F=48 00 40 10
    i2cbw(0x64);
    i2cbw(0x1C);
    i2cbw(0x48);
    i2cbw(0x00);
    i2cbw(0x40);
    i2cbw(0x10);
    i2cstop();
    i2cstart();  // 10-16=RTC设置值 BCD格式
    i2cbw(0x64);
    i2cbw(0x10);
    i2cbw(prtc_set[6]);  //秒
    i2cbw(prtc_set[5]);  //分
    i2cbw(prtc_set[4]);  //时
    i2cbw(prtc_set[3]);  //星期
    i2cbw(prtc_set[2]);  //日
    i2cbw(prtc_set[1]);  //月
    i2cbw(prtc_set[0]);  //年
    i2cstop();
    i2cstart();  // 1E-1F 00 10
    i2cbw(0x64);
    i2cbw(0x1E);
    i2cbw(0x00);
    i2cbw(0x10);
    i2cstop();
}

//把RTC读取并处理，写到DGUS对应的变量空间，主程序中每0.5秒调用一次
void rdtime(void)
{
    unsigned char rtcdata[8];
    unsigned char i, n, m;  //, k;
    i2cstart();
    i2cbw(0x64);
    i2cbw(0x10);
    i2cstop();
    i2cstart();
    i2cbw(0x65);
    for (i = 6; i > 0; i--)
    {
        rtcdata[i] = i2cbr();
        mack();
    }
    rtcdata[0] = i2cbr();
    mnak();
    i2cstop();
    for (i = 0; i < 3; i++)  //年月日转换成HEX
    {
        n          = rtcdata[i] / 16;
        m          = rtcdata[i] % 16;
        rtcdata[i] = n * 10 + m;
    }
    for (i = 4; i < 7; i++)  //时分秒转换成HEX
    {
        n          = rtcdata[i] / 16;
        m          = rtcdata[i] % 16;
        rtcdata[i] = n * 10 + m;
    }
    //处理星期的数据格式
    //	n=0;
    //	m=rtcdata[3];
    //	for(i=0;i<7;i++)
    //	{
    //		if(m&0x01)
    //		{
    //			break;
    //		}
    //		n++;
    //		m=(m>>1);
    //	}
    //	rtcdata[3]=n;

    rtcdata[3] = RTC_Get_Week(rtcdata[0], rtcdata[1], rtcdata[2]);

    WriteDGUS(0x0010, (u8*)rtcdata, sizeof(rtcdata));  //写入DGUS变量空间
}

/*****************************************************************************
 函 数 名  : void Time_Calibration(void)
 功能描述  : 时间校准，读取的为RTC时间设置的值
 输入参数  :
 输出参数  :
 修改历史  :
  1.日    期   : 2019年5月2日
    作    者   : chengjing
    修改内容   : 创建
****************************************************************************

*/
uint8_t RTC_Get_Week(uint8_t year, uint8_t month, uint8_t day)
{
    u16 y, c, m, d, w;
    uint16_t year_real = 0;

    year_real = (u16)year + 2000;
    y         = year_real % 100;  //年　如2015 即年是15年
    c         = year_real / 100;  // 年份前两位　如2015即20
    m         = month;
    d         = day;
    if (m == 1 || m == 2)
    {  //判断月份是否为1或2
        y--;
        m += 12;  //某年的1、2月要看作上一年的13、14月来计算
    }
    w = y + y / 4 + c / 4 - 2 * c + 13 * (m + 1) / 5 + d - 1;  //蔡勒公式的公式
    while (w < 0)
        w += 7;  //确保余数为正
    w %= 7;
    return w;
}

void RTC_Set_CMD(void)
{
    unsigned char time_calibra1[8];
    unsigned char i, n, m;
    uint8_t temp[2];
    uint8_t week = 0;
    ReadDGUS(RTC_Set, (uint8_t*)time_calibra, 8);
    if ((time_calibra[0] == 0x5A) && (time_calibra[1] == 0xA5))
    {
        for (i = 2; i < 8; i++)  //年月日时分秒转换成HEX
        {
            n                = time_calibra[i] / 10;
            m                = time_calibra[i] % 10;
            time_calibra1[i] = n * 16 + m;
        }

        prtc_set1[0] = time_calibra1[2];
        prtc_set1[1] = time_calibra1[3];
        prtc_set1[2] = time_calibra1[4];
        prtc_set1[4] = time_calibra1[5];
        prtc_set1[5] = time_calibra1[6];
        prtc_set1[6] = time_calibra1[7];
        temp[1]      = RTC_Get_Week(time_calibra[2], time_calibra[3], time_calibra[4]);

        temp[0]      = 0;
        prtc_set1[3] = temp[1];
        n            = temp[1] / 10;
        m            = temp[1] % 10;

        RTC_Set_Time(prtc_set1);

        time_calibra[0] = 0;
        time_calibra[1] = 0;
        WriteDGUS(RTC_Set, (uint8_t*)time_calibra, 8);
    }
}
