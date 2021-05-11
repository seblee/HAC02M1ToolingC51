/******************************************************************************
 * 文 件 名   : handle.c
 * 版 本 号   : V1.0
 * 作    者   : pinot
 * 生成日期   : 2019年11月27日
 * 功能描述   : 逻辑处理
 * 修改历史   :
 * 日    期   :
 * 作    者   :
 * 修改内容   :
 ******************************************************************************/

/*****************************************************************************
自定义头文件*/
#include "handle.h"

/*****************************************************************************
全局变量*/
//内部RAM
uint8_t value0F00[4] = {0};
uint8_t xp_status    = 0;
//是否开机标志位
u8 Power_Flag            = 0;
u8 TP_sdata              = 0;  //屏幕状态标志位
uint8_t temp_page[2]     = {0};
uint8_t temp_data[2]     = {0};
uint8_t TP_sdatus[2]     = {0};
uint8_t temp_TPdata[4]   = {0x00, 0x00, 0x46, 0x50};
uint8_t TP_sdatus_open[] = {0x5A, 0xA5, 0x0A, 0x83, 0x20, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//外部RAM
typedef struct dlt_645_8283
{
    uint8_t st_byte1;    //帧起始符
    uint8_t st_byte2;    //帧起始符
    uint8_t lenth;       //长度
    uint8_t func;        //类型
    uint8_t addr1;       //地址
    uint8_t addr2;       //地址
    uint8_t edata[250];  //数据内容
} dl_t_645_8283_body;
dl_t_645_8283_body xdata s_dl_t_645_8283_body = {0};

#define FRAME_BUFFER_LEN 255
uint8_t xdata frame_buf[FRAME_BUFFER_LEN];       //用来存取一帧数据
uint8_t xdata frame_buf1[FRAME_BUFFER_LEN] = 0;  //用来存取一帧数据
#define DLT_645_LEAST_LEN 7

//代码空间
uint8_t code CUart2[] = "Uart2Init Done!!!123";
uint8_t code CUart3[] = "Uart3Init Done!!!";
uint8_t code CUart4[] = "Uart4Init Done!!!";
uint8_t code CUart5[] = "Uart5Init Done!!!";

//解析到一帧数据可能出现的情况
typedef enum frame_result
{
    UNKNOWN,
    OK,          //成功找到一帧
    UNFINISHED,  //未接收完成
    ERROR,       //不满足此协议
} frame_result_t;

//定义协议类型
typedef enum protocol_type
{
    PROTOCOL_UNKNOWN,
    PROTOCOL_DL_T_698,
    PROTOCOL_DL_T_645,
    PROTOCOL_OTHER,
} protocol_type_t;

/*****************************************************************************
控制亮度*/
void SetBlight(uint8_t n)
{
    //	uint8_t temp[4]={0};
    WriteDGUS(DHW_LEDCFG, (uint8_t *)&n, sizeof(n));
    //	do
    //	{
    //    DelayMs(5);
    //	  ReadDGUS(DHW_LEDNOW,temp,2);//设置亮度与获取到的亮度不一致
    //	}while(!(temp[1]==n));
}
/*****************************************************************************
控制伴音*/
void SetVoice(int8_t status)
{
    uint8_t temp[4] = {0};
    ReadDGUS(DHW_SYSCFG, temp, sizeof(temp));
    temp[0] = 0x5A;
    if (1 == status)
    {
        temp[3] &= 0xF7;
    }
    else if (0 == status)
    {
        temp[3] |= 0x08;
    }
    else
    {
        return;
    }
    WriteDGUS(DHW_SYSCFG, temp, sizeof(temp));
    do
    {
        DelayMs(5);
        ReadDGUS(DHW_SYSCFG, temp, 1);
    } while (temp[0] != 0);
}
/*****************************************************************************
判断按键值是否有上传*/
int8_t GetValue0F00(void)
{
    uint8_t cleanData[4] = {0};
    ReadDGUS(DHW_0F00, value0F00, sizeof(value0F00));
    if (0x5A == value0F00[0])
    {
        WriteDGUS(DHW_0F00, cleanData, sizeof(cleanData));
        return 1;
    }
    return 0;
}
/*****************************************************************************
串口初次打印*/
void InitUartSend(void)
{
    Uart2SendStr(CUart2, sizeof(CUart2));
    //  Uart3SendStr(CUart3,sizeof(CUart3));
    //  Uart4SendStr(CUart4,sizeof(CUart4));
    //  Uart5SendStr(CUart5,sizeof(CUart5));
}
/*****************************************************************************
跳转指定页面*/
void JumpPage(uint16_t pageId)
{
    uint8_t temp[4] = {0x5A, 0x01, 0, 0};
    temp[2]         = (uint8_t)(pageId >> 8);
    temp[3]         = pageId;
    WriteDGUS(DHW_SPAGE, temp, sizeof(temp));
    do
    {
        DelayMs(5);
        ReadDGUS(DHW_SPAGE, temp, 1);
    } while (temp[0] != 0);
}

/*****************************************************************************
按键按压之后处理流程*/
void TouchHandle(void)
{
    uint8_t temp[16] = {0};
    uint16_t flag    = 0;
    uint16_t iconid  = 0;
    uint16_t getDar  = 0;
    getDar           = value0F00[1] << 8 | value0F00[2];
    LOG((uint8_t *)&getDar, sizeof(getDar));

    s_dl_t_645_8283_body.st_byte1 = 0x5A;
    s_dl_t_645_8283_body.st_byte2 = 0xA5;
    //	s_dl_t_645_8283_body.st_byte1=0xAA;
    //	s_dl_t_645_8283_body.st_byte2=0xBB;
    s_dl_t_645_8283_body.lenth    = value0F00[3] * 2 + 4;
    s_dl_t_645_8283_body.func     = 0x83;
    s_dl_t_645_8283_body.addr1    = value0F00[1];
    s_dl_t_645_8283_body.addr2    = value0F00[2];
    s_dl_t_645_8283_body.edata[0] = value0F00[3];
    ReadDGUS(getDar, (uint8_t *)&s_dl_t_645_8283_body.edata[1], value0F00[3] * 2);
    Uart2SendStr((uint8_t *)&s_dl_t_645_8283_body, s_dl_t_645_8283_body.lenth + 3);

    //	switch(getDar)
    //	{
    //		default:
    //		break;
    //	}
}

/*****************************************************************************
功能：回传数据*/
void dlt645_handle(void)
{
    uint16_t ADDR = 0;

    ADDR = ((uint16_t)(s_dl_t_645_8283_body.addr1)) << 8 | s_dl_t_645_8283_body.addr2;

    if (s_dl_t_645_8283_body.func == 0x82)
    {
        WriteDGUS(ADDR, (uint8_t *)&(s_dl_t_645_8283_body.edata), s_dl_t_645_8283_body.lenth - 3);
        s_dl_t_645_8283_body.lenth = 3;
        s_dl_t_645_8283_body.addr1 = 0x4F;
        s_dl_t_645_8283_body.addr2 = 0x4B;
        Uart2SendStr((uint8_t *)&s_dl_t_645_8283_body, s_dl_t_645_8283_body.lenth + 3);
    }
    else if (s_dl_t_645_8283_body.func == 0x83)
    {
        ReadDGUS(ADDR, (uint8_t *)&(s_dl_t_645_8283_body.edata[1]), s_dl_t_645_8283_body.edata[0] * 2);
        s_dl_t_645_8283_body.lenth += s_dl_t_645_8283_body.edata[0] * 2;
        Uart2SendStr((uint8_t *)&s_dl_t_645_8283_body, s_dl_t_645_8283_body.lenth + 3);
    }
}
/*****************************************************************************
功能:检索一帧数据将值赋给结构体*/
uint8_t parse_dlt645_frame(uint8_t *p_frame, uint16_t frame_len, uint8_t *sframe_body)
{
    uint16_t temp16_t = 0;  //一帧数据的总长度
//    uint16_t i        = 0;

    if (1)
    {
        memcpy(sframe_body, p_frame, frame_len);
        return 1;
    }
//    return 0;
}
/*****************************************************************************
功能:从缓存区buf中检索dlt645帧数据*/
/*
 * 将一帧数据读取到frame_buf中
 * line:缓存区0x7F开头的数据
 * out:将捡出来的帧复制到该数组中
 * frame_len:捡出来的帧的长度，
 * line_len:缓存区buf中0x7F开头的数据长度
 **/
frame_result_t find_dlt645_frame(uint8_t *line, uint8_t *out, uint16_t *frame_len, uint16_t line_len)
{
    uint16_t frame_length = 0;  //一帧数据的总长度
    uint16_t temp_len     = 0;

    if (line_len < DLT_645_LEAST_LEN)
    {
        return UNFINISHED;
    }
    //判断第0位
    if (line[0] != 0x5A)
    {
        return ERROR;
    }
    if (line[1] != 0xA5)
    {
        return ERROR;
    }
    frame_length = line[2] + 3;

    if (frame_length <= line_len)
    {
        //检到一帧数据
        for (temp_len = 0; temp_len < frame_length; temp_len++)
        {
            out[temp_len] = *line;
            line++;
        }
        *frame_len = frame_length;
        return OK;
    }
    else
    {
        //数据还没接收完整
        return UNFINISHED;
    }
}

/*****************************************************************************
功能:协议数据解析*/
void ParseBuffer(void)
{
    uint16_t frame_length = 0;  //一帧数据的总长度
    uint16_t i = 0, temp_len = 0;
    uint8_t has_content          = 0;  // buf中是否有数据
    uint8_t frame_error          = 0;  //缓存区当前的数据对所有协议都不满足
    uint8_t *p_buf               = Uart2_Rx;
    protocol_type_t protl_type   = PROTOCOL_UNKNOWN;
    frame_result_t find_frame_re = UNKNOWN;

    //用来保存每个协议解析后的结果
    // frame_results[0] 保存PROTOCOL_DL_T_645协议解析结果
    // frame_results[1] 保存PROTOCOL_DL_T_698协议解析结果
    frame_result_t frame_results[2] = {UNKNOWN, UNKNOWN};

    has_content = uart2_rx_count > 2;
    while (has_content)
    {
        p_buf = Uart2_Rx;
        //检索0x5A开头的数据
        while (*p_buf != 0x5A && p_buf < Uart2_Rx + uart2_rx_count)
        {
            p_buf++;
        }
        if (p_buf == Uart2_Rx + uart2_rx_count)
        {
            //检索当前包数据，都不包含，清空
            uart2_rx_count = 0;
            break;
        }
        // Uart4_Rx中剩余的数据长度
        temp_len = uart2_rx_count - (p_buf - Uart2_Rx);

        //以下处理不包含校验
        switch (protl_type)
        {
            case PROTOCOL_UNKNOWN:
                memset(frame_buf, 0, sizeof(frame_buf));
                find_frame_re = UNKNOWN;
                frame_error   = 0;
                frame_length  = 0;
                for (i = 0; i < 3; i++)
                {
                    frame_results[i] = UNKNOWN;
                }

            case PROTOCOL_DL_T_645:
                find_frame_re = find_dlt645_frame(p_buf, frame_buf, &frame_length, temp_len);

                frame_results[0] = find_frame_re;
                if (find_frame_re == OK)
                {
                    if (parse_dlt645_frame(frame_buf, frame_length, (uint8_t *)&s_dl_t_645_8283_body))
                    {
                        //解析到一包有效数据
                        dlt645_handle();
                        memset(&s_dl_t_645_8283_body, 0, sizeof(dl_t_645_8283_body));
                    }
                    break;
                }

            case PROTOCOL_OTHER:
                //此处添加其他协议解析
                // break;

            default:
                if (frame_results[0] == ERROR)
                {
                    //缓存区的数据不满足现有协议的解析
                    //继续找下一个0x7F起始符
                    p_buf++;  //跳过当前的0x7F
                    //检索0x7F开头的数据
                    while (*p_buf != 0x5A && p_buf < Uart2_Rx + uart2_rx_count)
                    {
                        p_buf++;
                    }

                    if (p_buf == Uart2_Rx + uart2_rx_count)
                    {
                        //检索当前包数据，都不包含，清空
                        uart2_rx_count = 0;
                        break;
                    }

                    //找到下一条0x5A开头的数据帧
                    frame_error = 1;
                }
                break;
        }

        //当成功检索到一帧数据或缓存区的数据不满足现有协议的解析
        // buf中剩余的有效数据前移
        if (find_frame_re == OK || frame_error)
        {
            // uart_rcvd_buf剩余的数据长度
            temp_len = uart2_rx_count - (p_buf - Uart2_Rx) - frame_length;
            if (temp_len > 0)
            {
                //当前uart_rcvd_buf中剩余的数据前移
                for (i = 0; i < temp_len; i++)
                {
                    Uart2_Rx[i]                 = *(p_buf + frame_length + i);
                    *(p_buf + frame_length + i) = 0x00;
                }
                has_content = 1;  //继续循环解析
            }
            else
            {
                //解析过的位清空
                for (i = 0; i < (p_buf - Uart2_Rx) + frame_length; i++)
                {
                    Uart2_Rx[i] = 0x00;
                }
                has_content = 0;
            }
            uart2_rx_count = temp_len;
        }
        else
        {
            has_content = 0;
        }
    }
}

/*****************************************************************************
主流程*/
void HandleProc(void)
{
    if (GetValue0F00())
    {
        TouchHandle();
    }
    ParseBuffer();
}
