/**
* \file            uart.c
* \brief           UART driver module source file
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
#include "hal_uart.h"
#include "sam.h"


#define SERCOM_SLOW_CLOCK_SOURCE(x)               (x >> 8)
Sercom* uart_peripheral_mapping[] = {SERCOM0, SERCOM1, SERCOM2, SERCOM3, SERCOM4, SERCOM5};


static inline void uart_wait_for_sync(const void* const hw, const uint32_t bits_to_read) {
    while (((Sercom*)hw)->SPI.SYNCBUSY.reg & bits_to_read) {
        ;
    }
}

static inline uint8_t get_fast_clk_gen_val(const uart_clock_sources_t clock_sources) {
    const uint16_t fast_clk_val = (clock_sources & 0xFF) - 1;
    return fast_clk_val;
}

static inline uint8_t get_slow_clk_gen_val(const uart_clock_sources_t clock_sources) {
    const uint16_t slow_clk_val = SERCOM_SLOW_CLOCK_SOURCE(clock_sources) - 1;
    return slow_clk_val;
}

uhal_status_t uart_init(const uart_peripheral_inst_t uart_peripheral, const uint32_t baudrate, const uart_clock_sources_t clock_source, const uart_extra_config_opt_t uart_exta_opt) {

    // Set the clock system
#ifdef __SAMD51__

#else
PM->APBCMASK.reg |= 1 << (PM_APBCMASK_SERCOM0_Pos + uart_peripheral);
if (clock_source != UART_CLK_SOURCE_USE_DEFAULT) {
        const uint8_t clk_gen_slow = get_slow_clk_gen_val(clock_source);
        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(clk_gen_slow) | GCLK_CLKCTRL_ID_SERCOMX_SLOW | GCLK_CLKCTRL_CLKEN;
        while (GCLK->STATUS.bit.SYNCBUSY);
        const uint8_t clk_gen_fast = get_fast_clk_gen_val(clock_source);
        GCLK->CLKCTRL.reg =
            GCLK_CLKCTRL_GEN(clk_gen_fast) |
            ((GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + uart_peripheral) << GCLK_CLKCTRL_ID_Pos) | GCLK_CLKCTRL_CLKEN;
        GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0x01) | GCLK_GENDIV_ID(clk_gen_fast);
        while (GCLK->STATUS.bit.SYNCBUSY);
} else {
        const uint8_t clk_gen_slow = 3;
        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(clk_gen_slow) | GCLK_CLKCTRL_ID_SERCOMX_SLOW | GCLK_CLKCTRL_CLKEN;
        while (GCLK->STATUS.bit.SYNCBUSY);
        const uint8_t clk_gen_fast = 0;
        GCLK->CLKCTRL.reg =
            GCLK_CLKCTRL_GEN(clk_gen_fast) |
            ((GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + uart_peripheral) << GCLK_CLKCTRL_ID_Pos) | GCLK_CLKCTRL_CLKEN;
        GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0x01) | GCLK_GENDIV_ID(clk_gen_fast);
        while (GCLK->STATUS.bit.SYNCBUSY);
}
#endif
Sercom* sercom_inst = uart_peripheral_mapping[uart_peripheral];
sercom_inst->USART.CTRLA.reg = SERCOM_USART_CTRLA_SWRST;
uart_wait_for_sync(sercom_inst, (SERCOM_USART_SYNCBUSY_SWRST | SERCOM_USART_SYNCBUSY_ENABLE));
sercom_inst->USART.CTRLA.reg = SERCOM_USART_CTRLA_ENABLE | SERCOM_USART_CTRLA_MODE(1) | SERCOM_USART_CTRLA_RXPO(1);
sercom_inst->USART.BAUD.reg = baudrate;
return UHAL_STATUS_OK;
}

uhal_status_t uart_transmit(const uart_peripheral_inst_t uart_peripheral, const uint8_t* transmit_buffer, const size_t size) {
Sercom* sercom_inst = uart_peripheral_mapping[uart_peripheral];
sercom_inst->USART.CTRLB.reg |= SERCOM_USART_CTRLB_TXEN;
for(uint8_t i = 0; i < size; i++ ) {
    sercom_inst->USART.DATA.reg = transmit_buffer[i];
    while(!sercom_inst->USART.INTFLAG.bit.TXC);
}
sercom_inst->USART.CTRLB.reg &= ~SERCOM_USART_CTRLB_TXEN;
return UHAL_STATUS_OK;
}

uhal_status_t uart_receive(const uart_peripheral_inst_t uart_peripheral, uint8_t* receive_buffer, const size_t size) {
Sercom* sercom_inst = uart_peripheral_mapping[uart_peripheral];
sercom_inst->USART.CTRLB.reg |= SERCOM_USART_CTRLB_RXEN;
for(uint8_t i = 0; i < size; i++ ) {
    while(!sercom_inst->USART.INTFLAG.bit.RXC);
    receive_buffer[i] = sercom_inst->USART.DATA.reg;
}
return UHAL_STATUS_OK;
}