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
#include "gpio_platform_specific.h"
#include "hal_gpio.h"

#define GPIO_OPT_PULL_UP_POS             2
#define GPIO_OPT_PULL_DOWN_POS           3
#define GPIO_OPT_SAMPLE_CONTINUOUSLY_POS 5

#define VALUE_CONTAINS_MASK(NUM, MASK) (NUM & MASK)
#define BIT_IS_SET(VAL, BIT_NUM) (VAL >> BIT_NUM) & 1
#define SHIFT_N(N) 1 << N

#define PIN_IS_EVEN_NUMBER(x) (x % 2) == 0

void set_gpio_pin_lvl(const gpio_pin_t pin, gpio_level_t level) {
    if (level) {
        PORT->Group[pin.port_num].OUTSET.reg = SHIFT_N(pin.pin_num);
    } else {
        PORT->Group[pin.port_num].OUTCLR.reg = SHIFT_N(pin.pin_num);
    }
}

void toggle_gpio_pin_output(const gpio_pin_t pin) {
    PORT->Group[pin.port_num].OUTTGL.reg = SHIFT_N(pin.pin_num);
}

gpio_level_t get_gpio_pin_level(const gpio_pin_t pin) {
    const gpio_level_t res = BIT_IS_SET(PORT->Group[pin.port_num].IN.reg, pin.pin_num);
    return res;
}

static inline void prv_set_function(const gpio_pin_t pin, const uint8_t function) {
    PORT->Group[pin.port_num].PINCFG[pin.pin_num].bit.PMUXEN = 0x01; /* Enable the pin mux function */
    /* There is a separate pin mux for even and odd pins (See datasheet) */
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
        PORT->Group[pin.port_num].DIRSET.reg = SHIFT_N(pin.pin_num);
    } else {
        PORT->Group[pin.port_num].PINCFG[pin.pin_num].reg = PORT_PINCFG_INEN;
        PORT->Group[pin.port_num].DIRCLR.reg = SHIFT_N(pin.pin_num);
    }
}

void set_gpio_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode) {
    const uint8_t pin_mode_is_direction = (pin_mode >= GPIO_MODE_INPUT);
    if (pin_mode_is_direction) {
        prv_set_dir(pin, pin_mode);
    } else {
        prv_set_function(pin, pin_mode);
    }
}

gpio_mode_t get_gpio_pin_mode(const gpio_pin_t pin) {
    const uint8_t pincfg_reg = PORT->Group[pin.port_num].PINCFG[pin.pin_num].reg;
    if (pincfg_reg & PORT_PINCFG_PMUXEN) {
        const uint8_t pmux_reg = PORT->Group[pin.port_num].PMUX[pin.pin_num >> 1].reg;
        if (PIN_IS_EVEN_NUMBER(pin.pin_num)) {
            const uint8_t res = VALUE_CONTAINS_MASK(pmux_reg, 0xF);
            return res;
        } else {
            const uint8_t res = VALUE_CONTAINS_MASK((pmux_reg >> 4), 0xF);
            return res;
        }
    } else {
        const uint32_t pin_is_set_as_output_pin = PORT->Group[pin.port_num].DIR.reg & (1 << pin.pin_num);
        if (pin_is_set_as_output_pin) {
            return GPIO_MODE_OUTPUT;
        } else {
            return GPIO_MODE_INPUT;
        }
    }
}

void set_gpio_pin_options(const gpio_pin_t pin, const gpio_opt_t opt) {
    const uint8_t prev_pincfg_val = PORT->Group[pin.port_num].PINCFG[pin.pin_num].reg;
    const uint8_t non_settable_opt = (VALUE_CONTAINS_MASK(prev_pincfg_val, PORT_PINCFG_PMUXEN) | VALUE_CONTAINS_MASK(prev_pincfg_val, PORT_PINCFG_INEN));
    const uint8_t pull_up_en = VALUE_CONTAINS_MASK(opt, GPIO_OPT_PULL_UP);
    const uint8_t pull_en = ((VALUE_CONTAINS_MASK(opt, GPIO_OPT_PULL_DOWN) >> 1) |(pull_up_en));
    const uint8_t sampling_opt_set = VALUE_CONTAINS_MASK(opt, GPIO_OPT_SAMPLE_CONTINUOUSLY);
    const uint8_t reg_val = non_settable_opt | pull_en | VALUE_CONTAINS_MASK(opt, GPIO_OPT_DRIVE_STRENGTH_STRONG);
    PORT->Group[pin.port_num].PINCFG[pin.pin_num].reg = reg_val;

    if (pull_up_en && pull_en) {
        PORT->Group[pin.port_num].OUTSET.reg = SHIFT_N(pin.pin_num);
    }

    uint32_t res = PORT->Group[pin.port_num].CTRL.reg;
    if (sampling_opt_set) {
        res |= SHIFT_N(pin.pin_num);
        PORT->Group[pin.port_num].CTRL.reg = res;
    } else {
        res &= ~(SHIFT_N(pin.pin_num));
        PORT->Group[pin.port_num].CTRL.reg = res;
    }
}

gpio_opt_t get_gpio_pin_options(const gpio_pin_t pin) {
    const uint8_t pincfg_register = PORT->Group[pin.port_num].PINCFG[pin.pin_num].reg;
    const uint8_t pull_up_en = BIT_IS_SET(PORT->Group[pin.port_num].OUT.reg, pin.pin_num);
    const uint8_t pull_down_en = (pull_up_en == 0) << GPIO_OPT_PULL_DOWN_POS;
    const uint8_t sampling_opt_en = BIT_IS_SET(PORT->Group[pin.port_num].CTRL.reg, pin.pin_num);

    uint8_t res = VALUE_CONTAINS_MASK(pincfg_register, GPIO_OPT_DRIVE_STRENGTH_STRONG) | (sampling_opt_en << GPIO_OPT_SAMPLE_CONTINUOUSLY_POS);

    if (VALUE_CONTAINS_MASK(pincfg_register, PORT_PINCFG_PULLEN)) {
        res |= (pull_down_en) | (pull_up_en << GPIO_OPT_PULL_UP_POS);
        return res;
    }

    return res;
}
