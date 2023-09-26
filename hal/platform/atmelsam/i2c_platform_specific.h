/**
* \file            i2c_platform_specific.h
* \brief           Include file with platform specific options for the I2C module
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

#ifndef I2C_MASTER_PLATFORM_SPECIFIC
#define I2C_MASTER_PLATFORM_SPECIFIC
/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
//
// Created by victor on 9/18/23.
//

#include <sam.h>
#include <stddef.h>
#include "sercom_stuff.h"
#include "peripheral_clocking.h"

typedef enum { I2C_OPERATING_MODE_MASTER, I2C_OPERATING_MODE_SLAVE } i2c_operating_mode_t;

typedef struct {
    sercom_num_t sercom_inst_num;
    Sercom* sercom_inst;
    clk_gen_num_t clk_gen_slow;
    clk_gen_num_t clk_gen_fast;
    uint32_t fast_clk_gen_frequency;
    uint8_t irq_priority;
    i2c_operating_mode_t operating_mode;
    unsigned short i2c_slave_addr;
} i2c_periph_inst_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
