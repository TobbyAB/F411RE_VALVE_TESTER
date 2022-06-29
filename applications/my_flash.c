/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-05     GWVT       the first version
 */

#include "my_flash.h"

static sfud_flash *sfud_dev = NULL;
static uint8_t device_data[5] = {
        10,10,40,1,1
};
void my_flash_init(){
    uint8_t i;
    sfud_dev = rt_sfud_flash_find("spi10");
//    dynamic_mutex = rt_mutex_create("flash_mutex", RT_IPC_FLAG_FIFO);/
    if(sfud_dev == RT_NULL)rt_kprintf("sfud_dev error");
    else {
        sfud_read(sfud_dev, 0, 5, device_data);
        if(device_data[0] == 0 && device_data[1] == 0 && device_data[2] == 0 && device_data[3] == 0 && device_data[4] == 0){
            device_data[0] = 10;
            device_data[1] = 10;
            device_data[2] = 40;
            device_data[3] = 3;
            device_data[4] = 1;
            rt_kprintf("0 0 0 0 0");
        }else {
            if(device_data[0] < 1)device_data[0] = 10;
            else if(device_data[0] > 99)device_data[0] = 99;

            if(device_data[1] < 1)device_data[1] = 10;
            else if(device_data[1] > 99)device_data[1] = 99;

            if(device_data[2] < 1)device_data[2] = 40;
            else if(device_data[2] > 50)device_data[2] = 50;

            if(device_data[3] < 3)device_data[3] = 3;
            else if(device_data[3] > 99)device_data[3] = 99;

            if(device_data[4] > 1)device_data[4] = 1;
        }
        for(i=0;i<5;i++){
            rt_kprintf("%d  ",device_data[i]);
        }
    }
}

uint8_t get_device_data(uint8_t k){
    if(k < 5){
        return device_data[k];
    }
    return 0;
}
void set_device_data(uint8_t switch_time,uint8_t battry_time,uint8_t cap_number,uint8_t ch1,uint8_t ch2){
    device_data[0] = switch_time;
    device_data[1] = battry_time;
    device_data[2] = cap_number;
    device_data[3] = ch1;
    device_data[4] = ch2;
    sfud_erase(sfud_dev, 0, 4096);
    sfud_write(sfud_dev, 0, 5, device_data);
}


