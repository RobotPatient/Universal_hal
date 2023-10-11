/**
* \file            spi_slave.c
* \brief           Source file which implements the standard SPI API functions
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
#include "hal_spi_slave.h"

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
    while (((Sercom*)hw)->SPI.SYNCBUSY.reg & bits_to_read) {};
}

uhal_status_t spi_slave_init(const spi_slave_dev_t* spi_instance) {
    const spi_periph_inst_t* spi_peripheral_inst = spi_instance->spi_peripheral;
    const uint8_t            invalid_sercom_instance_num = (spi_peripheral_inst->sercom_inst_num > SERCOM_INST_NUM - 1);
    const uint8_t            hw_handle_is_null = (spi_peripheral_inst->sercom_inst == NULL);
    const uint8_t            invalid_clk_gen_num =
        (spi_peripheral_inst->clk_gen_slow > GCLK_GEN_NUM - 1 || spi_peripheral_inst->clk_gen_fast > GCLK_GEN_NUM - 1);
    if (hw_handle_is_null || invalid_sercom_instance_num || invalid_clk_gen_num) {
        return;
    }
    // Set the clock system
#ifdef __SAMD51__

#else
    PM->APBCMASK.reg |= 1 << (PM_APBCMASK_SERCOM0_Pos + spi_peripheral_inst->sercom_inst_num);
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(spi_peripheral_inst->clk_gen_slow) | GCLK_CLKCTRL_ID_SERCOMX_SLOW | GCLK_CLKCTRL_CLKEN;
    while (GCLK->STATUS.bit.SYNCBUSY)
        ;
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(spi_peripheral_inst->clk_gen_fast)
                        | ((GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + spi_peripheral_inst->sercom_inst_num) << GCLK_CLKCTRL_ID_Pos) | GCLK_CLKCTRL_CLKEN;
    GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0x01) | GCLK_GENDIV_ID(spi_peripheral_inst->clk_gen_fast);
    while (GCLK->STATUS.bit.SYNCBUSY)
        ;
#endif
    spi_peripheral_inst->sercom_inst->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_SWRST;
    spi_wait_for_sync(spi_peripheral_inst->sercom_inst, SERCOM_SPI_SYNCBUSY_SWRST | SERCOM_SPI_SYNCBUSY_ENABLE);
    spi_peripheral_inst->sercom_inst->SPI.CTRLA.reg =
        SERCOM_SPI_CTRLA_MODE(spi_peripheral_inst->operating_mode) | (spi_instance->clock_polarity << SERCOM_SPI_CTRLA_CPHA_Pos)
        | (spi_instance->data_order << SERCOM_SPI_CTRLA_CPOL_Pos) | (SERCOM_SPI_CTRLA_DIPO(spi_peripheral_inst->dipo_pad))
        | (SERCOM_SPI_CTRLA_DOPO(spi_peripheral_inst->dopo_pad));
    spi_peripheral_inst->sercom_inst->SPI.CTRLB.reg = SERCOM_SPI_CTRLB_PLOADEN | SERCOM_SPI_CTRLB_CHSIZE(spi_instance->character_size);
    spi_peripheral_inst->sercom_inst->SPI.INTENSET.reg = SERCOM_SPI_INTENSET_RXC | SERCOM_SPI_INTENSET_TXC | SERCOM_SPI_INTENSET_SSL;
    spi_peripheral_inst->sercom_inst->SPI.CTRLA.reg |= SERCOM_SPI_CTRLA_ENABLE;
    spi_wait_for_sync(spi_peripheral_inst->sercom_inst, SERCOM_SPI_SYNCBUSY_ENABLE);
    const enum IRQn irq_type = (SERCOM0_IRQn + spi_peripheral_inst->sercom_inst_num);
    NVIC_EnableIRQ(irq_type);
    NVIC_SetPriority(irq_type, 2);
    sercom_bustrans_buffer[spi_peripheral_inst->sercom_inst_num].transaction_type = SERCOMACT_IDLE_SPI_SLAVE;
}

uhal_status_t spi_slave_deinit(const spi_slave_dev_t* spi_instance) {
}