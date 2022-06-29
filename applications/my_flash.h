/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-05     GWVT       the first version
 */
#ifndef APPLICATIONS_MY_FLASH_H_
#define APPLICATIONS_MY_FLASH_H_

#include <rtthread.h>
#include <rtdbg.h>
#include "rtdevice.h"
#include "spi_flash_sfud.h"

void my_flash_init();
uint8_t get_device_data(uint8_t k);
void set_device_data(uint8_t switch_time,uint8_t battry_time,uint8_t cap_number,uint8_t ch1,uint8_t ch2);
#endif /* APPLICATIONS_MY_FLASH_H_ */
