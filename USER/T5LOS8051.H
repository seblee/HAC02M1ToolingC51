#ifndef __T5LOS8051_H__
#define __T5LOS8051_H__

/* COMMON BYTE Register  */
sfr P0              = 0x80;                     /*P0端口*/
sfr P1              = 0x90;                     /*P1端口*/
sfr P2              = 0xA0;                     /*P2端口*/
sfr P3              = 0xB0;                     /*P3端口(只有P3.0-P3.3有效)*/
sfr PSW             = 0xD0;                     /*程序状态字*/
sfr ACC             = 0xE0;                     /*累加器*/
sfr B               = 0xF0;                     /*乘除法辅助*/
sfr SP              = 0x81;                     /*堆栈指针*/
sfr DPL             = 0x82;                     /*数据指针(低8位)*/
sfr DPH             = 0x83;                     /*数据指针(高8位)*/
sfr PCON            = 0x87;                     /*电源控制及波特率*/
sfr TCON            = 0x88;                     /*定时器控制*/
sfr TMOD            = 0x89;                     /*定时器方式选择*/
sfr TL0             = 0x8A;                     /*定时器0低8位*/
sfr TL1             = 0x8B;                     /*定时器1低8位*/
sfr TH0             = 0x8C;                     /*定时器0高8位*/
sfr TH1             = 0x8D;                     /*定时器1高8位*/
//sfr IE              = 0xA8;                     /*<REG51>中断允许控制*/
//sfr IP              = 0xB8;                     /*<REG51>中断优先控制*/
sfr IP0             = 0xA9;                     /*中断优先级控制器0*/
sfr IP1             = 0xB9;                     /*中断优先级控制器1*/
//sfr SCON            = 0x98;                     /*<REG51>串行口控制*/
sfr SCON0           = 0x98;                     /*UART2控制接口*/
sfr SCON1           = 0x9B;                     /*UART3控制接口*/
sfr SBUF0           = 0x99;                     /*UART2收发缓冲区*/
sfr SBUF1           = 0x9C;                     /*UART3收发缓冲区*/

/* T5L BYTE Register Must INIT */
sfr CKCON           = 0x8E;                     /*|0x00|CPU运行在1T模式*/
sfr T2CON           = 0xC8;                     /*|0x70|T2控制寄存器*/
sfr DPC             = 0x93;                     /*|0x00|DPTR的变化模式*/
sfr PAGESEL         = 0x94;                     /*|0x01|64KB代码空间*/
sfr D_PAGESEL       = 0x95;                     /*|0x02|32KB的RAM空间*/
sfr MUX_SEL         = 0xC9;                     /*|0x60|外设复用选择*/
sfr PORTDRV         = 0xF9;                     /*|0x01|IO输出模式的驱动能力*/
sfr RAMMODE         = 0xF8;                     /*|0x00|DGUS变量存储器访问接口控制*/

/* BIT Register */
/* DGUS 变量存储器 */
sbit APP_REQ        = RAMMODE^7;                /*写1请求读写，不占用必须清零*/
sbit APP_EN         = RAMMODE^6;                /*写1启动1次读写，硬件执行后清零*/
sbit APP_RW         = RAMMODE^5;                /*1=读变量存储器，0=写变量存储器*/
sbit APP_ACK        = RAMMODE^4;                /*应答：1=OK，0=BUSY 需要继续等待*/
sbit APP_DATA3      = RAMMODE^3;                /*1=对应Byte写入，0=对应Byte不写入*/
sbit APP_DATA2      = RAMMODE^2;                /*同上*/
sbit APP_DATA1      = RAMMODE^1;                /*同上*/
sbit APP_DATA0      = RAMMODE^0;                /*同上*/
sfr ADR_H           = 0xF1;                     /*DGUS变量存储器高8位地址，A23:A16*/
sfr ADR_M           = 0xF2;                     /*DGUS变量存储器高8位地址，A15:A8*/
sfr ADR_L           = 0xF3;                     /*DGUS变量存储器高8位地址，A7:A0*/
sfr ADR_INC         = 0xF4;                     /*自动增量，ADR=(读写器)ADR+ADR_INC*/
sfr DATA3           = 0xFA;                     /*DGUS变量数据接口*/
sfr DATA2           = 0xFB;                     /*同上*/
sfr DATA1           = 0xFC;                     /*同上*/
sfr DATA0           = 0xFD;                     /*同上*/

/* SFR扩展接口 */
sfr EXADR           = 0xFE;                     /*扩展SFR地址*/
sfr EXDATA          = 0xFF;                     /*扩展SFR的数据接口*/

/* 数学运算单元MDU */
sfr MAC_MODE        = 0xE5;                     /*MAC硬件乘加器控制寄存器*/
sfr DIV_MODE        = 0xE6;                     /*DIV硬件除法器控制寄存器*/

/* 定时器0/1 */
sbit TF1            = TCON^7;                   /*T1溢出中断请求标志位*/
sbit TR1            = TCON^6;                   /*T1运行控制位*/
sbit TF0            = TCON^5;                   /*T0溢出中断请求标志位*/
sbit TR0            = TCON^4;                   /*T0运行控制位*/
sbit IE1            = TCON^3;                   /*外部中断1的中断标志*/
sbit IT1            = TCON^2;                   /*外部中断1控制位*/
sbit IE0            = TCON^1;                   /*外部中断0的中断标志*/
sbit IT0            = TCON^0;                   /*外部中断0控制位*/
/* 定时器2 */
sbit TSF2           = T2CON^7;                  /*0=CPU主频/12，1=CPU主频/24*/
sbit TR2            = T2CON^0;                  /*0=T2关闭，1=T2运行*/
sfr TH2             = 0xCD;                     /*T2运行值H*/
sfr TL2             = 0xCC;                     /*T2运行值L*/
sfr TRL2H           = 0xCB;                     /*T2的重装载值H*/
sfr TRL2L           = 0xCA;                     /*T2的重装载值L*/

/* IO口 */
sfr P0MDOUT         = 0xB7;                     /*P0口输出配置*/
sfr P1MDOUT         = 0xBC;                     /*P1口输出配置*/
sfr P2MDOUT         = 0xBD;                     /*P2口输出配置*/
sfr P3MDOUT         = 0xBE;                     /*P3口输出配置*/
/* IO测试 */
sbit P0_0           = P0^0;
sbit P1_0           = P1^0;
sbit P1_1           = P1^1;
sbit P3_1           = P3^1;

/* UART2 */
sbit SM0            = SCON0^7;                  /*方式选择1*/
sbit SM1            = SCON0^6;                  /*方式选择2*/
sbit SM2            = SCON0^5;                  /*多机通信控制*/
sbit REN0           = SCON0^4;                  /*串行接收允许*/
sbit TB80           = SCON0^3;                  /*欲发的第九位*/
sbit RB80           = SCON0^2;                  /*欲收的第九位*/
sbit TI0            = SCON0^1;                  /*发送中有无中断*/
sbit RI0            = SCON0^0;                  /*接收中有无中断*/
sfr ADCON           = 0xD8;                     /*波特率发生器选择*/
sfr SREL0H          = 0xBA;                     /*设置波特率，当ADCON为0x80时*/
sfr SREL0L          = 0xAA;

/* UART3 */
sfr SREL1H          = 0xBB;                     /*设置波特率*/
sfr SREL1L          = 0x9D;

/* UART4 */
sfr SCON2T          = 0x96;                     /*UART4发送控制*/
sfr SCON2R          = 0x97;                     /*UART4接收控制*/
sfr SBUF2_TX        = 0x9E;                     /*UART4发送数据接口*/
sfr SBUF2_RX        = 0x9F;                     /*UART4接收数据接口*/
sfr BODE2_DIV_H     = 0xD9;                     /*UART4波特率设置*/
sfr BODE2_DIV_L     = 0xE7;

/* UART5 */
sfr SCON3T          = 0xA7;                     /*UART5发送控制*/
sfr SCON3R          = 0xAB;                     /*UART5接收控制*/
sfr SBUF3_TX        = 0xAC;                     /*UART5发送数据接口*/
sfr SBUF3_RX        = 0xAD;                     /*UART5接收数据接口*/
sfr BODE3_DIV_H     = 0xAE;                     /*UART5波特率设置*/
sfr BODE3_DIV_L     = 0xAF;

/* CAN通信 */
sfr CAN_CR          = 0x8F;                     /*CAN接口控制寄存器*/
sfr CAN_IR          = 0x91;                     /*CAN接口中断状态寄存器*/
sfr CAN_ET          = 0xE8;                     /*CAN接口错误类型寄存器，位寻址*/

/* 中断系统配置 */
/* 控制0 */
sfr IEN0            = 0xA8;
sbit EA             = IEN0^7;                   /*中断总控制位*/
sbit ET2            = IEN0^5;                   /*T2定时器中断使能控制位*/
sbit ES0            = IEN0^4;                   /*UART2中断使能控制位*/
sbit ET1            = IEN0^3;                   /*T1定时器中断使能控制位*/
sbit EX1            = IEN0^2;                   /*外部中断1使能控制位P3.1*/
sbit ET0            = IEN0^1;                   /*T0定时器中断使能控制位*/
sbit EX0            = IEN0^0;                   /*外部中断0使能控制位P3.0*/
/* 控制1 */
sfr IEN1            = 0xB8;
sbit ES3R           = IEN1^5;                   /*UART5接收中断使能控制位*/
sbit ES3T           = IEN1^4;                   /*UART5发送中断使能控制位*/
sbit ES2R           = IEN1^3;                   /*UART4接收中断使能控制位*/
sbit ES2T           = IEN1^2;                   /*UART4发送中断使能控制位*/
sbit ECAN           = IEN1^1;                   /*CAN通信中断使能控制位*/
/* 控制2 */
sfr IEN2            = 0x9A;                     /*UART3中断使能控制位*/
/* 控制3 */
sfr IEN3            = 0xD1;                     /*UART3中断使能控制位*/

/* 中断标志 */
sfr IRCON           = 0xC0;                     /*中断标志寄存器4*/
sfr IRCON2          = 0xBF;
sbit TF2            = IRCON^6;                  /*T2中断触发标志*/

/* 程序状态寄存器 */
sbit CY             = PSW^7;                    /*进位、借位标志*/
sbit AC             = PSW^6;                    /*辅助进位、借位标志*/
sbit F0             = PSW^5;                    /*用户标志位*/
sbit RS1            = PSW^4;                    /*寄存器组选择控制位*/
sbit RS0            = PSW^3;                    /*寄存器组选择控制位*/
sbit OV             = PSW^2;                    /*溢出标志*/
sbit F1             = PSW^1;                    /*保留*/
sbit P              = PSW^0;                    /*奇偶校验标志位*/

#endif
