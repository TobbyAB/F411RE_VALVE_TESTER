/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-02     Gwvt       the first version
 */
#ifndef APPLICATIONS_MY_LCD_U8G2_H_
#define APPLICATIONS_MY_LCD_U8G2_H_

#include <rtthread.h>
#include <rtdbg.h>
#include "rtdevice.h"
#include <u8g2_port.h>

void my_lcd_init();
void u8g2_my_ClearBuffer();
void u8g2_my_SendBuffer();
void u8g2_xszf(uint8_t x,uint8_t y,char *data);
void u8g2_xs_time(uint8_t x,uint8_t y,uint8_t sz);
void u8g2_xs_max(uint8_t x,uint8_t y,uint8_t sz);
void u8g2_xs_switch(uint8_t x,uint8_t y,uint8_t c1,uint8_t c2);
#endif /* APPLICATIONS_MY_LCD_U8G2_H_ */
