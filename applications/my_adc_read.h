/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-02     Gwvt       the first version
 */
#ifndef APPLICATIONS_MY_ADC_READ_H_
#define APPLICATIONS_MY_ADC_READ_H_

#include <rtthread.h>
#include <rtdbg.h>
#include "rtdevice.h"

#define ADC_DEV_NAME        "adc1"      /* ADC 设备名称 */
#define ADC_DEV_CHANNEL1     11           /* ADC 通道 */
#define ADC_DEV_CHANNEL2     13          /* ADC 通道 */
#define REFER_VOLTAGE       330         /* 参考电压 3.3V,数据精度乘以100保留2位小数*/
#define CONVERT_BITS        (1 << 12)   /* 转换位数为12位 */

void my_adc_init();
uint32_t read_adc1();
uint32_t read_adc2();

#endif /* APPLICATIONS_MY_ADC_READ_H_ */
