#include <T5LOS8051.H>
#include "T5L_lib.h"
//检查指定DGUS变量地址的DATA3清零的话就退出
void wait_ok(unsigned int addr)
{
    ADR_H   = 0x00;
    ADR_M   = (unsigned char)(addr >> 8);
    ADR_L   = (unsigned char)(addr);
    ADR_INC = 0x00;
    do
    {
        for (addr = 0; addr < 1000; addr++)  //释放变量空间一段时间
        {
            DATA2 = DATA1;
        }
        RAMMODE = 0xAF;
        while (APP_ACK == 0)
            ;
        APP_EN = 1;
        while (APP_EN == 1)
            ;
        RAMMODE = 0x00;
    } while (DATA3 != 0);
}

//读写T5L片内256KW Flash，mod=0x5A 为读取，mod=0xA5为写入
// addr=DGUS变量地址，必须是偶数；addr_flash=flash读取地址，必须是偶数；len=读取字长度，必须是偶数。
void T5L_Flash(unsigned char mod, unsigned int addr, long addr_flash, unsigned int len)
{
    ADR_H   = 0x00;
    ADR_M   = 0x00;
    ADR_L   = 0x04;
    ADR_INC = 0x01;
    RAMMODE = 0x8F;  //启动读Flash
    while (APP_ACK == 0)
        ;
    DATA3  = mod;
    DATA2  = (unsigned char)(addr_flash >> 16);
    DATA1  = (unsigned char)(addr_flash >> 8);
    DATA0  = (unsigned char)(addr_flash & 0xFE);
    APP_EN = 1;
    while (APP_EN == 1)
        ;
    DATA3  = (unsigned char)(addr >> 8);
    DATA2  = (unsigned char)(addr & 0xFE);
    DATA1  = (unsigned char)(len >> 8);
    DATA0  = (unsigned char)(len & 0xFE);
    APP_EN = 1;
    while (APP_EN == 1)
        ;
    RAMMODE = 0x00;
    wait_ok(0x0004);
}  //等待数据读取OK

//读取DGUS变量数据 addr=DGUS变量地址  *buf=数据返回指针  len=读取数据字长度
void read_dgusii_vp(unsigned int addr, unsigned char *buf, unsigned int len)
{
    unsigned char i;
    i       = (unsigned char)(addr & 0x01);
    addr    = addr / 2;
    ADR_H   = 0x00;
    ADR_M   = (unsigned char)(addr >> 8);
    ADR_L   = (unsigned char)(addr);
    ADR_INC = 0x01;
    RAMMODE = 0xAF;
    while (APP_ACK == 0)
        ;
    while (len > 0)
    {
        APP_EN = 1;
        while (APP_EN == 1)
            ;
        if ((i == 0) && (len > 0))
        {
            *buf++ = DATA3;
            *buf++ = DATA2;
            i      = 1;
            len--;
        }
        if ((i == 1) && (len > 0))
        {
            *buf++ = DATA1;
            *buf++ = DATA0;
            i      = 0;
            len--;
        }
    }
    RAMMODE = 0x00;
}

//写DGUS变量数据 addr=DGUS变量地址  *buf=写入数据指针  len=写入数据字长度
void write_dgusii_vp(unsigned int addr, unsigned char *buf, unsigned int len)
{
    unsigned char i;
    i       = (unsigned char)(addr & 0x01);
    addr    = addr / 2;
    ADR_H   = 0x00;
    ADR_M   = (unsigned char)(addr >> 8);
    ADR_L   = (unsigned char)(addr);
    ADR_INC = 0x01;
    RAMMODE = 0x8F;
    while (APP_ACK == 0)
        ;
    if (i && len > 0)
    {
        RAMMODE = 0x83;
        DATA1   = *buf++;
        DATA0   = *buf++;
        APP_EN  = 1;
        while (APP_EN == 1)
            ;
        len--;
    }
    RAMMODE = 0x8F;
    while (len >= 2)
    {
        DATA3  = *buf++;
        DATA2  = *buf++;
        DATA1  = *buf++;
        DATA0  = *buf++;
        APP_EN = 1;
        while (APP_EN == 1)
            ;
        len = len - 2;
    }
    if (len)
    {
        RAMMODE = 0x8C;
        DATA3   = *buf++;
        DATA2   = *buf++;
        APP_EN  = 1;
        while (APP_EN == 1)
            ;
    }
    RAMMODE = 0x00;
}

//看门狗配置并开启
void wdt_on(void)
{
    MUX_SEL = MUX_SEL & 0xFD;  //配置WDT溢出时间前，关闭WDT
    ADR_H   = 0xFF;
    ADR_M   = 0x00;
    ADR_L   = 0x1B;
    RAMMODE = 0x8F;  // WDT溢出时间=0xFF001B值/CPU主频
    DATA3   = 0x0C;  // 206.4384MHz下，0x0C4E000为1秒
    DATA2   = 0x4E;
    DATA1   = 0x00;
    DATA0   = 0x00;
    APP_EN  = 1;
    while (APP_EN == 1)
        ;
    RAMMODE = 0x00;
    MUX_SEL = MUX_SEL | 0x02;
}  //开启WDT

//看门狗关闭
void wdt_off(void)
{
    MUX_SEL = MUX_SEL & 0xFD;
}

//看门狗喂狗
void wdt_rst(void)
{
    MUX_SEL = MUX_SEL | 0x01;
}

// T5L OS CPU初始化
void initcpu(void)
{
    // CKCON     = 0x00;
    // T2CON     = 0x70;
    // DPC       = 0x00;
    // PAGESEL   = 0x01;
    // D_PAGESEL = 0x02;  // DATA RAM 08000-0FFFF
    // MUX_SEL = 0x60;  //把UART2、UART3 引出来，WDT关闭
    // RAMMODE = 0x00;  //不占用DGUS变量存储器
    PORTDRV = 0x01;  // IO驱动强度 +/-8mA
    IEN0    = 0x00;  //关闭所有中断
    IEN1    = 0x00;
    IEN2    = 0x00;
    IP0     = 0x00;  //中断优先级用默认值
    IP1     = 0x00;

    P0      = 0x00;  // IO配置，输出关闭，是不是输出取决于P0MDOUT
    P1      = 0x00;
    P2      = 0x00;
    P3      = 0x00;
    P0MDOUT = 0x50;  // P0 0101 0000
    P1MDOUT = 0x00;  // P1 0000 0000
    P2MDOUT = 0x00;  // P2 0000 0000
    P3MDOUT = 0x00;  // P3 0000 0000

    // ADCON  = 0x80;  // UART2 配置 8N1 115200
    // SCON0  = 0x50;
    // SREL0H = 0x03;  //波特率=FCLK/64*(1024-SREL)
    // SREL0L = 0xE4;

    // SCON1  = 0x90;  // UART3 配置 8N1 115200
    // SREL1H = 0x03;  //波特率=FCLK/32*(1024-SREL1)
    // SREL1L = 0xC8;

    // SCON2T      = 0x80;  // UART4 配置 8N1 115200
    // SCON2R      = 0x80;
    // BODE2_DIV_H = 0x00;  //波特率=FCLK/8*DIV
    // BODE2_DIV_L = 0xE0;

    // SCON3T      = 0x80;  // UART5 配置 8N1 115200
    // SCON3R      = 0x80;
    // BODE3_DIV_H = 0x00;  //波特率=FCLK/8*DIV
    // BODE3_DIV_L = 0xE0;

    // TMOD = 0x11;  // T0 16bit定时器
    // TH0  = 0x00;
    // TL0  = 0x00;
    // TR0  = 0;
    // TCON = 0x05;  // EX0 EX1是下跳沿中断

    // TH1 = 0x00;  // T1 16bit定时器
    // TL1 = 0x00;
    // TR1 = 0;

    // T2CON = 0x71;  // T2 16bit Autoload模式
    // TH2   = 0x00;
    // TL2   = 0x00;
    // TRL2H = 0xFF;  // 10uS定时器
    // TRL2L = 0x54;
}
