
#ifndef _PASSWORD_H_
#define _PASSWORD_H_

#define PSW_LOGIN 0x1100           //密码登录
#define CHARGE_PASSWORD 0x1110     //密码登录
#define RECONFIRM_PASSWORD 0x1120  //密码登录

extern unsigned long ctr_inc;
extern unsigned int ctr1_inc;
extern unsigned long ctr2_inc;

extern void password();
extern void reconfirm_password();
extern void ctr_cmd();
extern void Init_Save();
extern void Save_cmd();
extern void Nop_cmd();

#endif
