/**
* \file            i2c_slave.c
* \brief           Source file which implements the standard I2C API functions
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
#include <stdbool.h>
#include <hal_i2c_slave.h>
#include <stddef.h>
#include <default_irq_handlers.h>

/**
 * @brief Helper function which waits for the SERCOM peripheral to get in sync and finish requested operations.
 *        By continually reading its I2CS syncbusy register.
 * @param hw Pointer to the SERCOM peripheral to be manipulated or read
 * @param bits_to_read The bits to read within the syncbusy register (bitmask)
 * @note Possible bits that can be read are: SERCOM_I2CS_SYNCBUSY_SWRST
 *                                           SERCOM_I2CS_SYNCBUSY_ENABLE
 *                                           SERCOM_I2CS_SYNCBUSY_SYSOP
 * @note This function can only be used for use with I2C client/slave configuration
 */
static inline void i2c_slave_wait_for_sync(const void *const hw, const uint32_t bits_to_read)
{
    while (((Sercom *)hw)->I2CS.SYNCBUSY.reg & bits_to_read) {
    };
}

void disable_i2c_interface(const void *const hw) {
    ((Sercom*)hw)->I2CM.CTRLA.reg &= ~SERCOM_I2CM_CTRLA_ENABLE;
    const uint32_t waitflags = (SERCOM_I2CS_SYNCBUSY_SWRST | SERCOM_I2CS_SYNCBUSY_ENABLE);
    i2c_slave_wait_for_sync(hw, waitflags);
}

void i2c_slave_init(i2c_periph_inst_t *i2c_instance){
    const bool InvalidSercomInstNum = (i2c_instance->sercom_inst_num < 0 || i2c_instance->sercom_inst_num > 5);
    const bool InvalidSercomInst = (i2c_instance->sercom_inst == NULL);
    const bool InvalidClockGen = (i2c_instance->clk_gen_slow < 0 || i2c_instance->clk_gen_slow > 6 || i2c_instance->clk_gen_fast < 0 || i2c_instance->clk_gen_fast > 6);
    if (InvalidSercomInst || InvalidSercomInstNum || InvalidClockGen){
        return;
    }
// Set the clock system
#ifdef __SAMD51__

#else
    PM->APBCMASK.reg |= 1 << (PM_APBCMASK_SERCOM0_Pos + i2c_instance->sercom_inst_num);
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(i2c_instance->clk_gen_slow) | GCLK_CLKCTRL_ID_SERCOMX_SLOW | GCLK_CLKCTRL_CLKEN;
    while (GCLK->STATUS.bit.SYNCBUSY)
        ;
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(i2c_instance->clk_gen_fast) | ((GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + i2c_instance->sercom_inst_num) << GCLK_CLKCTRL_ID_Pos) | GCLK_CLKCTRL_CLKEN;
    GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0x01) | GCLK_GENDIV_ID(i2c_instance->clk_gen_fast);
    while (GCLK->STATUS.bit.SYNCBUSY)
        ;
#endif
    Sercom* SercomInst = i2c_instance->sercom_inst;
    const bool SercomEnabled = SercomInst->I2CM.CTRLA.bit.ENABLE;
    if(SercomEnabled){
        disable_i2c_interface(SercomInst);
    }
    SercomInst->I2CS.CTRLA.reg = (SERCOM_I2CS_CTRLA_SWRST | SERCOM_I2CS_CTRLA_MODE(4));
    i2c_slave_wait_for_sync(SercomInst, SERCOM_I2CS_SYNCBUSY_SWRST);
    SercomInst->I2CS.CTRLA.reg = (1 << SERCOM_I2CS_CTRLA_LOWTOUTEN_Pos      /* SCL Low Time-Out: disabled */
                                  | 0 << SERCOM_I2CS_CTRLA_SCLSM_Pos    /* SCL Clock Stretch Mode: disabled */
                                  | 0 << SERCOM_I2CS_CTRLA_SPEED_Pos    /* Transfer Speed: 0 */
                                  | 1 << SERCOM_I2CS_CTRLA_SEXTTOEN_Pos /* Slave SCL Low Extend Time-Out: disabled */
                                  | 0b10 << SERCOM_I2CS_CTRLA_SDAHOLD_Pos  /* SDA Hold Time: 0 */
                                  | 0 << SERCOM_I2CS_CTRLA_PINOUT_Pos   /* Pin Usage: disabled */
                                  | 0 << SERCOM_I2CS_CTRLA_RUNSTDBY_Pos /* Run In Standby: disabled */
                                  | 4 << SERCOM_I2CS_CTRLA_MODE_Pos);
    SercomInst->I2CS.CTRLB.reg |= SERCOM_I2CS_CTRLB_SMEN;
    i2c_slave_wait_for_sync(SercomInst, SERCOM_I2CS_SYNCBUSY_MASK);
    const uint8_t slave_addr = i2c_instance->i2c_slave_addr;
    SercomInst->I2CS.ADDR.reg = (0 << SERCOM_I2CS_ADDR_ADDRMASK_Pos       /* Address Mask: 0 */
                                 | 0 << SERCOM_I2CS_ADDR_TENBITEN_Pos /* Ten Bit Addressing Enable: disabled */
                                 | 0 << SERCOM_I2CS_ADDR_GENCEN_Pos   /* General Call Address Enable: disabled */
                                 | (slave_addr) << SERCOM_I2CS_ADDR_ADDR_Pos);
    SercomInst->I2CS.CTRLA.reg |= SERCOM_I2CS_CTRLA_ENABLE;
    SercomInst->I2CS.INTENSET.reg = SERCOM_I2CS_INTENSET_AMATCH | SERCOM_I2CS_INTENSET_PREC | SERCOM_I2CS_INTENSET_DRDY;
    const enum IRQn irq_type = (SERCOM0_IRQn + i2c_instance->sercom_inst_num);
    NVIC_EnableIRQ(irq_type);
    NVIC_SetPriority(irq_type, 2);
}

void i2c_slave_deinit(i2c_periph_inst_t *i2c_instance){
    disable_i2c_interface(i2c_instance->sercom_inst);
}