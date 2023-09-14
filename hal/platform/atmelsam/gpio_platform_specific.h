/**
* \file            gpio_platform_specific.h
* \brief           Include file with peripheral specific settings for the GPIO module
*/
/*
*  Copyright 2023 (C) Victor Hogeweij <hogeweyv@gmail.com>
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
* This file is part of the Universal Hal Framework.
*
* Author:          Victor Hogeweij <hogeweyv@gmail.com>
*/

#ifndef GPIO_PLATFORM_SPECIFIC
#define GPIO_PLATFORM_SPECIFIC
/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
    GPIO_LOW,
    GPIO_HIGH,
} gpio_level_t;

typedef enum { GPIO_PORT_A, GPIO_PORT_B, GPIO_PORT_C } gpio_port_t;

typedef enum {
    GPIO_MODE_A,
    GPIO_MODE_B,
    GPIO_MODE_C,
    GPIO_MODE_D,
    GPIO_MODE_E,
    GPIO_MODE_F,
    GPIO_MODE_G,
    GPIO_MODE_H,
    GPIO_MODE_I,
    GPIO_MODE_J,
    GPIO_MODE_K,
    GPIO_MODE_L,
    GPIO_MODE_N,
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
} gpio_mode_t;

typedef struct {
    gpio_port_t port_num;
    uint8_t pin_num;
} gpio_pin_t;

typedef enum {
    GPIO_OPT_PULL_UP = 4,
    GPIO_OPT_PULL_DOWN = 8,
    GPIO_OPT_SAMPLE_CONTINUOUSLY = 32,
    GPIO_OPT_DRIVE_STRENGTH_STRONG = 64,
} gpio_opt_t;

typedef enum {
    GPIO_IRQ_CHANNEL_0,
    GPIO_IRQ_CHANNEL_1,
    GPIO_IRQ_CHANNEL_2,
    GPIO_IRQ_CHANNEL_3,
    GPIO_IRQ_CHANNEL_4,
    GPIO_IRQ_CHANNEL_5,
    GPIO_IRQ_CHANNEL_6,
    GPIO_IRQ_CHANNEL_7,
    GPIO_IRQ_CHANNEL_8,
    GPIO_IRQ_CHANNEL_9,
    GPIO_IRQ_CHANNEL_10,
    GPIO_IRQ_CHANNEL_11,
    GPIO_IRQ_CHANNEL_12,
    GPIO_IRQ_CHANNEL_13,
    GPIO_IRQ_CHANNEL_14,
    GPIO_IRQ_CHANNEL_15,
    GPIO_IRQ_NMI
} gpio_irq_channel_t;

typedef enum {
    GPIO_IRQ_NONE,
    GPIO_IRQ_COND_RISING_EDGE,
    GPIO_IRQ_COND_FALLING_EDGE,
    GPIO_IRQ_COND_BOTH_EDGES,
    GPIO_IRQ_COND_HIGH_LVL,
    GPIO_IRQ_COND_LOW_LVL
} gpio_irq_condition_t;

typedef enum{
    GPIO_IRQ_EXTRA_NONE,
    GPIO_IRQ_EXTRA_FILTERING,
    GPIO_IRQ_USE_EVENTS,
    GPIO_IRQ_WAKE_FROM_SLEEP
}gpio_irq_extra_opt_t;

typedef struct {
gpio_irq_channel_t irq_channel;
gpio_irq_condition_t irq_condition;
gpio_irq_extra_opt_t irq_extra_opt;
} gpio_irq_opt_t;


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
