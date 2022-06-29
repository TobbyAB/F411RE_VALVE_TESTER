/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-02     Gwvt       the first version
 */
#include "my_lcd_u8g2.h"

u8g2_t u8g2;

void my_lcd_init(){
    u8g2_Setup_st7920_s_128x64_f(&u8g2, U8G2_R0, u8x8_byte_rtthread_4wire_hw_spi, u8x8_gpio_and_delay_rtthread);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2),U8X8_PIN_CS,28);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2),U8X8_PIN_RESET,30);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
}

void u8g2_xs_time(uint8_t x,uint8_t y,uint8_t sz){
    char data[5] = "     ";
    if(sz >= 10)data[0] = sz/10%10 + '0';
    data[1] = sz%10 + '0';
    data[2] = ' ';
    data[3] = 'S';
    data[4] = 0;
    u8g2_DrawStr(&u8g2, x, y+12, data);
}
void u8g2_xs_max(uint8_t x,uint8_t y,uint8_t sz){
    char data[6] = "      ";
    data[0] = sz/10%10 + '0';
    data[1] = '.';
    data[2] = sz%10 + '0';
    data[3] = ' ';
    data[4] = 'V';
    data[5] = 0;
    u8g2_DrawStr(&u8g2, x, y+12, data);
}

void u8g2_xs_switch(uint8_t x,uint8_t y,uint8_t c1,uint8_t c2){
    if(c1 == 0)u8g2_DrawStr(&u8g2, x, y+12, "ch2");
    else if(c2 == 0)u8g2_DrawStr(&u8g2, x, y+12, "ch1");
    else u8g2_DrawStr(&u8g2, x, y+12, "ch1+ch2");
}
void u8g2_xszf(uint8_t x,uint8_t y,char *data){
    u8g2_DrawStr(&u8g2, x, y+12, data);
}

void u8g2_my_ClearBuffer(){
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_lastapprenticebold_tr);
}
void u8g2_my_SendBuffer(){
    u8g2_SendBuffer(&u8g2);
}
