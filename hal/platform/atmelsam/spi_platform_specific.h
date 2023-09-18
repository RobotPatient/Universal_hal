/**
* \file            spi_platform_specific.h
* \brief           Include file with platform specific options for the SPI module
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

#ifndef HAL_SPI_PLATFORM_SPECIFIC
#define HAL_SPI_PLATFORM_SPECIFIC
/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <sam.h>
#include <stddef.h>

#include <i2c_platform_specific.h>
#include <gpio_platform_specific.h>

typedef enum { SPI_OPERATING_MODE_MASTER, SPI_OPERATING_MODE_SLAVE } spi_operating_mode_t;

typedef struct {
    sercom_num_t sercom_inst_num;
    Sercom* sercom_inst;
    clk_gen_num_t clk_gen_slow;
    clk_gen_num_t clk_gen_fast;
    uint32_t fast_clk_gen_frequency;
    spi_operating_mode_t operating_mode;
} spi_periph_inst_t;

typedef struct {
    spi_periph_inst_t* spi_peripheral;
    gpio_pin_t cs_pin;
} spi_dev_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
