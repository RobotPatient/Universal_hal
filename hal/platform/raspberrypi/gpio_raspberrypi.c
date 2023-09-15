/**
* \file            gpio_raspberrypi.c
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
#include <gpio.h>
#include "bit_manipulation.h"
#include "gpio_platform_specific.h"
#include "hal_gpio.h"
#include "hardware/sync.h"

#include "hardware/irq.h"
#include "hardware/structs/iobank0.h"

#if LIB_PICO_BINARY_INFO
#include "pico/binary_info.h"
#endif

#define GPIO_OPT_PULL_UP_POS             0
#define GPIO_OPT_PULL_DOWN_POS           1
#define GPIO_OPT_SAMPLE_CONTINUOUSLY_POS 4

#define REMOVE_IO_OFFSET(x)              x - GPIO_MODE_INPUT

void set_gpio_pin_lvl(const gpio_pin_t pin, gpio_level_t level) {
    gpio_put(pin, level);
}

void toggle_gpio_pin_output(const gpio_pin_t pin) {
    const uint32_t mask = 1ul << pin.pin_num;
    gpio_xor_mask(mask);
}

gpio_level_t get_gpio_pin_level(const gpio_pin_t pin) {
    return gpio_get(pin.pin_num);
}

void set_gpio_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode) {
    check_gpio_param(pin.pin_num);
    //invalid_params_if(GPIO, ((uint32_t)fn << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB) & ~IO_BANK0_GPIO0_CTRL_FUNCSEL_BITS);
    // Set input enable on, output disable off
    hw_write_masked(&padsbank0_hw->io[gpio], PADS_BANK0_GPIO0_IE_BITS, PADS_BANK0_GPIO0_IE_BITS | PADS_BANK0_GPIO0_OD_BITS);
    // Zero all fields apart from fsel; we want this IO to do what the peripheral tells it.
    // This doesn't affect e.g. pullup/pulldown, as these are in pad controls.
    if (BITMASK_COMPARE(pin_mode, GPIO_MODE_INPUT) || BITMASK_COMPARE(pin_mode, GPIO_MODE_OUTPUT)) {
        iobank0_hw->io[gpio].ctrl = GPIO_MODE_F5 << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
        gpio_set_dir(pin.pin_num, REMOVE_IO_OFFSET(pin_mode));
    } else {
        iobank0_hw->io[gpio].ctrl = fn << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
    }
}

gpio_mode_t get_gpio_pin_mode(const gpio_pin_t pin) {
    uint8_t function;

    function = gpio_get_function(pin.pin_num);
    if (function == GPIO_MODE_F5) {
        function = gpio_get_dir(pin.pin_num) + GPIO_MODE_INPUT;
        return function;
    } else {
        return function;
    }
}

void set_gpio_pin_options(const gpio_pin_t pin, const gpio_opt_t opt) {
}

gpio_opt_t get_gpio_pin_options(const gpio_pin_t pin) {
    return GPIO_OPT_DRIVE_STRENGTH_HIGH;
}

void set_gpio_interrupt(const gpio_pin_t pin, gpio_irq_opt_t irq_opt) {
}