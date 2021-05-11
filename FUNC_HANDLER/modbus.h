/**
 * @file modbus.h
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
#ifndef __MODBUS_H__
#define __MODBUS_H__

#include "sys.h"

#define UART_TX_BUF_MAX_LEN 128
#define UART_RX_BUF_MAX_LEN 128

#define MODBUS_READ_REG_ADDRESS 0x4001  //寄存器首地址

#define MODBUS_WRITE_REG_ADDRESS 0x5001  //寄存器写地址

#define MODBUS_SEND_TIME_PERIOD 500  // 0.5秒钟
#define RX_CHECK_TICK_TIME 10  //超过10个字节的时间没有收到新的字符，认为接收结束，10*10*1000/9600,

#define SLAVE_ID 0x02  //设备端地址
#define BUS_EN 0X5A
#define BUS_DIS 0X00
#define BUS_FUN_03H 0x03  //读寄存器地址命令
#define BUS_FUN_06H 0x06  //写寄存器地址命令
#define CMD_NUMBER (sizeof(modbusCmdlib) / sizeof(modbosCmd_t))
#define CHECK_NUMBER (sizeof(dataCheckLib) / sizeof(dataCheckCmd_t))

typedef struct modbosCmd
{
    u8 modbusEn;
    u8 slaveID;
    u8 funCode;
    u8 length;
    u8 timeout;
    u8 mode;
    u16 modePara;
    u16 VPAddr;
    u16 slaveAddr;
    u16 feedback;
} modbosCmd_t;
typedef struct dataCheckCmd
{
    u8 Enable;
    u8 page;
    u16 dataAddr;
    u16 backAddr;
    u16 flagAddr;
} dataCheckCmd_t;

enum
{
    MODE_ALWA = 0,
    MODE_PAGE = 1,
    MODE_PARA = 2,
    MODE_PANP = 3,
};
extern u8 modbus_rx_flag;                      //接收到的字符串的标志，为1表示有收到数据
extern u8 modbus_rx_buf[UART_RX_BUF_MAX_LEN];  //接收到的字符串的内容
extern u8 modbus_rx_count;                     //接收到的字符串的长度

extern _TKS_FLAGA_type modbusFlag;
#define cmdTxFlag modbusFlag.bits.b0
#define cmdRxFlag modbusFlag.bits.b1

void Modbus_UART_Init(void);
void Modbus_Process_Task(void);
void getCmd(u8 *index);
void checkChange(void);
void forcedOutputHnadle(void);

#endif
