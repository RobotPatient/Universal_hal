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
#include <stdbool.h>
#include "gpio_platform_specific.h"
#include "hal_gpio.h"

#define GPIO_OPT_PULL_UP_POS             2
#define GPIO_OPT_PULL_DOWN_POS           3
#define GPIO_OPT_SAMPLE_CONTINUOUSLY_POS 5

void set_gpio_pin_lvl(const gpio_pin_t pin, gpio_level_t level) {
    if (level) {
        PORT->Group[pin.port_num].OUTSET.reg = (1 << pin.pin_num);
    } else {
        PORT->Group[pin.port_num].OUTCLR.reg = (1 << pin.pin_num);
    }
}

void toggle_gpio_pin_output(const gpio_pin_t pin) {
    PORT->Group[pin.port_num].OUTTGL.reg = 1 << pin.pin_num;
}

const gpio_level_t get_gpio_pin_level(const gpio_pin_t pin) {
    return (PORT->Group[pin.port_num].IN.reg >> pin.pin_num) & 1;
}

void set_gpio_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode) {
    const uint8_t pin_mode_is_direction = (pin_mode >= GPIO_MODE_INPUT);
    if (pin_mode_is_direction) {
        PORT->Group[pin.port_num].PINCFG[pin.pin_num].bit.PMUXEN = 0;
        if (pin_mode == GPIO_MODE_OUTPUT) {
            PORT->Group[pin.port_num].PINCFG[pin.pin_num].reg &= ~(PORT_PINCFG_INEN);
            PORT->Group[pin.port_num].DIRSET.reg = 1 << pin.pin_num;
        } else {
            PORT->Group[pin.port_num].PINCFG[pin.pin_num].reg = PORT_PINCFG_INEN;
            PORT->Group[pin.port_num].DIRCLR.reg = 1 << pin.pin_num;
        }
    } else {
        PORT->Group[pin.port_num].PINCFG[pin.pin_num].bit.PMUXEN = 0x01; /* Enable the pin mux function */
        /* There is a seperate pin mux for even and odd pin numbers (See datasheet) */
        if (pin.pin_num % 2 == 0) {
            PORT->Group[pin.port_num].PMUX[pin.pin_num >> 1].bit.PMUXE = pin_mode;
        } else {
            PORT->Group[pin.port_num].PMUX[pin.pin_num >> 1].bit.PMUXO = pin_mode;
        }
    }
}

const gpio_mode_t get_gpio_pin_mode(const gpio_pin_t pin) {
    const uint8_t pincfg_reg = PORT->Group[pin.port_num].PINCFG[pin.pin_num].reg;
    if (pincfg_reg & PORT_PINCFG_PMUXEN) {
        const uint8_t PMUX_RESULT = PORT->Group[pin.port_num].PMUX[pin.pin_num >> 1].reg;
        if (pin.pin_num % 2) {
            const uint8_t res = PMUX_RESULT & 0xF;
            return res;
        } else {
            const uint8_t res = (PMUX_RESULT >> 4) & 0xF;
            return res;
        }
    } else {
        const uint32_t pin_is_set_as_output_pin = PORT->Group[pin.port_num].DIR.reg;
        if (pin_is_set_as_output_pin) {
            return GPIO_MODE_OUTPUT;
        } else {
            return GPIO_MODE_INPUT;
        }
    }
}

void set_gpio_pin_options(const gpio_pin_t pin, const gpio_opt_t opt) {
    const uint8_t prev_pincfg_val = PORT->Group[pin.port_num].PINCFG[pin.pin_num].reg;
    const uint8_t non_settable_opt = ((prev_pincfg_val & PORT_PINCFG_PMUXEN) | (prev_pincfg_val & PORT_PINCFG_INEN));
    const uint8_t pullup = (opt & GPIO_OPT_PULL_UP);
    const uint8_t pullen = ((opt & GPIO_OPT_PULL_DOWN) >> 1 | (pullup));
    const uint8_t reg_val = non_settable_opt | pullen | (opt & GPIO_OPT_DRIVE_STRENGTH_STRONG);
    PORT->Group[pin.port_num].PINCFG[pin.pin_num].reg = reg_val;

    if (pullup && pullen) {
        PORT->Group[pin.port_num].OUTSET.reg = 1 << pin.pin_num;
    }

    const uint8_t sampling_opt_set = opt & GPIO_OPT_SAMPLE_CONTINUOUSLY;
    uint32_t res = PORT->Group[pin.port_num].CTRL.reg;
    if (sampling_opt_set) {
        res |= 1 << pin.pin_num;
        PORT->Group[pin.port_num].CTRL.reg = res;
    } else {
        res &= ~(1 << pin.pin_num);
        PORT->Group[pin.port_num].CTRL.reg = res;
    }
}

gpio_opt_t get_gpio_pin_options(const gpio_pin_t pin) {
    const uint8_t pincfg_register = PORT->Group[pin.port_num].PINCFG[pin.pin_num].reg;
    const uint8_t pullup_en = (PORT->Group[pin.port_num].OUT.reg & (1 << pin.pin_num)) >> pin.pin_num;
    const uint8_t pulldown_en = (pullup_en == 0) << GPIO_OPT_PULL_DOWN_POS;
    const uint8_t sampling_opt_en = (PORT->Group[pin.port_num].CTRL.reg & (1 << pin.pin_num)) >> (pin.pin_num);

    uint8_t res  = (pincfg_register & GPIO_OPT_DRIVE_STRENGTH_STRONG)  | (sampling_opt_en << GPIO_OPT_SAMPLE_CONTINUOUSLY_POS);

    if (pincfg_register & PORT_PINCFG_PULLEN) {
        res |= (pulldown_en) | (pullup_en << GPIO_OPT_PULL_UP_POS);
        return res;
    }

    return res;
}
