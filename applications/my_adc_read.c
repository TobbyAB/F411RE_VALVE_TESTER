/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-02     Gwvt       the first version
 */

#include "my_adc_read.h"

rt_adc_device_t adc_dev;

void my_adc_init(){
    adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
    if (adc_dev == RT_NULL)
    {
        rt_kprintf("adc sample run failed! can't find %s device!\n", ADC_DEV_NAME);
    }
}

uint32_t read_adc1(){
    uint32_t value=0;
    rt_adc_enable(adc_dev, ADC_DEV_CHANNEL1);
    value = rt_adc_read(adc_dev, ADC_DEV_CHANNEL1);
//    rt_kprintf("the value is :%d \n", value);
    rt_adc_disable(adc_dev, ADC_DEV_CHANNEL1);
    return value;
}

uint32_t read_adc2(){
    uint32_t value=0;
    rt_adc_enable(adc_dev, ADC_DEV_CHANNEL2);
    value = rt_adc_read(adc_dev, ADC_DEV_CHANNEL2);
//    rt_kprintf("the value is :%d \n", value);
    rt_adc_disable(adc_dev, ADC_DEV_CHANNEL2);
    return value;
}
MSH_CMD_EXPORT(read_adc1,read adc1);
