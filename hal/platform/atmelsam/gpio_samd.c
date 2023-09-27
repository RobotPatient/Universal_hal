/**
* \file            gpio_samd.c
* \brief           Source file which implements the standard GPIO API functions
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
#include <sam.h>
#include "bit_manipulation.h"
#include "gpio_platform_specific.h"
#include "hal_gpio.h"

#define GPIO_OPT_PULL_UP_POS             2
#define GPIO_OPT_PULL_DOWN_POS           3
#define GPIO_OPT_SAMPLE_CONTINUOUSLY_POS 5

static inline uhal_status_t check_gpio_pin_parameter(const gpio_pin_t pin) {
    /* Most of the SAMD series mcu have no more than two I/O banks */
    const uint8_t invalid_port_num = (pin.port_num != GPIO_PORT_A || pin.port_num != GPIO_PORT_B);
    /* Pin properties are stored in 32 bit registers. Thus, the pin-number can never be bigger than 31.*/
    const uint8_t invalid_pin_num = (pin.pin_num < 0 || pin.pin_num > 31);
    if (invalid_pin_num || invalid_port_num) {
        return UHAL_STATUS_INVALID_PARAMETERS;
    } else {
        return UHAL_STATUS_OK;
    }
}

uhal_status_t gpio_set_pin_lvl(const gpio_pin_t pin, gpio_level_t level) {
    const int8_t status = check_gpio_pin_parameter(pin);
    if (status != UHAL_STATUS_OK) {
        return status;
    }

    if (level) {
        /*
         * GPIO LEVEL HIGH: Set the OUTSET register to (1 << pin_num).
         * This will set a high output state if a pin is set to the output direction
         * or manually enable pull-ups if the pin is set to output and PULLEN bit is set.
         */
        PORT->Group[pin.port_num].OUTSET.reg = SHIFT_ONE_LEFT_BY_N(pin.pin_num);
    } else {
        /* GPIO LEVEL LOW: Set the OUTCLR register to (1 << pin_num).
         * This will set a low output state if pin is set to the output direction.
         */
        PORT->Group[pin.port_num].OUTCLR.reg = SHIFT_ONE_LEFT_BY_N(pin.pin_num);
    }
    return UHAL_STATUS_OK;
}

uhal_status_t gpio_toggle_pin_output(const gpio_pin_t pin) {
    const int8_t status = check_gpio_pin_parameter(pin);
    if (status != UHAL_STATUS_OK) {
        return status;
    }
    /*
     * The OUTTGL register gets set with a value of (1 << pin_num).
     * This will toggle the output status of the given pin.
     * @note This might cause unwanted behavior if a pin is set as input instead of output.
     */
    PORT->Group[pin.port_num].OUTTGL.reg = SHIFT_ONE_LEFT_BY_N(pin.pin_num);
    return UHAL_STATUS_OK;
}

gpio_level_t gpio_get_pin_level(const gpio_pin_t pin) {
    const int8_t status = check_gpio_pin_parameter(pin);
    if (status != UHAL_STATUS_OK) {
        return status;
    }
    /*
     * The IN register will be read, and the bit corresponding to the pin gets returned.
     * This gives the current input status of the pin.
     */
    const gpio_level_t res = BIT_IS_SET(PORT->Group[pin.port_num].IN.reg, pin.pin_num);
    return res;
}

static inline void prv_set_function(const gpio_pin_t pin, const uint8_t function) {
    /* Enable the internal pin-mux function */
    PORT->Group[pin.port_num].PINCFG[pin.pin_num].bit.PMUXEN = 0x01;
    //    PORT->Group[pin.pin_num].OUTCLR.reg = SHIFT_ONE_LEFT_BY_N(pin.pin_num);
    /* There is a separate pin-mux for even and odd pins (See datasheet)
     * Pin.pin.num has to be shifted to the right by two to divide in two,
     * Then the corresponding even or uneven register can be written.
     *
     * Writing to the PMUXE or PMUXO register sets the function corresponding to
     * the PORT FUNCTION MULTIPLEXING table 7-1. This value goes from group A...H
     * and is mapped to numbers using the function enum.
     */
    if (PIN_IS_EVEN_NUMBER(pin.pin_num)) {
        PORT->Group[pin.port_num].PMUX[pin.pin_num >> 1].bit.PMUXE = function;
    } else {
        PORT->Group[pin.port_num].PMUX[pin.pin_num >> 1].bit.PMUXO = function;
    }
}

static inline void prv_set_dir(const gpio_pin_t pin, const uint8_t direction) {
    PORT->Group[pin.port_num].PINCFG[pin.pin_num].bit.PMUXEN = 0;
    if (direction == GPIO_MODE_OUTPUT) {
        PORT->Group[pin.port_num].PINCFG[pin.pin_num].reg &= ~(PORT_PINCFG_INEN);
        PORT->Group[pin.port_num].DIRSET.reg = SHIFT_ONE_LEFT_BY_N(pin.pin_num);
    } else {
        PORT->Group[pin.port_num].PINCFG[pin.pin_num].reg = PORT_PINCFG_INEN;
        PORT->Group[pin.port_num].DIRCLR.reg = SHIFT_ONE_LEFT_BY_N(pin.pin_num);
    }
}

uhal_status_t gpio_set_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode) {
    const int8_t status = check_gpio_pin_parameter(pin);
    if (status != UHAL_STATUS_OK) {
        return status;
    }
    /*
     * Detect using the offset of GPIO_MODE_INPUT in the enum whether the mode is an input or output
     */
    const uint8_t pin_mode_is_direction = (pin_mode >= GPIO_MODE_INPUT);
    if (pin_mode_is_direction) {
        prv_set_dir(pin, pin_mode);
    } else {
        prv_set_function(pin, pin_mode);
    }
    return UHAL_STATUS_OK;
}

static inline gpio_mode_t prv_get_function(const gpio_pin_t pin) {
    const uint8_t pmux_reg = PORT->Group[pin.port_num].PMUX[pin.pin_num >> 1].reg;
    uint8_t       res;
    if (PIN_IS_EVEN_NUMBER(pin.pin_num)) {
        res = GET_LOWER_4_BITS_OF_BYTE(pmux_reg);
        return res;
    } else {
        res = GET_UPPER_4_BITS_OF_BYTE(pmux_reg);
        return res;
    }
}

static inline gpio_mode_t prv_get_dir(const gpio_pin_t pin) {
    const uint32_t pin_is_set_as_output_pin = BITMASK_COMPARE(PORT->Group[pin.port_num].DIR.reg, SHIFT_ONE_LEFT_BY_N(pin.pin_num));
    if (pin_is_set_as_output_pin) {
        return GPIO_MODE_OUTPUT;
    } else {
        return GPIO_MODE_INPUT;
    }
}

gpio_mode_t gpio_get_pin_mode(const gpio_pin_t pin) {
    const uint8_t pincfg_reg = PORT->Group[pin.port_num].PINCFG[pin.pin_num].reg;
    if (pincfg_reg & PORT_PINCFG_PMUXEN) {
        return prv_get_function(pin);
    } else {
        return prv_get_dir(pin);
    }
}

static inline uint8_t get_non_settable_pincfg_options(const gpio_pin_t pin) {
    const uint8_t prev_pincfg_val = PORT->Group[pin.port_num].PINCFG[pin.pin_num].reg;
    const uint8_t non_settable_opt = (BITMASK_COMPARE(prev_pincfg_val, PORT_PINCFG_PMUXEN) | BITMASK_COMPARE(prev_pincfg_val, PORT_PINCFG_INEN));
    return non_settable_opt;
}

uhal_status_t gpio_set_pin_options(const gpio_pin_t pin, const gpio_opt_t opt) {
    const int8_t status = check_gpio_pin_parameter(pin);
    if (status != UHAL_STATUS_OK) {
        return status;
    }
    /*
     * Some bits in the pincfg register are not set by this function and should not be changed.
     * These bits are retrieved with this function to be included in the final reg_val later.
     */
    const uint8_t non_settable_pincfg_options = get_non_settable_pincfg_options(pin);
    /*
     * Do some trickery. The enum with gpio options has PULL_DOWN and PULL_UP defined.
     * The pull-up register and drive_strength bit positions are aligned with the position
     * in the pincfg register. Thus, minimal operations are needed to translate flags to setting bits.
     *
     * The PULL_DOWN option differs one-bit position from the PULL_UP option, and PULL_UP is aligned with the PULL_EN flag.
     * To enable the PULL_EN flag, we need to just shift one bit to enable the PULL_EN flag :)
     */
    const uint8_t pull_up_en = BITMASK_COMPARE(opt, GPIO_OPT_PULL_UP);
    const uint8_t pull_en = ((BITMASK_COMPARE(opt, GPIO_OPT_PULL_DOWN) >> 1) | (pull_up_en));
    const uint8_t reg_val = non_settable_pincfg_options | pull_en | BITMASK_COMPARE(opt, GPIO_OPT_DRIVE_STRENGTH_HIGH);
    PORT->Group[pin.port_num].PINCFG[pin.pin_num].reg = reg_val;

    /*
     * When the pull-up option is set, we need to do one extra operation.
     * Set the OUTSET register to disable the default pull_down and enable pull_up.
     */
    if (pull_up_en && pull_en) {
        PORT->Group[pin.port_num].OUTSET.reg = SHIFT_ONE_LEFT_BY_N(pin.pin_num);
    }

    /*
     * The sampling options are not set in the pincfg and require to be written to
     * the CTRL register. That is what this section does.
     */
    const uint8_t sampling_opt_set = BITMASK_COMPARE(opt, GPIO_OPT_SAMPLE_CONTINUOUSLY);
    uint32_t      res = PORT->Group[pin.port_num].CTRL.reg;
    if (sampling_opt_set) {
        res |= SHIFT_ONE_LEFT_BY_N(pin.pin_num);
        PORT->Group[pin.port_num].CTRL.reg = res;
    } else {
        res &= ~(SHIFT_ONE_LEFT_BY_N(pin.pin_num));
        PORT->Group[pin.port_num].CTRL.reg = res;
    }
    return UHAL_STATUS_OK;
}

gpio_opt_t gpio_get_pin_options(const gpio_pin_t pin) {
    /*
     * Get the pincfg register to extract the PULL_EN and DRIVE_STR bits from.
     */
    const uint8_t pincfg_register = PORT->Group[pin.port_num].PINCFG[pin.pin_num].reg;

    const uint8_t sampling_opt_en = BIT_IS_SET(PORT->Group[pin.port_num].CTRL.reg, pin.pin_num);

    uint8_t res = BITMASK_COMPARE(pincfg_register, GPIO_OPT_DRIVE_STRENGTH_HIGH) | (sampling_opt_en << GPIO_OPT_SAMPLE_CONTINUOUSLY_POS);

    if (BITMASK_COMPARE(pincfg_register, PORT_PINCFG_PULLEN)) {
        /*
         * Pull-up requires out register to be set... This will be used to distinguish whether a pull-up or pull-down is set.
         */
        const uint8_t pull_up_en = BIT_IS_SET(PORT->Group[pin.port_num].OUT.reg, pin.pin_num);
        const uint8_t pull_down_en = (pull_up_en == 0) << GPIO_OPT_PULL_DOWN_POS; /* PULL_UP is not set, then it must be a PULL_DOWN */
        res |= (pull_down_en) | (pull_up_en << GPIO_OPT_PULL_UP_POS);             /* Add flags to the end result */
        return res;
    }

    return res;
}

static inline uhal_status_t wait_for_eic_gclk_sync() {
    int timeout = 65535;
    int timeout_attempt = 4;
    while (EIC->STATUS.bit.SYNCBUSY) {
        timeout--;
        if (timeout <= 0) {
            if (--timeout_attempt) {
                timeout = 65535;
            } else {
                return UHAL_STATUS_PERIPHERAL_CLOCK_ERROR;
            }
        }
    }
    return UHAL_STATUS_OK;
}

uhal_status_t gpio_set_interrupt_on_pin(const gpio_pin_t pin, gpio_irq_opt_t irq_opt) {
    int8_t status = check_gpio_pin_parameter(pin);
    if (status != UHAL_STATUS_OK) {
        return status;
    }
    /*
     * Check whether pin given is set as output or input. If set as output, make it an input.
     */
    const uint32_t pin_is_set_as_output = BITMASK_COMPARE(PORT->Group[pin.port_num].DIR.reg, SHIFT_ONE_LEFT_BY_N(pin.pin_num));
    if (pin_is_set_as_output) {
        prv_set_dir(pin, GPIO_MODE_INPUT);
    }

    /*
     * Set the pin-mux to GPIO_MODE_A which is the EIC group on most of the SAMD microcontroller lines
     */
    prv_set_function(pin, GPIO_MODE_A);

    /*
     * Set the clock of EIC to the system_clk on the given CLK_GEN
     */
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_ID_EIC | GCLK_CLKCTRL_GEN(irq_opt.irq_clk_generator);

    /*
     * Wait for the peripheral to apply changes..
     */
    status = wait_for_eic_gclk_sync();
    if (status != UHAL_STATUS_OK) {
        return status;
    }

    /*
     * EIC is divided within two sections: GPIO_CONFIG0 AND GPIO_CONFIG1 because the system is limited to 32-bit register sizes.
     * Therefore, we need to decide on the basis of the channel selected which CONFIG register to write.
     */
    uint32_t filter_mask, trigger_mask;
    if (irq_opt.irq_channel <= GPIO_IRQ_CHANNEL_7) {
        /*
         * Calculate the bit_positions of the filter_mask and trigger_mask bits for the specific pin-channel
         * and convert it in to a mask.
         */
        filter_mask = BITMASK_COMPARE(irq_opt.irq_extra_opt, GPIO_IRQ_EXTRA_FILTERING) << ((irq_opt.irq_channel * 4) + (EIC_CONFIG_FILTEN0_Pos));
        trigger_mask = irq_opt.irq_condition << (4 * irq_opt.irq_channel);
        EIC->CONFIG[0].reg |= filter_mask | trigger_mask;
    } else if (irq_opt.irq_channel <= GPIO_IRQ_CHANNEL_15) {
        filter_mask = BITMASK_COMPARE(irq_opt.irq_extra_opt, GPIO_IRQ_EXTRA_FILTERING)
                      << (((irq_opt.irq_channel - GPIO_IRQ_CHANNEL_8) * 4) + (EIC_CONFIG_FILTEN0_Pos));
        trigger_mask = irq_opt.irq_condition << (4 * (irq_opt.irq_channel - GPIO_IRQ_CHANNEL_8));
        EIC->CONFIG[1].reg |= filter_mask | trigger_mask;
    } else {
        filter_mask = BITMASK_COMPARE(irq_opt.irq_extra_opt, GPIO_IRQ_EXTRA_FILTERING) << EIC_NMICTRL_NMIFILTEN_Pos;
        trigger_mask = irq_opt.irq_condition;
        EIC->NMICTRL.reg = filter_mask | trigger_mask;
    }

    if (BITMASK_COMPARE(irq_opt.irq_extra_opt, GPIO_IRQ_WAKE_FROM_SLEEP)) {
        EIC->WAKEUP.reg |= SHIFT_ONE_LEFT_BY_N(irq_opt.irq_channel);
    } else {
        EIC->WAKEUP.reg &= ~(SHIFT_ONE_LEFT_BY_N(irq_opt.irq_channel));
    }
    /*
     * Check whether to use interrupts or events,
     * if events configure as little as possible to make it working :)
     */
    if (BITMASK_COMPARE(irq_opt.irq_extra_opt, GPIO_IRQ_USE_EVENTS)) {
        EIC->INTENCLR.reg = SHIFT_ONE_LEFT_BY_N(irq_opt.irq_channel);
        EIC->EVCTRL.reg |= SHIFT_ONE_LEFT_BY_N(irq_opt.irq_channel);
    } else {
        NVIC_DisableIRQ(EIC_IRQn);
        NVIC_ClearPendingIRQ(EIC_IRQn);

        NVIC_SetPriority(EIC_IRQn, 0);
        NVIC_EnableIRQ(EIC_IRQn);

        /*
         * Set the interrupt for this specific pin.
         */
        EIC->INTENSET.reg = SHIFT_ONE_LEFT_BY_N(irq_opt.irq_channel);
    }
    /*
     * Enable the peripheral
     */
    EIC->CTRL.reg |= EIC_CTRL_ENABLE;

    /*
     * Wait for the peripheral to apply changes..
     */
    status = wait_for_eic_gclk_sync();
    if (status != UHAL_STATUS_OK) {
        return status;
    }

    return UHAL_STATUS_OK;
}