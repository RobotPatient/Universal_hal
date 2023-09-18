/**
* \file            spi.c
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

#include <hal_spi.h>
#include <stddef.h>
#include <default_irq_handlers.h>

void spi_init(const spi_dev_t *spi_instance, unsigned long baud_rate) {
    const spi_periph_inst_t *spi_peripheral_inst = spi_instance->spi_peripheral;
    const uint8_t invalid_sercom_instance_num = (spi_peripheral_inst->sercom_inst_num > SERCOM_INST_NUM - 1);
    const uint8_t hw_handle_is_null = (spi_peripheral_inst->sercom_inst == NULL);
    const uint8_t invalid_clk_gen_num = (spi_peripheral_inst->clk_gen_slow > GCLK_GEN_NUM - 1 ||
                                         spi_peripheral_inst->clk_gen_fast > GCLK_GEN_NUM - 1);
    if (hw_handle_is_null || invalid_sercom_instance_num || invalid_clk_gen_num) {
        return;
    }
        // Set the clock system
#ifdef __SAMD51__

#else
    PM->APBCMASK.reg |= 1 << (PM_APBCMASK_SERCOM0_Pos + spi_peripheral_inst->sercom_inst_num);
    GCLK->CLKCTRL.reg =
            GCLK_CLKCTRL_GEN(spi_peripheral_inst->clk_gen_slow) | GCLK_CLKCTRL_ID_SERCOMX_SLOW | GCLK_CLKCTRL_CLKEN;
    while (GCLK->STATUS.bit.SYNCBUSY);
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(spi_peripheral_inst->clk_gen_fast) |
                        ((GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + spi_peripheral_inst->sercom_inst_num)
                                << GCLK_CLKCTRL_ID_Pos) | GCLK_CLKCTRL_CLKEN;
    GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0x01) | GCLK_GENDIV_ID(spi_peripheral_inst->clk_gen_fast);
    while (GCLK->STATUS.bit.SYNCBUSY);
#endif
}

void spi_deinit(const spi_dev_t *spi_instance) {

}

void spi_write_blocking(const spi_dev_t *spi_instance, const unsigned char *write_buff, size_t size) {

}

void spi_write_non_blocking(const spi_dev_t *spi_instance, const unsigned char *write_buff, size_t size) {

}

void spi_read_blocking(const spi_dev_t *spi_instance, unsigned char *read_buff, size_t amount_of_bytes) {

}

void spi_read_non_blocking(const spi_dev_t *spi_instance, unsigned char *read_buff, size_t amount_of_bytes) {

}