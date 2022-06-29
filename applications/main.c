/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-02     RT-Thread    first version
 */

#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "rtdevice.h"
#include "my_lcd_u8g2.h"
#include "my_adc_read.h"
#include "my_flash.h"

#define OPEN1       0x0A
#define CLOSED1     0x14
#define HALL1       0X16
#define RELAY1      0x13

#define OPEN2       0x02
#define CLOSED2     0x22
#define HALL2       0x20
#define RELAY2      0x09

#define BUTTON1     0x10
#define BUTTON2     0x11
#define BUTTON3     0x12
#define BUTTON4     0x1a

#define BUTTON_VALVE1 0x00
#define BUTTON_VALVE2 0x24

#define RGB_R1      0x15
#define RGB_G1      0x17

#define RGB_G2      0x01
#define RGB_R2      0x03

#define BUZZ        0x2d

uint8_t time_counter = 0;
uint8_t choose = 0;
uint8_t button_self_locking = 0;

uint8_t process = 0;

uint8_t switch_time = 10;
uint8_t battery_time = 10;
uint8_t close_time = 3;

uint16_t threshold = 1520;

uint16_t test_time1 = 0;
uint16_t test_time2 = 0;

uint8_t test_flag1 = 0;
uint8_t test_flag2 = 0;

uint8_t hall1_flag = 0;
uint8_t hall2_flag = 0;

uint8_t valve1_flag = 0;
uint8_t valve2_flag = 0;

uint8_t ch1 = 0;
uint8_t ch2 = 0;

uint8_t cap_number = 10;

//void hall_low1(void *args)
//{
//    rt_thread_mdelay(4);
//    if(rt_pin_read(HALL1) == PIN_LOW){
//        if(ch1 == 3 && valve1_flag == 1)valve1_flag = 2;
//    }
//}
//void hall_low2(void *args)
//{
//    rt_thread_mdelay(4);
//    if(rt_pin_read(HALL2) == PIN_LOW){
//        if(ch2 == 3 && valve2_flag == 1)valve2_flag = 2;
//    }
//}

void my_io_init(){
    rt_pin_mode(OPEN1, PIN_MODE_INPUT);
    rt_pin_mode(CLOSED1, PIN_MODE_INPUT);
    rt_pin_mode(HALL1, PIN_MODE_INPUT);
    rt_pin_mode(RELAY1, PIN_MODE_OUTPUT);

    rt_pin_mode(OPEN2, PIN_MODE_INPUT);
    rt_pin_mode(CLOSED2, PIN_MODE_INPUT);
    rt_pin_mode(HALL2, PIN_MODE_INPUT);
    rt_pin_mode(RELAY2, PIN_MODE_OUTPUT);

    rt_pin_mode(BUTTON1, PIN_MODE_INPUT);
    rt_pin_mode(BUTTON2, PIN_MODE_INPUT);
    rt_pin_mode(BUTTON3, PIN_MODE_INPUT);
    rt_pin_mode(BUTTON4, PIN_MODE_INPUT);

    rt_pin_mode(BUTTON_VALVE1, PIN_MODE_INPUT);
    rt_pin_mode(BUTTON_VALVE2, PIN_MODE_INPUT);

    rt_pin_mode(RGB_R1, PIN_MODE_OUTPUT);
    rt_pin_mode(RGB_G1, PIN_MODE_OUTPUT);
    rt_pin_mode(RGB_R2, PIN_MODE_OUTPUT);
    rt_pin_mode(RGB_G2, PIN_MODE_OUTPUT);

    rt_pin_mode(BUZZ, PIN_MODE_OUTPUT);

    rt_pin_write(RELAY1, PIN_LOW);
    rt_pin_write(RELAY2, PIN_LOW);

//    rt_pin_mode(HALL1, PIN_MODE_INPUT_PULLUP  );
//    rt_pin_attach_irq(HALL1, PIN_IRQ_MODE_FALLING , hall_low1, RT_NULL);
//    rt_pin_irq_enable(HALL1, PIN_IRQ_ENABLE);
//
//    rt_pin_mode(HALL2, PIN_MODE_INPUT_PULLUP  );
//    rt_pin_attach_irq(HALL2, PIN_IRQ_MODE_FALLING , hall_low2, RT_NULL);
//    rt_pin_irq_enable(HALL2, PIN_IRQ_ENABLE);


    my_adc_init();
}

uint8_t my_button_mode(){
        if(rt_pin_read(BUTTON1) == 0){
            if(button_self_locking == 0){
                button_self_locking = 1;
                return 1;
            }
        }else if(rt_pin_read(BUTTON2) == 0){
            if(button_self_locking == 0){
                button_self_locking = 1;
                return 2;
            }
        }else if(rt_pin_read(BUTTON3) == 0){
            if(button_self_locking == 0){
                button_self_locking = 1;
                return 3;
            }
        }else if(rt_pin_read(BUTTON4) == 0){
            if(button_self_locking == 0){
                button_self_locking = 1;
                return 4;
            }
        }else button_self_locking = 0;

    return 0;
}
void my_lcd_process(uint8_t select){
    static uint8_t sign = 0;
    uint8_t i;
    u8g2_my_ClearBuffer();
    switch(select){
        case 0:
            u8g2_xszf(44, 28, "Start");break;
        case 5:
            if(choose != 0 || time_counter < 5)u8g2_xszf(0, 0, "Valve  Time:");
//            if(choose != 1 || time_counter < 5)u8g2_xszf(0, 17,"Battery Time:");
//            if(choose != 2 || time_counter < 5)u8g2_xszf(0, 34, "Battry Max:");
            if(choose != 1 || time_counter < 5)u8g2_xszf(0, 17,"Charge Time:");
            if(choose != 2 || time_counter < 5)u8g2_xszf(0, 34, "Charge Max:");
            if(choose != 3 || time_counter < 5)u8g2_xszf(0, 51, "Close  Time:");


            if((choose != 0 && choose != 10) || time_counter < 5)u8g2_xs_time(80, 0, switch_time);
            if((choose != 1 && choose != 11) || time_counter < 5)u8g2_xs_time(80, 17, battery_time);
            if((choose != 2 && choose != 12) || time_counter < 5)u8g2_xs_max(80,34,cap_number);
            if((choose != 3 && choose != 13) || time_counter < 5)u8g2_xs_time(80,51,close_time);

            break;

        case 6:
            if(ch1 > 0){
                u8g2_xszf(0, 0, "OPEN:");
                u8g2_xszf(0, 17, "CAPA:");
                u8g2_xszf(0, 34, "CLOS:");
                u8g2_xszf(0, 51, "HALL:");
                if(ch1 < 3){
                    if(test_flag1 & 0x01)u8g2_xszf(36, 0, "OK");
                    else if(test_flag1 & 0x02)u8g2_xszf(36, 0, "ERR");
                    else {
                        for(i=0;i<3;i++){
                            if(i < (sign/5))u8g2_xszf(36+i*5, 0, ".");
                        }
                    }
                    if(test_flag1 & 0x04)u8g2_xszf(36, 17, "OK");
                    else if(test_flag1 & 0x08)u8g2_xszf(36, 17, "ERR");
                    else {
                        for(i=0;i<3;i++){
                            if(i < (sign/5))u8g2_xszf(36+i*5, 17, ".");
                        }
                    }
                }else if(ch1 == 3){
                    if(test_flag1 & 0x01)u8g2_xszf(36, 0, "OK");
                    else u8g2_xszf(36, 0, "ERR");
                    if(test_flag1 & 0x04)u8g2_xszf(36, 17, "OK");
                    else u8g2_xszf(36, 17, "ERR");

                    if(test_flag1 & 0x10)u8g2_xszf(36, 34, "OK");
                    else if(test_flag1 & 0x20)u8g2_xszf(36, 34, "ERR");
                    else {
                        for(i=0;i<3;i++){
                            if(i < (sign/5))u8g2_xszf(36+i*5, 34, ".");
                        }
                    }

                    if(test_flag1 & 0x40)u8g2_xszf(36, 51, "OK");
                    else if(test_flag1 & 0x80)u8g2_xszf(36, 51, "ERR");
                    else {
                        for(i=0;i<3;i++){
                            if(i < (sign/5))u8g2_xszf(36+i*5, 51, ".");
                        }
                    }
                }else {
                    if(test_flag1 & 0x01)u8g2_xszf(36, 0, "OK");
                    else if(test_flag1 & 0x02)u8g2_xszf(36, 0, "ERR");
                    if(test_flag1 & 0x04)u8g2_xszf(36, 17, "OK");
                    else if(test_flag1 & 0x08)u8g2_xszf(36, 17, "ERR");
                    if(test_flag1 & 0x10)u8g2_xszf(36, 34, "OK");
                    else if(test_flag1 & 0x20)u8g2_xszf(36, 34, "ERR");
                    if(test_flag1 & 0x40)u8g2_xszf(36, 51, "OK");
                    else if(test_flag1 & 0x80)u8g2_xszf(36, 51, "ERR");
                }
            }

            if(ch2 > 0){
                u8g2_xszf(64, 0, "OPEN:");
                u8g2_xszf(64, 17, "CAPA:");
                u8g2_xszf(64, 34, "CLOS:");
                u8g2_xszf(64, 51, "HALL:");

                if(ch2 < 3){
                    if(test_flag2 & 0x01)u8g2_xszf(100, 0, "OK");
                    else if(test_flag2 & 0x02)u8g2_xszf(100, 0, "ERR");
                    else {
                        for(i=0;i<3;i++){
                            if(i < (sign/5))u8g2_xszf(100+i*5, 0, ".");
                        }
                    }
                    if(test_flag2 & 0x04)u8g2_xszf(100, 16, "OK");
                    else if(test_flag2 & 0x08)u8g2_xszf(100, 17, "ERR");
                    else {
                        for(i=0;i<3;i++){
                            if(i < (sign/5))u8g2_xszf(100+i*5, 17, ".");
                        }
                    }
                }else if(ch2 == 3){
                    if(test_flag2 & 0x01)u8g2_xszf(100, 0, "OK");
                    else u8g2_xszf(100, 0, "ERR");
                    if(test_flag2 & 0x04)u8g2_xszf(100, 17, "OK");
                    else u8g2_xszf(100, 17, "ERR");

                    if(test_flag2 & 0x10)u8g2_xszf(100, 34, "OK");
                    else if(test_flag2 & 0x20)u8g2_xszf(100, 34, "ERR");
                    else {
                        for(i=0;i<3;i++){
                            if(i < (sign/5))u8g2_xszf(100+i*5, 34, ".");
                        }
                    }
                    if(test_flag2 & 0x40)u8g2_xszf(100, 51, "OK");
                    else if(test_flag2 & 0x80)u8g2_xszf(100, 51, "ERR");
                    else {
                        for(i=0;i<3;i++){
                            if(i < (sign/5))u8g2_xszf(100+i*5, 51, ".");
                        }
                    }
                }else {
                    if(test_flag2 & 0x01)u8g2_xszf(100, 0, "OK");
                    else if(test_flag2 & 0x02)u8g2_xszf(100, 0, "ERR");
                    if(test_flag2 & 0x04)u8g2_xszf(100, 16, "OK");
                    else if(test_flag2 & 0x08)u8g2_xszf(100, 17, "ERR");
                    if(test_flag2 & 0x10)u8g2_xszf(100, 34, "OK");
                    else if(test_flag2 & 0x20)u8g2_xszf(100, 34, "ERR");
                    if(test_flag2 & 0x40)u8g2_xszf(100, 51, "OK");
                    else if(test_flag2 & 0x80)u8g2_xszf(100, 51, "ERR");
                }
            }

            sign += 1;
            sign %= 20;
            break;

    }
    u8g2_my_SendBuffer();
}

void valve1_thread(){
    static uint16_t valve1_time1 = 0;
    static uint16_t valve1_time2 = 0;

    switch(ch1){
        case 0:
            break;
        case 1:
            valve1_time1 = switch_time * 10;
            valve1_time2 = battery_time* 10;
            test_flag1 = 0x00;
            ch1 = 2;
            rt_pin_write(RGB_R1, PIN_HIGH);
            rt_pin_write(RGB_G1, PIN_HIGH);
            rt_pin_write(RELAY1, PIN_HIGH);
            rt_thread_mdelay(100);
            break;
        case 2:
            if(valve1_time1)valve1_time1--;
            if(valve1_time2)valve1_time2--;

            if((test_flag1 & 0x03) == 0){
                if(valve1_time1 == 0){
                    test_flag1 |= 0x02;
                    ch1 = 4;
                }
                if(rt_pin_read(OPEN1) == 1){
                    test_flag1 |= 0x01;
                    valve1_time1 = 0;
                }
            }
            if((test_flag1 & 0x0c) == 0){
                if(valve1_time2 == 0){
                    test_flag1 |= 0x08;
                    ch1 = 4;
                }
                if(read_adc1() >= threshold){
                    test_flag1 |= 0x04;
                    valve1_time2 = 0;
                }
            }
            if(valve1_time1 == 0 && valve1_time2 == 0 && ch1 == 2){
                valve1_time1 = switch_time * 10;
                valve1_flag = 0;
                ch1 = 3;
                if(rt_pin_read(HALL1) == 1)valve1_flag = 1;
                rt_pin_write(RELAY1, PIN_LOW);
                rt_thread_mdelay(100);
            }
            break;

        case 3:
            if(valve1_time1)valve1_time1--;

            if(valve1_flag == 0 && rt_pin_read(HALL1) == 1)valve1_flag = 1;
            if(valve1_flag == 1 && rt_pin_read(HALL1) == 0){
                valve1_flag = 2;
                test_flag1 |= 0x40;
                valve1_time1 = close_time * 10;
            }
//            if(valve1_flag == 2 && rt_pin_read(HALL1) == 1){
//                valve1_flag = 3;
//                test_flag1 |= 0x40;
////                if(valve1_time1 >= (close_time * 10))
//                    valve1_time1 = close_time * 10;
//            }
            if((test_flag1 & 0x30) == 0){
                if(valve1_time1 == 0)test_flag1 |= 0x20;
                if(rt_pin_read(CLOSED1) == 1){
                    test_flag1 |= 0x10;
                    valve1_time1 = 0;
                }
            }
            if(valve1_time1 == 0){
                if(valve1_flag < 2)test_flag1 |= 0x80;
                ch1 = 4;
            }
            break;

        case 4:
            rt_pin_write(RELAY1, PIN_LOW);
            if(ch2 != 5){
                valve1_time1 = 6;
                if(test_flag1 == 0x55){
                    rt_pin_write(RGB_R1, PIN_LOW);
                }
                else {
                    rt_pin_write(RGB_G1, PIN_LOW);
                }
                ch1 = 5;
            }
            break;
        case 5:
            if(valve1_time1)valve1_time1--;
            if(test_flag1 == 0x55){
                rt_pin_write(BUZZ, PIN_HIGH);
            }else {
                rt_pin_write(BUZZ, valve1_time1%2);
            }
            if(valve1_time1 == 0){
                if(test_flag1 == 0x55)ch1 = 6;
                else ch1 = 7;
                rt_pin_write(BUZZ, PIN_LOW);
            }
            break;
        case 6:
            break;
    }
}
void valve2_thread(){
    static uint16_t valve2_time1 = 0;
    static uint16_t valve2_time2 = 0;

    switch(ch2){
        case 0:
            break;
        case 1:
            valve2_time1 = switch_time * 10;
            valve2_time2 = battery_time* 10;
            test_flag2 = 0x00;
            ch2 = 2;
            rt_pin_write(RGB_R2, PIN_HIGH);
            rt_pin_write(RGB_G2, PIN_HIGH);
            rt_pin_write(RELAY2, PIN_HIGH);
            rt_thread_mdelay(100);
            break;
        case 2:

            if(valve2_time1)valve2_time1--;
            if(valve2_time2)valve2_time2--;

            if((test_flag2 & 0x03) == 0){
                if(valve2_time1 == 0){
                    test_flag2 |= 0x02;
                    ch2 = 4;
                }
                if(rt_pin_read(OPEN2) == 1){
                    test_flag2 |= 0x01;
                    valve2_time1 = 0;
                }
            }
            if((test_flag2 & 0x0c) == 0){
                if(valve2_time2 == 0){
                    test_flag2 |= 0x08;
                    ch2 = 4;
                }
                if(read_adc2() >= threshold){
                    test_flag2 |= 0x04;
                    valve2_time2 = 0;
                }
            }
            if(valve2_time1 == 0 && valve2_time2 == 0 && ch2 == 2){
                valve2_time1 = switch_time * 10;
                valve2_flag = 0;
                ch2 = 3;
                if(rt_pin_read(HALL2) == 1)valve2_flag = 1;
                rt_pin_write(RELAY2, PIN_LOW);
                rt_thread_mdelay(100);
            }
            break;

        case 3:
            if(valve2_time1)valve2_time1--;

            if(valve2_flag == 0 && rt_pin_read(HALL2) == 1)valve2_flag = 1;
            if(valve2_flag == 1 && rt_pin_read(HALL2) == 0){
                valve2_flag = 2;
                test_flag2 |= 0x40;
                valve2_time1 = close_time * 10;
            }
//            if(valve2_flag == 2 && rt_pin_read(HALL2) == 1){
//                valve2_flag = 3;
//                test_flag2 |= 0x40;
////                if(valve2_time1 >= (close_time * 10))
//                    valve2_time1 = close_time * 10;
//            }

            if((test_flag2 & 0x30) == 0){
                if(valve2_time1 == 0)test_flag2 |= 0x20;
                if(rt_pin_read(CLOSED2) == 1){
                    test_flag2 |= 0x10;
                    valve2_time1 = 0;
                }
            }
            if(valve2_time1 == 0){
                if(valve2_flag < 2)test_flag2 |= 0x80;
                ch2 = 4;
            }
            break;

        case 4:
            rt_pin_write(RELAY2, PIN_LOW);
            if(ch1 != 5){
                valve2_time1 = 6;
                if(test_flag2 == 0x55){
                    rt_pin_write(RGB_R2, PIN_LOW);
                }
                else {
                    rt_pin_write(RGB_G2, PIN_LOW);
                }
                ch2 = 5;
            }
            break;
        case 5:
            if(valve2_time1)valve2_time1--;
            if(test_flag2 == 0x55){
                rt_pin_write(BUZZ, PIN_HIGH);
            }else {
                rt_pin_write(BUZZ, valve2_time1%2);
            }
            if(valve2_time1 == 0){
                if(test_flag2 == 0x55)ch2 = 6;
                else ch2 = 7;
                rt_pin_write(BUZZ, PIN_LOW);
            }
            break;
        case 6:
            break;
    }
}

void my_process_thread(uint8_t button){

    static uint8_t number = 0;

    switch(process){
        case 0:
            my_lcd_process(0);
            process = 1;
            choose = 0;
            time_counter = 0;
            switch_time = get_device_data(0);
            battery_time = get_device_data(1);
            cap_number = get_device_data(2);
            close_time = get_device_data(3);
            threshold = (cap_number * 307)/10;
            rt_thread_mdelay(2000);
            break;
        case 1:
            if(button == 1){
                if(choose < 10){
                    if(choose)choose -= 1;
                    else choose = 3;
                }else {
                    if(choose == 10){
                        if(switch_time < 99)switch_time ++;
                        else switch_time = 1;
                    }else if(choose == 11){
                        if(battery_time < 99)battery_time ++;
                        else battery_time = 1;
                    }else if(choose == 12){
                        if(cap_number < 50)cap_number++;
                        else cap_number = 1;
                    }else if(choose == 13){
                        if(close_time < 99)close_time++;
                        else close_time = 1;
                    }
                }
                time_counter = 0;
            }
            if(button == 2){
                if(choose < 10){
                    choose += 1;
                    choose %= 4;
                }else {
                    if(choose == 10){
                        if(switch_time > 1)switch_time--;
                        else switch_time = 99;
                    }else if(choose == 11){
                        if(battery_time > 1)battery_time--;
                        else battery_time = 99;
                    }else if(choose == 12){
                        if(cap_number > 1)cap_number--;
                        else cap_number = 50;
                    }else if(choose == 13){
                        if(close_time > 1)close_time--;
                        else close_time = 99;
                    }
                }
                time_counter = 0;
            }

            if(button == 3){
                if(choose < 10){
                    if(choose == 0)number = switch_time;
                    else if(choose == 1)number = battery_time;
                    else if(choose == 2)number = cap_number;
                    else if(choose == 3)number = close_time;
                    choose += 10;
                }else {
                    choose -= 10;
                    set_device_data(switch_time,battery_time,cap_number,close_time,0);
                    threshold = (cap_number * 307)/10;
                }
            }

            if(button == 4){
                if(choose < 10){
                    choose = 0;
                    process = 1;
                }else {
                    if(choose == 10)switch_time = number;
                    else if(choose == 11)battery_time = number;
                    else if(choose == 12)cap_number = number;
                    else if(choose == 13)close_time = number;
                    choose -= 10;
                }
            }
            if(rt_pin_read(BUTTON_VALVE1) == PIN_LOW && choose < 10){
                ch1 = 1;
                process = 2;
                choose = 0;
            }
            if(rt_pin_read(BUTTON_VALVE2) == PIN_LOW && choose < 10){
                ch2 = 1;
                process = 2;
                choose = 0;
            }
            my_lcd_process(5);
            break;

        case 2:
            valve1_thread();
            valve2_thread();
            if(ch1 == 0 || ch1 == 6 || ch1 == 7){
                if(rt_pin_read(BUTTON_VALVE1) == PIN_LOW)ch1 = 1;
            }
            if(ch2 == 0 || ch2 == 6 || ch2 == 7){
                if(rt_pin_read(BUTTON_VALVE2) == PIN_LOW)ch2 = 1;
            }
            if((button == 3 || button == 4) && (ch1 == 0 || ch1 == 6 || ch1 == 7) && (ch2 == 0 || ch2 == 6 || ch2 == 7)){
                ch1 = 0;
                ch2 = 0;
                choose = 0;
                time_counter = 0;
                process = 1;
            }
            my_lcd_process(6);
            break;
    }
}

int main(void)
{
    uint8_t button;
    my_io_init();
    my_flash_init();
    my_lcd_init();

    while (1)
    {

        button = my_button_mode();
        if(button != 0)rt_kprintf("button = %d\r\n",button);

        my_process_thread(button);
        rt_kprintf("%d     %d     %d",rt_pin_read(OPEN1),rt_pin_read(CLOSED1),rt_pin_read(HALL1));
        time_counter++;
        time_counter%=10;
        rt_thread_mdelay(30);
    }

    return RT_EOK;
}

void r1(){
    rt_pin_write(RGB_R1, PIN_HIGH);
}
void g1(){
    rt_pin_write(RGB_G1, PIN_HIGH);
}
void r2(){
    rt_pin_write(RGB_R2, PIN_HIGH);
}
void g2(){
    rt_pin_write(RGB_G2, PIN_HIGH);
}
MSH_CMD_EXPORT(r1, rgb);
MSH_CMD_EXPORT(g1, rgb);
MSH_CMD_EXPORT(r2, rgb);
MSH_CMD_EXPORT(g2, rgb);


