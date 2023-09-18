/**
* \file            hal_spi.h
* \brief           SPI module include file
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
#ifndef HAL_SPI_H
#define HAL_SPI_H
/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "spi_platform_specific.h"


/**
 * @brief Function to initialize the specified HW peripheral with I2C functionality.
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
void spi_init(const spi_dev_t *spi_instance, unsigned long baud_rate);

/**
 * @brief Function to de-initialize the specified HW peripheral (disables I2C on the HW peripheral).
 * @param i2c_instance I2C options used when configuring the HW peripheral.
 */
void spi_deinit(const spi_dev_t *spi_instance);

/**
 * @brief Function to execute a write blocking transaction (blocking means it will wait till the transaction is finished)
 *        This function does only work in host-mode.
 * @param i2c_instance I2C options used when configuring the HW peripheral.
 * @param addr The I2C address of the client device to write to
 * @param write_buff Pointer to the write buffer with all the bytes that have to be written
 * @param size The amount of bytes which have to be written
 * @param stop_bit Does this transaction end with or without a stop-bit: Value 1 is with stop-bit
 *                                                                       Value 0 is without stop-bit
 */
void spi_write_blocking(const spi_dev_t *spi_instance, const unsigned char *write_buff, size_t size);

/**
 * @brief Function to execute a write non-blocking transaction (non-blocking means it will not wait till the transaction is finished and stack them in a buffer or such)
 *        This function does only work in host-mode.
 * @param i2c_instance I2C options used when configuring the HW peripheral.
 * @param addr The I2C address of the client device to write to
 * @param write_buff Pointer to the write buffer with all the bytes that have to be written
 * @param size The amount of bytes which have to be written
 * @param stop_bit Does this transaction end with or without a stop-bit: Value 1 is with stop-bit
 *                                                                       Value 0 is without stop-bit
 */
void spi_write_non_blocking(const spi_dev_t *spi_instance, const unsigned char *write_buff, size_t size);

/**
 * @brief Function to execute a read blocking transaction (blocking means it will wait till the transaction is finished)
 *        This function does only work in host-mode.
 * @param i2c_instance I2C options used when configuring the HW peripheral.
 * @param addr The I2C address of the client device to write to
 * @param read_buff Pointer to the read buffer where all read bytes will be written
 * @param amount_of_bytes The amount of bytes which have to be read
 */
void spi_read_blocking(const spi_dev_t *spi_instance, unsigned char *read_buff, size_t amount_of_bytes);

/**
 * @brief Function to execute a read non-blocking transaction (non-blocking means it will not wait till the transaction is finished and stack the transactions in to a buffer)
 *        This function does only work in host-mode.
 * @param i2c_instance I2C options used when configuring the HW peripheral.
 * @param addr The I2C address of the client device to write to
 * @param read_buff Pointer to the read buffer where all read bytes will be written
 * @param amount_of_bytes The amount of bytes which have to be read
 */
void spi_read_non_blocking(const spi_dev_t *spi_instance, unsigned char *read_buff, size_t amount_of_bytes);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif