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

typedef enum {
    SPI_OPERATING_MODE_SLAVE = 0x2, SPI_OPERATING_MODE_MASTER
} spi_operating_mode_t;
    
typedef enum {
    SPI_CLOCK_POLARITY_SCK_LOW_IDLE, SPI_CLOCK_POLARITY_SCK_HIGH_IDLE
} spi_clock_polarity_t;

typedef enum {
    SPI_DATA_ORDER_MSB_FIRST, SPI_DATA_ORDER_LSB_FIRST
} spi_data_order_t;

typedef enum {
    SERCOM_DIPO_PAD0, SERCOM_DIPO_PAD1, SERCOM_DIPO_PAD2, SERCOM_DIPO_PAD3
} sercom_dipo_pad_t;

typedef enum {
    SERCOM_DOPO_PAD0, SERCOM_DOPO_PAD1, SERCOM_DOPO_PAD2, SERCOM_DOPO_PAD3
} sercom_dopo_pad_t;

typedef enum {
    SPI_ADDRESS_MODE_MASK, SPI_ADDRESS_MODE_2_ADDRS, SPI_ADDRESS_MODE_RANGE
} spi_slave_addr_mode_t;

typedef enum {
    SPI_8_BIT_CHARACTER_SIZE, SPI_9_BIT_CHARACTER_SIZE
} spi_character_size_t;

typedef struct {
    sercom_num_t sercom_inst_num;
    Sercom *sercom_inst;
    clk_gen_num_t clk_gen_slow;
    clk_gen_num_t clk_gen_fast;
    uint32_t fast_clk_gen_frequency;
    spi_operating_mode_t operating_mode;
    sercom_dipo_pad_t dipo_pad;
    sercom_dopo_pad_t dopo_pad;
} spi_periph_inst_t;

typedef struct {
    const spi_periph_inst_t *spi_peripheral;
    const gpio_pin_t cs_pin;
    const spi_clock_polarity_t clock_polarity;
    const spi_data_order_t data_order;
    const spi_slave_addr_mode_t slave_addr_mode;
    const spi_character_size_t character_size;
} spi_dev_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
