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

#include <sam.h>
#include <stddef.h>

typedef enum { I2COpModeMaster, I2COpModeSlave } i2c_operating_mode_t;

typedef enum { SERCOM_NUM_0, SERCOM_NUM_1, SERCOM_NUM_2, SERCOM_NUM_3, SERCOM_NUM_4, SERCOM_NUM_5 } sercom_num_t;

typedef enum { CLKGEN_0, CLKGEN_1, CLKGEN_2, CLKGEN_3, CLKGEN_4, CLKGEN_5, CLKGEN_6, CLKGEN_7, CLKGEN_8 } clk_gen_num_t;

typedef struct {
    sercom_num_t sercom_inst_num;
    Sercom* sercom_inst;
    clk_gen_num_t clk_gen_slow;
    clk_gen_num_t clk_gen_fast;
    uint32_t fast_clk_gen_frequency;
    i2c_operating_mode_t operating_mode;
    unsigned short i2c_slave_addr;
} i2c_periph_inst_t;

typedef struct {
    uint8_t transaction_type;
    uint8_t instance_num;
    const uint8_t* write_buffer;
    uint8_t* read_buffer;
    uint8_t buf_size;
    uint8_t buf_cnt;
} bustransaction_t;

typedef enum {
    SERCOMACT_NONE,
    SERCOMACT_IDLE_I2CS,
    SERCOMACT_IDLE_I2CM,
    SERCOMACT_I2C_DATA_TRANSMIT_NO_STOP,
    SERCOMACT_I2C_DATA_TRANSMIT_STOP,
    SERCOMACT_I2C_DATA_RECEIVE_STOP
} busactions_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
