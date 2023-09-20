/**
* \file            hal_spi_slave.h
* \brief           SPI slave driver module include file
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
/** SPI Module
 */
#ifndef HAL_SPI_SLAVE_H
#define HAL_SPI_SLAVE_H
/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "hal_gpio.h"
#include "spi_platform_specific.h"

/**
 * @brief SPI device struct
 */
typedef struct {
    const spi_periph_inst_t*    spi_peripheral;
    const spi_clock_polarity_t  clock_polarity;
    const spi_data_order_t      data_order;
    const spi_character_size_t  character_size;
    const spi_slave_addr_mode_t address_mode;
    const uint8_t               slave_addr;
    const gpio_pin_t            cs_pin;
} spi_slave_dev_t;

/**
 * @brief Function to initialize the specified HW peripheral with SPI functionality.
 *        Uses the options set in the i2c_periph_inst_t struct defined platform_specific.h.
 *        To ensure platform compatibility place static i2c_periph_inst_t for each hw peripheral in an board_options.h file
 *        and include it in the build.
 *        Depending on the platform used it can configure for either host/client functionality.
 * @param i2c_instance I2C options to be used when configuring the HW peripheral.
 *                      It might include options like: Slave mode enabled
 *                                                     Peripheral clock options
 *                                                     HW peripheral instance number
 *                                                     HW peripheral handle
 *
 * @param baud_rate The I2C Clock frequency to be used in transactions (only used in host mode, when in slave mode every value will be discarded)
 */
void spi_slave_init(const spi_slave_dev_t* spi_instance);

/**
 * @brief Function to de-initialize the specified HW peripheral (disables I2C on the HW peripheral).
 * @param i2c_instance I2C options used when configuring the HW peripheral.
 */
void spi_slave_deinit(const spi_slave_dev_t* spi_instance);

/**
 * @brief IRQ handler for SPI Client address match interrupt.
 *        Gets run when a start condition with valid slave address is detected.
 *        By defining this function inside a source file outside the Universal HALL, the default IRQ handler will be overridden
 *        and the compiler will automatically link your own custom implementation.
 * @param hw Handle to the HW peripheral on which the I2C bus is ran
 * @param transaction I2C transaction info about the current initialized transaction on the HW peripheral.
 *
 *  @note I2C Slave functionality doesn't use the read/write functions below
 */
void spi_slave_chip_select_irq(const void* hw, volatile bustransaction_t* transaction) __attribute__((weak));

/**
 * @brief IRQ handler for I2C Client receive interrupt.
 *        Gets run when a byte is received from the host.
 *        By defining this function inside a source file outside the Universal HALL, the default IRQ handler will be overridden
 *        and the compiler will automatically link your own custom implementation.
 * @param hw Handle to the HW peripheral on which the I2C bus is ran
 * @param transaction I2C transaction info about the current initialized transaction on the HW peripheral.
 *
 *  @note I2C Slave functionality doesn't use the read/write functions below
 */
void spi_slave_data_recv_irq(const void* hw, volatile bustransaction_t* transaction) __attribute__((weak));

/**
 * @brief IRQ handler for I2C Client send interrupt.
 *        Gets run when a byte is requested from the host.
 *        By defining this function inside a source file outside the Universal HALL, the default IRQ handler will be overridden
 *        and the compiler will automatically link your own custom implementation.
 * @param hw Handle to the HW peripheral on which the I2C bus is ran
 * @param transaction I2C transaction info about the current initialized transaction on the HW peripheral.
 *
 *  @note I2C Slave functionality doesn't use the read/write functions below
 */
void spi_slave_data_send_irq(const void* hw, volatile bustransaction_t* transaction) __attribute__((weak));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif