
#include "password.h"
//#include "dgus.h"
#include "handle.h"
//#include "strcmp.h"

int incflag               = 1;
int incflag0              = 1;
int incflag1              = 1;
uint8_t password1[8]      = {0};
uint8_t password2[8]      = {0};
uint8_t password3[8]      = {0};
uint8_t password4[8]      = {0};  //临时存放密码
uint8_t Vp_Clear[8]       = {0};  //清空密码框
uint8_t Temp_Addr[8]      = {0};
int8_t Temp_Addr1[4]      = {0};
uint8_t Temp_Addr2[2]     = {0};
uint8_t Temp_Addr3[2]     = {0};
uint8_t page[2]           = {0};
uint8_t page1[2]          = {0};
uint8_t page2[2]          = {0};
uint8_t Super_Password[8] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};  //原始密码：12345678
uint8_t zuobiao[]         = {0x5A, 0xA5, 0x00, 0x04, 0x00, 0xF0, 0x00, 0xB4};
uint8_t zuobiao1[]        = {0x5A, 0xA5, 0x00, 0x04, 0x00, 0xF0, 0x00, 0x90};
uint8_t zuobiao2[]        = {0x5A, 0xA5, 0x00, 0x04, 0x00, 0xF0, 0x00, 0xC8};
uint8_t Read_Flash[8]     = {0x5A, 0x00, 0x10, 0x08, 0x10, 0xA0, 0x00, 0x04};  //数据库的读指令
uint8_t Write_Flash[8]    = {0xA5, 0x00, 0x10, 0x08, 0x60, 0x10, 0x00, 0x04};  //数据库的写指令

// uint8_t Read_save_flag_Flash[8]  = {0x5A, 0x00, 0x20, 0x08, 0x30, 0xA0, 0x00, 0x02};  //数据库的读指令
// uint8_t Write_save_flag_Flash[8] = {0xA5, 0x00, 0x20, 0x08, 0x30, 0xA0, 0x00, 0x02};  //数据库的写指令
uint8_t Read_save_Flash[8]  = {0x5A, 0x00, 0x20, 0x10, 0x20, 0x06, 0x00, 0x06};  //数据库的读指令
uint8_t Write_save_Flash[8] = {0xA5, 0x00, 0x20, 0x10, 0x20, 0x06, 0x00, 0x06};  //数据库的写指令
uint8_t flash_flag[2]       = {0};
uint8_t save_temp[12]       = {0};
uint8_t save_temp1[12]      = {0};
uint8_t save_temp2[12]      = {0};

uint8_t Init_Data[10] = {0x00, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x11};
uint8_t stop_open[9]  = {0x5A, 0xA5, 0x06, 0x83, 0x20, 0x05, 0x01, 0x00, 0x00};
uint8_t stop_open1[9] = {0x5A, 0xA5, 0x06, 0x83, 0x20, 0x07, 0x01, 0x00, 0x00};
// uint8_t JUMP_PAGE[]={0x5A,0x01,0x00,0x00};

/***********密码登陆***************/

void password(void)
{
    uint8_t i = 0;
    ReadDGUS(DHW_NPAGE, page, sizeof(page));
    if (page[1] == 0x06)
    {
        if (incflag)
        {
            WriteDGUS(DHW_MONI, zuobiao, sizeof(zuobiao));
            incflag = 0;
        }
        ReadDGUS(PSW_LOGIN, password1, sizeof(password1));

        for (i = 0; i < 8; i++)
        {
            if (password1[i] == 0)
            {
                //如果密码为空，则什么都不做
                continue;
            }
            else
            {
                //将数据库1008地址的密码读到10A0变量地址中做对比
                // ReadNorFlash(0x000010, 0x10A0, 4);
                // ReadDGUS(0x10A0, password2, 8);
                //
                //	WriteDGUS(0x6000,0x10A0,8);

                WriteDGUS(0x0008, Read_Flash, sizeof(Read_Flash));
                do
                {  //判断数据库读操作是否完成
                    ReadDGUS(0x0008, Temp_Addr, 2);
                } while (Temp_Addr[0] == 0x5A);
                DelayMs(100);
                ReadDGUS(0x10A0, password2, 8);

                if (char_cmp(password2, password1, 8) == 0)
                {
                    //如果密码一样,切页到密码正确页面,清空密码框
                    JumpPage(4);
                    WriteDGUS(PSW_LOGIN, Vp_Clear, sizeof(Vp_Clear));
                    DelayMs(500);
                    incflag = 1;
                    return;
                    //如果密码不一样，比较是否原始密码
                }
                else if (char_cmp(password1, Super_Password, 8) == 0)
                {
                    //如果密码一样,切页到密码正确页面,清空密码框
                    JumpPage(4);
                    WriteDGUS(PSW_LOGIN, Vp_Clear, sizeof(Vp_Clear));
                    DelayMs(500);
                    incflag = 1;
                    return;
                }
                else
                {
                    //如果密码不一样,切页到密码错误页面,清空密码框
                    JumpPage(10);
                    WriteDGUS(PSW_LOGIN, Vp_Clear, sizeof(Vp_Clear));
                    DelayMs(2000);
                    JumpPage(2);

                    incflag = 1;
                    return;
                }
            }
        }
    }
    else
    {
        incflag = 1;
    }
}

/**********修改密码****************/

void reconfirm_password(void)
{
    uint8_t i = 0;
    uint8_t k = 0;
    //	uint8_t page[2]={0};
    ReadDGUS(DHW_NPAGE, page, sizeof(page));
    if (page[1] == 0x08)
    {
        if (incflag0)
        {
            WriteDGUS(CHARGE_PASSWORD, Vp_Clear, sizeof(Vp_Clear));
            WriteDGUS(DHW_MONI, zuobiao1, sizeof(zuobiao1));
            incflag0 = 0;
        }
        ReadDGUS(CHARGE_PASSWORD, password3, sizeof(password3));
        for (i = 0; i < 8; i++)
        {
            if (password3[i] == 0)
            {
                //如果密码为空，则什么都不做
                continue;
            }
            else
            {
                if (incflag1)
                {
                    WriteDGUS(DHW_MONI, zuobiao2, sizeof(zuobiao2));
                    incflag1 = 0;
                }
                ReadDGUS(RECONFIRM_PASSWORD, password4, sizeof(password4));
                for (k = 0; k < 8; k++)
                {
                    if (password4[k] == 0)
                    {
                        //如果密码为空，则什么都不做
                        continue;
                    }
                    else if (char_cmp(password3, password4, 8) == 0)
                    {
                        WriteDGUS(0x6010, password3, 8);
                        DelayMs(20);
                        WriteDGUS(0x0008, Write_Flash, sizeof(Write_Flash));
                        do
                        {  //判断数据库写操作是否完成
                            ReadDGUS(0x0008, Temp_Addr, 2);
                        } while (Temp_Addr[0] == 0xA5);
                        DelayMs(100);

                        //如果密码一样,切页到密码正确页面,清空密码框

                        JumpPage(11);
                        WriteDGUS(CHARGE_PASSWORD, Vp_Clear, sizeof(Vp_Clear));
                        WriteDGUS(RECONFIRM_PASSWORD, Vp_Clear, sizeof(Vp_Clear));
                        DelayMs(1800);
                        JumpPage(4);
                        incflag0 = 1;
                        incflag1 = 1;
                        return;
                    }
                    else
                    {
                        //如果密码不一样,切页到密码错误页面,清空密码框
                        JumpPage(12);
                        DelayMs(1800);
                        WriteDGUS(CHARGE_PASSWORD, Vp_Clear, sizeof(Vp_Clear));
                        WriteDGUS(RECONFIRM_PASSWORD, Vp_Clear, sizeof(Vp_Clear));

                        JumpPage(8);
                        incflag0 = 1;
                        incflag1 = 1;
                        return;
                    }
                }

                return;
            }
        }
    }
    else
    {
        incflag0 = 1;
        incflag1 = 1;
    }
}

void ctr_cmd(void)
{
    ReadDGUS(0x2010, Temp_Addr1, 2);  // 00 00~00 01

    if (Temp_Addr1[1] != Temp_Addr1[3])
    {
        Temp_Addr1[3] = Temp_Addr1[1];
        SetVoice(Temp_Addr1[1]);
    }
    ReadDGUS(0x2005, Temp_Addr2, 2);
    if (Temp_Addr2[1] == 0)
    {
        ctr_inc = 0;
    }
    else if (ctr_inc > 120000)
    {
        Temp_Addr2[1] = 0;
        WriteDGUS(0x2005, Temp_Addr2, 2);
        Uart2SendStr(stop_open, sizeof(stop_open));
    }  // else
    //{return;}
    ReadDGUS(0x2007, Temp_Addr3, 2);
    if (Temp_Addr3[1] == 0)
    {
        ctr1_inc = 0;
    }
    else if (ctr1_inc > 10000)
    {
        Temp_Addr3[1] = 0;
        WriteDGUS(0x2007, Temp_Addr3, 2);
        Uart2SendStr(stop_open1, sizeof(stop_open1));
    }
}

/**掉电保存***/

void Init_Save(void)
{
    //	uint8_t Init_Data[] = {0x00,0x17,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x11};
    // 0x30A0=flash_flag->0x002008
    WriteDGUS(0x0008, Read_save_Flash, sizeof(Read_save_Flash));
    do
    {  //判断数据库读操作是否完成
        ReadDGUS(0x0008, Temp_Addr, 2);
    } while (Temp_Addr[0] == 0x5A);
    DelayMs(100);
    ReadDGUS(0x200B, flash_flag, 2);
    //
    // flash_flag[1] = 0;
    //
    if (flash_flag[0] != 0 || flash_flag[1] != 0)  //新屏幕
    {
        flash_flag[0] = 0x00;
        flash_flag[1] = 0x00;
        WriteDGUS(0x200B, flash_flag, 2);
        WriteDGUS(0x2006, Init_Data, 10);
        // DelayMs(20);
        // WriteDGUS(0x0008, Write_save_Flash, sizeof(Write_save_Flash));
        // do
        // {  //判断数据库读操作是否完成
        //     ReadDGUS(0x0008, Temp_Addr, 2);
        // } while (Temp_Addr[0] == 0xA5);
        // DelayMs(100);
    }
    // else  //旧屏幕
    // {
    //     // 0x2006->0x002010
    //     WriteDGUS(0x0008, Read_save_Flash, sizeof(Read_save_Flash));
    //     do
    //     {  //判断数据库读操作是否完成
    //         ReadDGUS(0x0008, Temp_Addr, 2);
    //     } while (Temp_Addr[0] == 0x5A);
    //     DelayMs(100);
    //     ReadDGUS(0x2006, save_temp, 10);
    // }

    ReadDGUS(0x2006, save_temp, 10);
}

void Save_cmd(void)
{
    int i = 0;
    ReadDGUS(0x2006, save_temp1, 10);
    if (char_cmp(save_temp1, save_temp, 10) != 0)  //有改变
    {
        ReadDGUS(DHW_NPAGE, page, sizeof(page));
        if (page[1] != 0x04)
        {
            for (i = 0; i < 10; i++)
            {
                save_temp[i] = save_temp1[i];
            }
            DelayMs(20);
            WriteDGUS(0x0008, Write_save_Flash, sizeof(Write_save_Flash));
            do
            {  //判断数据库读操作是否完成
                ReadDGUS(0x0008, Temp_Addr, 2);
            } while (Temp_Addr[0] == 0xA5);
            DelayMs(100);
        }
    }
    else  //无改变
    {
        return;
    }
}

void Nop_cmd(void)
{
    ReadDGUS(DHW_NPAGE, page, sizeof(page));
    if (page[1] == 0x04)
    {
        ReadDGUS(0x0016, page2, sizeof(page2));
        if (page2[0] == 0x5A)
        {
            ctr2_inc = 0;
            page2[0] = 0;
            WriteDGUS(0x0016, page2, sizeof(page2));
        }
        else
        {
            if (ctr2_inc > 120000)
            {
                JumpPage(0);
            }
        }
    }
    else
    {
        ctr2_inc = 0;
        return;
    }
}
