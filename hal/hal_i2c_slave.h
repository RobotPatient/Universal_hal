/**
* \file            hal_i2c_slave.h
* \brief           I2C module include file
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
#ifndef HAL_I2C_SLAVE_H
#define HAL_I2C_SLAVE_H

#ifndef DISABLE_I2C_SLAVE_MODULE
#include "i2c_common/i2c_platform_specific.h"

/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Initialize the hardware peripheral with I2C slave functionality.
 * @param i2c_peripheral_num The i2c instance to initialize
 * @param slave_addr The i2c slave address to set
 * @param clock_sources The clock sources to use for the i2c peripheral (MCU specific)
 * @param extra_configuration_options The extra configuration options to use for this i2c slave instance
 */
uhal_status_t i2c_slave_init(const i2c_periph_inst_t i2c_peripheral_num, const uint16_t slave_addr, const i2c_clock_sources_t clock_sources,
                             const i2c_extra_opt_t extra_configuration_options);

#define I2C_SLAVE_INIT(i2c_peripheral_num, slave_addr, clock_sources, extra_configuration_options)                                  \
    ({                                                                                                                                               \
        int retval;                                                                                                                                  \
        I2C_SLAVE_INIT_PARAMETER_CHECK(i2c_peripheral_num, slave_addr, clock_sources, extra_configuration_options);                             \
        retval = i2c_slave_init(i2c_peripheral_num, slave_addr, clock_sources, extra_configuration_options);                        \
        retval;                                                                                                                                      \
    })

/**
 * @brief De-initialize an i2c slave initialized hardware peripheral.
 * @param i2c_peripheral_num The i2c peripheral to de-initialize
 */
uhal_status_t i2c_slave_deinit(const i2c_periph_inst_t i2c_peripheral_num);

#define I2C_SLAVE_DEINIT(i2c_peripheral_num)                                                                                                         \
    ({                                                                                                                                               \
        int retval;                                                                                                                                  \
        I2C_SLAVE_DEINIT_PARAMETER_CHECK(i2c_peripheral_num);                                                                                        \
        retval = i2c_slave_deinit(i2c_peripheral_num);                                                                                               \
        retval;                                                                                                                                      \
    })

/**
 * @brief IRQ handler for I2C Client address match interrupt.
 *        Gets run when a start condition with valid slave address is detected.
 *        By defining this function inside a source file outside the Universal HALL, the default IRQ handler will be overridden
 *        and the compiler will automatically link your own custom implementation.
 * @param hw Handle to the HW peripheral on which the I2C bus is ran
 * @param transaction I2C transaction info about the current initialized transaction on the HW peripheral.
 *
 *  @note I2C Slave functionality doesn't use the read/write functions below
 */
void i2c_slave_address_match_irq(const void* hw, volatile bustransaction_t* transaction) __attribute__((weak));

/**
 * @brief IRQ handler for I2C Client stop interrupt.
 *        Gets run when a stop condition is detected.
 *        By defining this function inside a source file outside the Universal HALL, the default IRQ handler will be overridden
 *        and the compiler will automatically link your own custom implementation.
 * @param hw Handle to the HW peripheral on which the I2C bus is ran
 * @param transaction I2C transaction info about the current initialized transaction on the HW peripheral.
 *
 *  @note I2C Slave functionality doesn't use the read/write functions below
 */
void i2c_slave_stop_irq(const void* hw, volatile bustransaction_t* transaction) __attribute__((weak));

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
void i2c_slave_data_recv_irq(const void* hw, volatile bustransaction_t* transaction) __attribute__((weak));

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
void i2c_slave_data_send_irq(const void* hw, volatile bustransaction_t* transaction) __attribute__((weak));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DISABLE_I2C_SLAVE_MODULE */

#endif //ATMELSAMD21_HAL_I2C_SLAVE_H
