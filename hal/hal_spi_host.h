/**
* \file            hal_spi_host.h
* \brief           SPI host driver module include file
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
#ifndef HAL_SPI_HOST_H
#define HAL_SPI_HOST_H

#ifndef DISABLE_SPI_HOST_MODULE

#include "hal_gpio.h"
#include "spi_common/spi_platform_specific.h"
/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/**
 * @brief Function to initialize the specified HW peripheral with SPI functionality.
 *
 * @param spi_peripheral_num The SPI peripheral to use
 * @param spi_clock_source The clock source to use for configuring the SPI peripheral
 * @param spi_clock_source_freq The frequency of the clock source
 * @param spi_bus_frequency The frequency/baud rate to use for the SPI communication
 * @param spi_extra_configuration_opt Extra configuration options for the SPI host driver
 */
uhal_status_t spi_host_init(const spi_host_inst_t spi_peripheral_num, const uint32_t spi_clock_source, const uint32_t spi_clock_source_freq,
                            const unsigned long spi_bus_frequency, const spi_bus_opt_t spi_extra_configuration_opt);

#define SPI_HOST_INIT(spi_peripheral_num, peripheral_clock_source, peripheral_clock_freq, spi_bus_frequency, spi_extra_configuration_opt)            \
    ({                                                                                                                                               \
        int retval;                                                                                                                                  \
        SPI_HOST_INIT_PARAMETER_CHECK(spi_peripheral_num, peripheral_clock_source, peripheral_clock_freq, spi_bus_frequency,                         \
                                      spi_extra_configuration_opt);                                                                                  \
        retval = spi_host_init(spi_peripheral_num, peripheral_clock_source, peripheral_clock_freq, spi_bus_frequency, spi_extra_configuration_opt);  \
        retval;                                                                                                                                      \
    })

/**
 * @brief Function to de-initialize the specified HW peripheral (disables SPI on the HW peripheral).
 *
 * @param spi_peripheral_num The SPI peripheral to use
 */
uhal_status_t spi_host_deinit(const spi_host_inst_t spi_peripheral_num);

#define SPI_HOST_DEINIT(spi_peripheral_num)                                                                                                          \
    ({                                                                                                                                               \
        int retval;                                                                                                                                  \
        SPI_HOST_DEINIT_PARAMETER_CHECK(spi_peripheral_num);                                                                                         \
        retval = spi_host_deinit(spi_peripheral_num);                                                                                                \
        retval;                                                                                                                                      \
    })
/**
 * @brief Start a spi transaction (sets the chip select line low)
 *
 * @param spi_peripheral_num The SPI device to start a transaction with.
 */
uhal_status_t spi_host_start_transaction(const spi_host_inst_t spi_peripheral_num, const gpio_pin_t chip_select_pin,
                                         const spi_extra_dev_opt_t device_specific_config_opt);

#define SPI_HOST_START_TRANSACTION(spi_peripheral_num, chip_select_pin, device_specific_config_opt)                                                  \
    ({                                                                                                                                               \
        int retval;                                                                                                                                  \
        SPI_HOST_START_TRANSACTION_PARAMETER_CHECK(spi_peripheral_num, chip_select_pin, device_specific_config_opt);                                 \
        retval = spi_host_start_transaction(spi_peripheral_num, chip_select_pin, device_specific_config_opt);                                        \
        retval;                                                                                                                                      \
    })

/**
 * @brief End a spi transaction (sets the chip select line high)
 *
 * @param spi_peripheral_num The SPI device to start a transaction with.
 */
uhal_status_t spi_host_end_transaction(const spi_host_inst_t spi_peripheral_num, const gpio_pin_t chip_select_pin);

#define SPI_HOST_END_TRANSACTION(spi_peripheral_num, chip_select_pin)                                                                                \
    ({                                                                                                                                               \
        int retval;                                                                                                                                  \
        SPI_HOST_END_TRANSACTION_PARAMETER_CHECK(spi_peripheral_num, chip_select_pin);                                                               \
        retval = spi_host_end_transaction(spi_peripheral_num, chip_select_pin);                                                                      \
        retval;                                                                                                                                      \
    })

/**
 * @brief Function to execute a write blocking transaction (blocking means it will wait till the transaction is finished)
 *
 * @param spi_peripheral_num The SPI peripheral to use.
 * @param write_buff Pointer to a buffer containing data to write
 * @param size The amount of bytes to write
 */
uhal_status_t spi_host_write_blocking(const spi_host_inst_t spi_peripheral_num, const unsigned char* write_buff, const size_t size);

#define SPI_HOST_WRITE_BLOCKING(spi_peripheral_num, write_buff, size)                                                                                \
    ({                                                                                                                                               \
        int retval;                                                                                                                                  \
        SPI_HOST_WRITE_PARAMETER_CHECK(spi_peripheral_num, write_buff, size);                                                                        \
        retval = spi_host_write_blocking(spi_peripheral_num, write_buff, size);                                                                      \
        retval;                                                                                                                                      \
    })

/**
 * @brief Function to execute a write non-blocking transaction (non-blocking means it will not wait till the transaction is finished and stack them in a buffer or such)
 *
 * @param spi_peripheral_num The SPI peripheral to use.
 * @param write_buff Pointer to a buffer containing data to write
 * @param size The amount of bytes to write
 */
uhal_status_t spi_host_write_non_blocking(const spi_host_inst_t spi_peripheral_num, const unsigned char* write_buff, const size_t size);

#define SPI_HOST_WRITE_NON_BLOCKING(spi_peripheral_num, write_buff, size)                                                                            \
    ({                                                                                                                                               \
        int retval;                                                                                                                                  \
        SPI_HOST_WRITE_PARAMETER_CHECK(spi_peripheral_num, write_buff, size);                                                                        \
        retval = spi_host_write_non_blocking(spi_peripheral_num, write_buff, size);                                                                  \
        retval;                                                                                                                                      \
    })

/**
 * @brief Function to execute a read blocking transaction (blocking means it will wait till the transaction is finished)
 *
* @param spi_peripheral_num The SPI peripheral to use.
* @param read_buff Pointer to a read buffer
* @param amount_of_bytes The amount of bytes to read
 */
uhal_status_t spi_host_read_blocking(const spi_host_inst_t spi_peripheral_num, unsigned char* read_buff, size_t amount_of_bytes);

#define SPI_HOST_READ_BLOCKING(spi_peripheral_num, read_buff, amount_of_bytes)                                                                       \
    ({                                                                                                                                               \
        int retval;                                                                                                                                  \
        SPI_HOST_READ_PARAMETER_CHECK(spi_peripheral_num, read_buff, amount_of_bytes);                                                               \
        retval = spi_host_read_blocking(spi_peripheral_num, read_buff, amount_of_bytes);                                                             \
        retval;                                                                                                                                      \
    })

/**
 * @brief Function to execute a read non-blocking transaction (non-blocking means it will not wait till the transaction is finished and stack the transactions in to a buffer)
 *
 * @param spi_peripheral_num The SPI peripheral to use.
 * @param read_buff Pointer to a read buffer
 * @param amount_of_bytes The amount of bytes to read
 */
uhal_status_t spi_host_read_non_blocking(const spi_host_inst_t spi_peripheral_num, unsigned char* read_buff, size_t amount_of_bytes);

#define SPI_HOST_READ_NON_BLOCKING(spi_peripheral_num, read_buff, amount_of_bytes)                                                                   \
    ({                                                                                                                                               \
        int retval;                                                                                                                                  \
        SPI_HOST_READ_PARAMETER_CHECK(spi_peripheral_num, read_buff, amount_of_bytes);                                                               \
        retval = spi_host_read_non_blocking(spi_peripheral_num, read_buff, amount_of_bytes);                                                         \
        retval;                                                                                                                                      \
    })

/**
 * @brief IRQ handler for SPI host data receive interrupt.
 *        Gets run when a host read action is executed.
 *        By defining this function inside a source file outside the Universal HAL, the default IRQ handler will be overridden
 *       and the compiler will automatically link your own custom implementation.
 * @param hw Handle to the HW peripheral on which the SPI bus is ran
 * @param transaction SPI transaction info about the current initialized transaction on the HW peripheral.
 *                    The info will be automatically supplied when using the spi_host_write and spi_host_read functions above
 *
 * @note Using your own custom IRQ handler might break the use of the write and read functions listed above
 */
void spi_host_data_recv_irq(const void* hw, volatile bustransaction_t* transaction) __attribute__((weak));

/**
 * @brief IRQ handler for SPI host data send interrupt.
 *        Gets run when a host write action is executed.
 *        By defining this function inside a source file outside the Universal HAL, the default IRQ handler will be overridden
 *        and the compiler will automatically link your own custom implementation.
 * @param hw Handle to the HW peripheral on which the SPI bus is ran
 * @param transaction SPI transaction info about the current initialized transaction on the HW peripheral.
 *                    The info will be automatically supplied when using the spi_host_write and spi_host_read functions above.
 *
 * @note Using your own custom IRQ handler might break the use of the write and read functions listed above
 */
void spi_host_data_send_irq(const void* hw, volatile bustransaction_t* transaction) __attribute__((weak));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DISABLE_SPI_HOST_MODULE */

#endif