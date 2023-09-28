/**
* \file            hal_i2c.h
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
/**
 * The I2C module
 *
 * Possibilities:
 * - Slave/Client and Master/Host functionality : Master means MCU as host and the connecting device as a slave device, with slave meaning the mcu is the connecting slave device.
 * @note Host & Slave functionality is not supported by every hardware peripheral of every microcontroller variant.
 *       The universal hal can't check whether your microcontroller supports this, please make sure that the microcontroller has
 *       this feature and if the feature is supported by this hal, see wiki (https://hoog-v.github.io/Universal_hal/).
 * @note Slave functionality is very difficult to implement with a generic api. Therefore this module only supports Slave functionality with
 *       the interrupt functions listed below. The read_(non_)blocking and write_(non_)blocking functions do not work with this mode. Why?
 *       Because every slave device has other functionality and way of interacting with the host. It is way easier to give the user of the library/framework
 *       full control of the slave functionality, instead of abstracting away these interfaces in to write and read functions.
 * - Blocking and non-blocking write and read functions for I2C host functionality.
 * @note The write and read functions are implemented using interrupts. Keep in mind that this might mess with other timing-critical applications implemented
 *       on the microcontroller.
 * @note The implementation of non-blocking functions differ for each microcontroller variant. Some microcontroller variants have built-in functionality for
 *       stacking transactions. Other microcontrollers don't have it and have to be implemented manually in software.
 * - Setting different clock-frequencies for the I2C connection.
 * @note The supported clock-frequencies are dependent on which frequencies are supported by the hardware peripheral of the microcontroller variant.
 *       The I2C module checks for this, you have to... Keep a eye on the datasheet of the MCU and the wiki (https://hoog-v.github.io/Universal_hal/).
 *       Software bit-banged implementations of the I2C driver only support standard mode speeds (100KHz)
 * - Linking your own custom interrupt handlers to this module for master as well as slave functionality.
 * @note Linking custom implementations of i2c master irq functions will certainly break the write and read functions...
 *       This is not the case if you facilitate the usage of the read and write functions in your custom implementation.
 *       See the default handler in the default_irq_handlers.c file.
 *
 * Impossibilities:
 * - Although the platform specific details have been abstracted away in to the i2c_periph_inst_t struct, you still have to manually configure this struct
 *   with the right settings when porting between different microcontroller platforms.
 * @note The recommended way to make porting easier is by making a general board_define file included with your main.c which has all the peripheral configurations and
 *      pin_definitions defined. When porting the name of the structs will be the same, but the implementation details will differ, which means there will be only one place
 *      to edit when porting.
 * - Read and write functions for slave functionality. As said in the note above, every slave device is different. One is a big block of memory, while the other might be an active sensor.
 *   It is very difficult to create a generic API that facilitates every possible slave device ever created. That's why only the IRQ handlers and init function work in this mode.
 * @note The irq handlers trigger on the same moment in every device making porting easier.
 * @note The irq handlers are weakly linked to the default handler, meaning that this might not be a misra compatible solution. But it is certainly better than passing callback function
 *       pointers.
 * - When using a microcontroller with extensive clock systems: Cortex-M based microcontrollers for example. The i2c module will not configure clock generators or other clock system options.
 *   It might depending on the implementation of the init function link to an already configured clock generator listed in the i2c_periph_inst_t struct.
 * - Due to the minimalistic interface there has not been a lot of error checking incorporated in the library... This is still an in progress.
 *
 * - The ISR handlers are shared among all hardware peripherals. It is very difficult to provide a separate handler for every hardware instance since this differs for every mcu variant.
 *   The bustransaction_t struct helps to identify which peripheral it runs on. But keep this in mind.
 *
 */
#ifndef HAL_I2C_H
#define HAL_I2C_H
/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "assert.h"
#include "error_handling.h"
#include "i2c_platform_specific.h"

typedef enum {
    I2C_NO_STOP_BIT,
    I2C_STOP_BIT
} i2c_stop_bit_t;

/**
 * @brief Function to initialize the specified HW peripheral with I2C host functionality.
 *        To ensure platform compatibility use the default option as much as possible for each hw peripheral.
 *
 * @param i2c_peripheral_num The i2c peripheral to configure
 * @param clock_sources The clock source(s) to use..
 * @param periph_clk_freq The clock frequency of the peripheral (if multiple clocks are used this will be the fastest frequency).
 * @param baud_rate_freq The I2C Clock frequency to be used in transactions (only used in host mode, when in slave mode every value will be discarded).
 * @param extra_configuration_options The extra configuration options like: - DMA use
 *                                                                          - 4-Wire mode
 *                                                                          - IRQ priority
 */
uhal_status_t _i2c_host_init(const i2c_periph_inst_t i2c_peripheral_num, const i2c_clock_sources_t clock_sources,
                             const uint32_t periph_clk_freq, const uint32_t baud_rate_freq,
                             const i2c_extra_opt_t extra_configuration_options);

#define I2C_HOST_INIT(i2c_peripheral_num, clock_sources, periph_clk_freq, baud_rate_freq, extra_configuration_options) \
do {                                                                                                                    \
    I2C_HOST_INIT_FUNC_PARAMETER_CHECK(i2c_peripheral_num, clock_sources, periph_clk_freq, baud_rate_freq, extra_configuration_options); \
    _i2c_host_init(i2c_peripheral_num, clock_sources, periph_clk_freq, baud_rate_freq, extra_configuration_options);               \
}while(0);

/**
 * @brief Function to de-initialize the specified HW peripheral (disables I2C on the HW peripheral).
 * @param i2c_peripheral_num I2C options used when configuring the HW peripheral.
 */
uhal_status_t _i2c_host_deinit(const i2c_periph_inst_t i2c_peripheral_num);


#define I2C_HOST_DEINIT(i2c_peripheral_num) \
do {                                        \
I2C_HOST_DEINIT_FUNC_PARAMETER_CHECK(i2c_peripheral_num);    \
_i2c_host_deinit(i2c_peripheral_num);\
}while(0);

/**
 * @brief Function to execute a write blocking transaction (blocking means it will wait till the transaction is finished)
 *        This function does only work in host-mode.
 * @param i2c_peripheral_num The i2c peripheral to use
 * @param addr The I2C address of the client device to write to
 * @param write_buff Pointer to the write buffer with all the bytes that have to be written
 * @param size The amount of bytes which have to be written
 * @param stop_bit Does this transaction end with or without a stop-bit: Value 1 is with stop-bit
 *                                                                       Value 0 is without stop-bit
 */
uhal_status_t
_i2c_host_write_blocking(const i2c_periph_inst_t i2c_peripheral_num, unsigned char addr,
                         const unsigned char *write_buff,
                         size_t size,
                         i2c_stop_bit_t stop_bit);

#define I2C_HOST_WRITE_BLOCKING(i2c_peripheral_num, addr, write_buff, size, stop_bit) \
do {                                                                            \
I2C_HOST_WRITE_FUNC_PARAMETER_CHECK(i2c_peripheral_num, addr, write_buff, size, stop_bit); \
_i2c_host_write_blocking(i2c_peripheral_num, addr, write_buff, size, stop_bit);             \
}while(0);

/**
 * @brief Function to execute a write non-blocking transaction (non-blocking means it will not wait till the transaction is finished and stack them in a buffer or such)
 *        This function does only work in host-mode.
 * @param i2c_peripheral_num The i2c peripheral to use
 * @param addr The I2C address of the client device to write to
 * @param write_buff Pointer to the write buffer with all the bytes that have to be written
 * @param size The amount of bytes which have to be written
 * @param stop_bit Does this transaction end with or without a stop-bit: Value 1 is with stop-bit
 *                                                                       Value 0 is without stop-bit
 */
uhal_status_t
_i2c_host_write_non_blocking(const i2c_periph_inst_t i2c_peripheral_num, unsigned short addr, const unsigned char *write_buff,
                             size_t size,
                             i2c_stop_bit_t stop_bit);

#define I2C_HOST_WRITE_NON_BLOCKING(i2c_peripheral_num, addr, write_buff, size, stop_bit) \
do {                                                                            \
I2C_HOST_WRITE_FUNC_PARAMETER_CHECK(i2c_peripheral_num, addr, write_buff, size, stop_bit); \
_i2c_host_write_non_blocking(i2c_peripheral_num, addr, write_buff, size, stop_bit);             \
}while(0);

/**
 * @brief Function to execute a read blocking transaction (blocking means it will wait till the transaction is finished)
 *        This function does only work in host-mode.
 * @param i2c_peripheral_num The i2c peripheral to use
 * @param addr The I2C address of the client device to write to
 * @param read_buff Pointer to the read buffer where all read bytes will be written
 * @param amount_of_bytes The amount of bytes which have to be read
 */
uhal_status_t
_i2c_host_read_blocking(const i2c_periph_inst_t i2c_peripheral_num, unsigned short addr, unsigned char *read_buff,
                        size_t amount_of_bytes);

#define I2C_HOST_READ_BLOCKING(i2c_peripheral_num, addr, read_buff, size) \
do {                                                                            \
I2C_HOST_READ_FUNC_PARAMETER_CHECK(i2c_peripheral_num, addr, read_buff, size); \
_i2c_host_read_blocking(i2c_peripheral_num, addr, read_buff, size);             \
}while(0);

/**
 * @brief Function to execute a read non-blocking transaction (non-blocking means it will not wait till the transaction is finished and stack the transactions in to a buffer)
 *        This function does only work in host-mode.
 * @param i2c_peripheral_num The i2c peripheral to use
 * @param addr The I2C address of the client device to write to
 * @param read_buff Pointer to the read buffer where all read bytes will be written
 * @param amount_of_bytes The amount of bytes which have to be read
 */
uhal_status_t
_i2c_host_read_non_blocking(const i2c_periph_inst_t i2c_peripheral_num, unsigned short addr, unsigned char *read_buff,
                            size_t amount_of_bytes);

#define I2C_HOST_READ_NON_BLOCKING(i2c_peripheral_num, addr, read_buff, size) \
do {                                                                            \
I2C_HOST_READ_FUNC_PARAMETER_CHECK(i2c_peripheral_num, addr, read_buff, size); \
_i2c_host_read_non_blocking(i2c_peripheral_num, addr, read_buff, size);             \
}while(0);
/**
 * @brief IRQ handler for I2C host data receive interrupt.
 *        Gets run when a host read action is executed.
 *        By defining this function inside a source file outside the Universal HALL, the default IRQ handler will be overridden
 *       and the compiler will automatically link your own custom implementation.
 * @param hw Handle to the HW peripheral on which the I2C bus is ran
 * @param transaction I2C transaction info about the current initialized transaction on the HW peripheral.
 *                    The info will be automatically supplied when using the i2c_write and i2c_read functions below
 *
 * @note Using your own custom IRQ handler might break the use of the write and read functions listed above
 */
void i2c_host_data_recv_irq(const void *hw, volatile bustransaction_t *transaction) __attribute__((weak));

/**
 * @brief IRQ handler for I2C host data send interrupt.
 *        Gets run when a host write action is executed.
 *        By defining this function inside a -Obinarsource file outside the Universal HALL, the default IRQ handler will be overridden
 *        and the compiler will automatically link your own custom implementation.
 * @param hw Handle to the HW peripheral on which the I2C bus is ran
 * @param transaction I2C transaction info about the current initialized transaction on the HW peripheral.
 *                    The info will be automatically supplied when using the i2c_write and i2c_read functions below.
 *
 * @note Using your own custom IRQ handler might break the use of the write and read functions listed above
 */
void i2c_host_data_send_irq(const void *hw, volatile bustransaction_t *transaction) __attribute__((weak));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif