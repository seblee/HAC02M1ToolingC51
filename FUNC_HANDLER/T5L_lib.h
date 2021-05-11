#ifndef __T5L_LIB_H_
#define __T5L_LIB_H_
/**
 ****************************************************************************
 * @Warning Without permission from the author,Not for commercial use
 * @File
 * @Author  xiaowine@cee0.com
 * @date
 * @version V1.0
 *************************************************
 * @brief   -->>
 ****************************************************************************
 * @attention
 * Powered By Xiaowine
 * <h2><center>&copy;  Copyright(C) cee0.com 2015-2019</center></h2>
 * All rights reserved
 *
 **/

#define READRFLASH 0x5A
#define WRITERFLASH 0xA5

void wait_ok(unsigned int addr);
void T5L_Flash(unsigned char mod, unsigned int addr, long addr_flash, unsigned int len);
void read_dgusii_vp(unsigned int addr, unsigned char *buf, unsigned int len);
void write_dgusii_vp(unsigned int addr, unsigned char *buf, unsigned int len);
void wdt_on(void);
void wdt_off(void);
void wdt_rst(void);
void initcpu(void);

#endif
