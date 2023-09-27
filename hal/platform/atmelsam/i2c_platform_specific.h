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
#include "peripheral_clocking.h"
#include "sercom_stuff.h"
#include "error_handling.h"

typedef enum {
    I2C_OPERATING_MODE_MASTER,
    I2C_OPERATING_MODE_SLAVE
} i2c_operating_mode_t;

//typedef struct {
//    sercom_num_t         sercom_inst_num;
//    Sercom*              sercom_inst;
//    clk_gen_num_t        clk_gen_slow;
//    clk_gen_num_t        clk_gen_fast;
//    uint32_t             fast_clk_gen_frequency;
//    uint8_t              irq_priority;
//    i2c_operating_mode_t operating_mode;
//    unsigned short       i2c_slave_addr;
//}
//
typedef enum {
  I2C_SERCOM0,
  I2C_SERCOM1,
  I2C_SERCOM2,
  I2C_SERCOM3,
  I2C_SERCOM4,
  I2C_SERCOM5
} i2c_periph_inst_t;



typedef enum {
  I2C_CLK_SOURCE_USE_DEFAULT = 0x00,
  I2C_CLK_SOURCE_FAST_CLKGEN0 = 0x01,
  I2C_CLK_SOURCE_FAST_CLKGEN1 = 0x02,
  I2C_CLK_SOURCE_FAST_CLKGEN2 = 0x03,
  I2C_CLK_SOURCE_FAST_CLKGEN3 = 0x04,
  I2C_CLK_SOURCE_FAST_CLKGEN4 = 0x05,
  I2C_CLK_SOURCE_FAST_CLKGEN5 = 0x06,
  I2C_CLK_SOURCE_FAST_CLKGEN6 = 0x07,
  I2C_CLK_SOURCE_FAST_CLKGEN7 = 0x08,
  I2C_CLK_SOURCE_SLOW_CLKGEN0 = 0x100,
  I2C_CLK_SOURCE_SLOW_CLKGEN1 = 0x200,
  I2C_CLK_SOURCE_SLOW_CLKGEN2 = 0x300,
  I2C_CLK_SOURCE_SLOW_CLKGEN3 = 0x400,
  I2C_CLK_SOURCE_SLOW_CLKGEN4 = 0x500,
  I2C_CLK_SOURCE_SLOW_CLKGEN5 = 0x600,
  I2C_CLK_SOURCE_SLOW_CLKGEN6 = 0x700,
  I2C_CLK_SOURCE_SLOW_CLKGEN7 = 0x800,
} i2c_clock_sources_t;

typedef enum {
  I2C_EXTRA_OPT_NONE = 0,
  I2C_EXTRA_OPT_4_WIRE_MODE = 1,
  I2C_EXTRA_OPT_IRQ_PRIO_0 = 0x100,
  I2C_EXTRA_OPT_IRQ_PRIO_1 = 0x200,
  I2C_EXTRA_OPT_IRQ_PRIO_2 = 0x300,
  I2C_EXTRA_OPT_IRQ_PRIO_3 = 0x400
} i2c_extra_opt_t;




#define I2C_HOST_FUNC_PARAMETER_CHECK(i2c_instance, baud_rate) do { \
                                                                  } while(0);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
