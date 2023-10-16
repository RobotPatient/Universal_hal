/**
* \file            spi_host.c
* \brief           Source file which implements the standard SPI Host API functions
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
#include <stddef.h>
#include "bit_manipulation.h"
#include "irq/default_irq_handlers.h"
#include "hal_gpio.h"
#include "hal_spi_host.h"
#include "spi_common/spi_platform_specific.h"

#define SERCOM_SLOW_CLOCK_SOURCE(x)               (x >> 8)

Sercom *spi_peripheral_mapping_table[6] = {SERCOM0, SERCOM1, SERCOM2, SERCOM3, SERCOM4, SERCOM5};


static inline Sercom *get_sercom_inst(const i2c_periph_inst_t peripheral_inst_num) {
    return spi_peripheral_mapping_table[peripheral_inst_num];
}

/**
 * @brief Helper function which waits for the sercom peripheral to get in sync and finish requested operations.
 *        By continually reading its SPI syncbusy register.
 * @param hw Pointer to the sercom peripheral to be manipulated or read
 * @param bits_to_read The bits to read within the syncbusy register (bitmask)
 * @note Possible bits that can be read are: SERCOM_SPI_SYNCBUSY_SWRST
 *                                           SERCOM_SPI_SYNCBUSY_ENABLE
 *                                           SERCOM_SPI_SYNCBUSY_CTRLB
 */
static inline void spi_wait_for_sync(const void* const hw, const uint32_t bits_to_read) {
    while (((Sercom*)hw)->SPI.SYNCBUSY.reg & bits_to_read) {
        ;
    }
}

static inline void spi_wait_for_transaction_finish(volatile bustransaction_t* bustransaction, busactions_t flag) {
    while (bustransaction->transaction_type != flag) {
        ;
    }
}

static inline uint8_t get_fast_clk_gen_val(const i2c_clock_sources_t clock_sources) {
    const uint16_t fast_clk_val = (clock_sources & 0xFF) - 1;
    return fast_clk_val;
}

static inline uint8_t get_slow_clk_gen_val(const i2c_clock_sources_t clock_sources) {
    const uint16_t slow_clk_val = SERCOM_SLOW_CLOCK_SOURCE(clock_sources) - 1;
    return slow_clk_val;
}

static inline uint8_t get_character_size_from_bus_opt(const spi_bus_opt_t bus_opt) {
    return BITMASK_COMPARE(bus_opt, SPI_BUS_OPT_CHAR_SIZE_10_BIT) >> 2;
}

static inline uint8_t get_clock_polarity_from_bus_opt(const spi_bus_opt_t bus_opt) {
    return BITMASK_COMPARE(bus_opt, SPI_BUS_OPT_CLOCK_POLARITY_SCK_HIGH);
}

static inline uint8_t get_data_order_from_bus_opt(const spi_bus_opt_t bus_opt) {
    return BITMASK_COMPARE(bus_opt, SPI_BUS_OPT_DATA_ORDER_LSB_FIRST) >> 1;
}

static inline uint8_t get_dopo_pad_from_bus_opt(const spi_bus_opt_t bus_opt) {
    return BITMASK_COMPARE(bus_opt, 0x38) >> 3;
}

static inline uint8_t get_dipo_pad_from_bus_opt(const spi_bus_opt_t bus_opt) {
    return BITMASK_COMPARE(bus_opt, 0x1C0) >> 6;
}

uhal_status_t spi_host_init(const spi_host_inst_t spi_instance, const uint32_t spi_clock_source, const uint32_t spi_clock_source_freq,
                            const unsigned long spi_bus_frequency, const spi_bus_opt_t spi_extra_configuration_opt) {
Sercom* sercom_instance = get_sercom_inst(spi_instance);
//    const spi_periph_inst_t* spi_peripheral_inst = spi_instance->spi_peripheral;
//    const uint8_t            invalid_sercom_instance_num = (spi_peripheral_inst->sercom_inst_num > SERCOM_INST_NUM - 1);
//    const uint8_t            hw_handle_is_null = (spi_peripheral_inst->sercom_inst == NULL);
//    const uint8_t            invalid_clk_gen_num =
//        (spi_peripheral_inst->clk_gen_slow > GCLK_GEN_NUM - 1 || spi_peripheral_inst->clk_gen_fast > GCLK_GEN_NUM - 1);
//    if (hw_handle_is_null || invalid_sercom_instance_num || invalid_clk_gen_num) {
//        return;
//    }
    // Set the clock system
#ifdef __SAMD51__

#else
PM->APBCMASK.reg |= 1 << (PM_APBCMASK_SERCOM0_Pos + spi_instance);
if (spi_clock_source != I2C_CLK_SOURCE_USE_DEFAULT) {
        const uint8_t clk_gen_slow = get_slow_clk_gen_val(spi_clock_source);
        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(clk_gen_slow) | GCLK_CLKCTRL_ID_SERCOMX_SLOW | GCLK_CLKCTRL_CLKEN;
        while (GCLK->STATUS.bit.SYNCBUSY);
        const uint8_t clk_gen_fast = get_fast_clk_gen_val(spi_clock_source);
        GCLK->CLKCTRL.reg =
            GCLK_CLKCTRL_GEN(clk_gen_fast) |
            ((GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + spi_instance) << GCLK_CLKCTRL_ID_Pos) | GCLK_CLKCTRL_CLKEN;
        GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0x01) | GCLK_GENDIV_ID(clk_gen_fast);
        while (GCLK->STATUS.bit.SYNCBUSY);
} else {
        const uint8_t clk_gen_slow = 3;
        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(clk_gen_slow) | GCLK_CLKCTRL_ID_SERCOMX_SLOW | GCLK_CLKCTRL_CLKEN;
        while (GCLK->STATUS.bit.SYNCBUSY);
        const uint8_t clk_gen_fast = 0;
        GCLK->CLKCTRL.reg =
            GCLK_CLKCTRL_GEN(clk_gen_fast) |
            ((GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + spi_instance) << GCLK_CLKCTRL_ID_Pos) | GCLK_CLKCTRL_CLKEN;
        GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0x01) | GCLK_GENDIV_ID(clk_gen_fast);
        while (GCLK->STATUS.bit.SYNCBUSY);
}
#endif
    const uint8_t dopo_pad = get_dopo_pad_from_bus_opt(spi_extra_configuration_opt);
    const uint8_t dipo_pad = get_dipo_pad_from_bus_opt(spi_extra_configuration_opt);
    const uint8_t clock_polarity = get_clock_polarity_from_bus_opt(spi_extra_configuration_opt);
    const uint8_t character_size = get_character_size_from_bus_opt(spi_extra_configuration_opt);
    const uint8_t data_order = get_data_order_from_bus_opt(spi_extra_configuration_opt);
    sercom_instance->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_SWRST;
    spi_wait_for_sync(sercom_instance, SERCOM_SPI_SYNCBUSY_SWRST | SERCOM_SPI_SYNCBUSY_ENABLE);
    sercom_instance->SPI.CTRLA.reg =
        SERCOM_SPI_CTRLA_MODE_SPI_MASTER | (clock_polarity << SERCOM_SPI_CTRLA_CPHA_Pos)
        | (data_order << SERCOM_SPI_CTRLA_CPOL_Pos) | (SERCOM_SPI_CTRLA_DIPO(dipo_pad))
        | (SERCOM_SPI_CTRLA_DOPO(dopo_pad));
    sercom_instance->SPI.CTRLB.reg = SERCOM_SPI_CTRLB_PLOADEN | SERCOM_SPI_CTRLB_CHSIZE(character_size);
    sercom_instance->SPI.BAUD.reg = spi_clock_source_freq / spi_bus_frequency / 2;
    sercom_instance->SPI.INTENSET.reg = SERCOM_SPI_INTENSET_RXC | SERCOM_SPI_INTENSET_TXC | SERCOM_SPI_INTENSET_SSL;
    sercom_instance->SPI.CTRLA.reg |= SERCOM_SPI_CTRLA_ENABLE;
    spi_wait_for_sync(sercom_instance, SERCOM_SPI_SYNCBUSY_ENABLE);
    const enum IRQn irq_type = (SERCOM0_IRQn + spi_instance);
    NVIC_EnableIRQ(irq_type);
    NVIC_SetPriority(irq_type, 2);
    sercom_bustrans_buffer[spi_instance].transaction_type = SERCOMACT_IDLE_SPI_HOST;
    return UHAL_STATUS_OK;
}

uhal_status_t spi_host_deinit(const spi_host_inst_t spi_instance) {
}

uhal_status_t spi_start_transaction(const spi_host_inst_t spi_instance, const gpio_pin_t chip_select_pin,
                                    const spi_extra_dev_opt_t device_specific_config_opt) {
    gpio_set_pin_lvl(chip_select_pin, GPIO_LOW);
}

uhal_status_t spi_end_transaction(const spi_host_inst_t spi_instance, const gpio_pin_t chip_select_pin) {
    gpio_set_pin_lvl(chip_select_pin, GPIO_HIGH);
}

uhal_status_t spi_write_blocking(const spi_host_inst_t spi_instance, const unsigned char* write_buff, size_t size) {
    spi_write_non_blocking(spi_instance, write_buff, size);
    spi_wait_for_transaction_finish(&sercom_bustrans_buffer[spi_instance], SERCOMACT_IDLE_SPI_HOST);
}

uhal_status_t spi_write_non_blocking(const spi_host_inst_t spi_instance, const unsigned char* write_buff, size_t size) {
    Sercom* sercom_instance = get_sercom_inst(spi_instance);
    const sercom_num_t sercom_inst_num = spi_instance;
    spi_wait_for_transaction_finish(&sercom_bustrans_buffer[sercom_inst_num], SERCOMACT_IDLE_SPI_HOST);
    sercom_bustrans_buffer[sercom_inst_num].buf_cnt = 0;
    sercom_bustrans_buffer[sercom_inst_num].buf_size = size;
    sercom_bustrans_buffer[sercom_inst_num].write_buffer = write_buff;
    sercom_bustrans_buffer[sercom_inst_num].transaction_type = SERCOMACT_SPI_DATA_TRANSMIT;
    sercom_instance->SPI.INTENSET.reg = SERCOM_SPI_INTENSET_DRE;
}

uhal_status_t spi_read_blocking(const spi_host_inst_t spi_instance, unsigned char* read_buff, size_t amount_of_bytes) {
    const sercom_num_t sercom_inst_num = spi_instance;
    spi_read_non_blocking(spi_instance, read_buff, amount_of_bytes);
    spi_wait_for_transaction_finish(&sercom_bustrans_buffer[sercom_inst_num], SERCOMACT_IDLE_SPI_HOST);
}

uhal_status_t spi_read_non_blocking(const spi_host_inst_t spi_instance, unsigned char* read_buff, size_t amount_of_bytes) {
    Sercom* sercom_instance = get_sercom_inst(spi_instance);
    const sercom_num_t sercom_inst_num = spi_instance;
    spi_wait_for_transaction_finish(&sercom_bustrans_buffer[sercom_inst_num], SERCOMACT_IDLE_SPI_HOST);
    sercom_bustrans_buffer[sercom_inst_num].buf_cnt = 0;
    sercom_bustrans_buffer[sercom_inst_num].buf_size = amount_of_bytes;
    sercom_bustrans_buffer[sercom_inst_num].read_buffer = read_buff;
    sercom_bustrans_buffer[sercom_inst_num].transaction_type = SERCOMACT_SPI_DATA_RECEIVE;
    sercom_instance->SPI.CTRLB.reg |= SERCOM_SPI_CTRLB_RXEN;
    sercom_instance->SPI.DATA.reg = 0;
    sercom_instance->SPI.INTENSET.reg = SERCOM_SPI_INTENSET_DRE;
}
