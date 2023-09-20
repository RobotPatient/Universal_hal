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

#define GPIO_OPT_SLEW_RATE_SLOW_POS 2
#define GPIO_OPT_DRIVE_STRENGTH_POS 4
#define GPIO_OPT_PULL_UP_POS 1
#define ADD_GPIO_DRIVE_STENGTH_OPT_OFFSET(x) (x + GPIO_OPT_DRIVE_STRENGTH_POS)

#define REMOVE_IO_OFFSET(x)             (x - GPIO_MODE_INPUT)

void gpio_set_pin_lvl(const gpio_pin_t pin, gpio_level_t level) {
    gpio_put(pin.pin_num, level);
}

void gpio_toggle_pin_output(const gpio_pin_t pin) {
    const uint32_t mask = 1ul << pin.pin_num;
    gpio_xor_mask(mask);
}

gpio_level_t gpio_get_pin_level(const gpio_pin_t pin) {
    return gpio_get(pin.pin_num);
}

void gpio_set_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode) {
    //invalid_params_if(GPIO, ((uint32_t)fn << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB) & ~IO_BANK0_GPIO0_CTRL_FUNCSEL_BITS);
    // Set input enable on, output disable off
    hw_write_masked(&padsbank0_hw->io[pin.pin_num], PADS_BANK0_GPIO0_IE_BITS, PADS_BANK0_GPIO0_IE_BITS | PADS_BANK0_GPIO0_OD_BITS);
    // Zero all fields apart from fsel; we want this IO to do what the peripheral tells it.
    // This doesn't affect e.g. pullup/pulldown, as these are in pad controls.
    if (BITMASK_COMPARE(pin_mode, GPIO_MODE_INPUT) || BITMASK_COMPARE(pin_mode, GPIO_MODE_OUTPUT)) {
        iobank0_hw->io[pin.pin_num].ctrl = GPIO_MODE_F5 << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
        gpio_set_dir(pin.pin_num, REMOVE_IO_OFFSET(pin_mode));
    } else {
        iobank0_hw->io[pin.pin_num].ctrl = pin_mode << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
    }
}

gpio_mode_t gpio_get_pin_mode(const gpio_pin_t pin) {
    uint8_t function;

    function = gpio_get_function(pin.pin_num);
    if (function == GPIO_MODE_F5) {
        function = gpio_get_dir(pin.pin_num) + GPIO_MODE_INPUT;
        return function;
    } else {
        return function;
    }
}

void gpio_set_pin_options(const gpio_pin_t pin, const gpio_opt_t opt) {
    const uint8_t driver_mask_cmp_val = GPIO_OPT_DRIVE_STRENGTH_HIGH | GPIO_OPT_DRIVE_STRENGTH_MEDIUM 
                                        | GPIO_OPT_DRIVE_STRENGTH_LOW | GPIO_OPT_DRIVE_STRENGTH_VLOW;

    switch(BITMASK_COMPARE(opt, driver_mask_cmp_val)) {
        case GPIO_OPT_DRIVE_STRENGTH_HIGH:
        {
            gpio_set_drive_strength(pin.pin_num, GPIO_DRIVE_STRENGTH_12MA);
            break;
        }
        case GPIO_OPT_DRIVE_STRENGTH_MEDIUM:
        {
            gpio_set_drive_strength(pin.pin_num, GPIO_DRIVE_STRENGTH_8MA);
            break;
        }
        case GPIO_OPT_DRIVE_STRENGTH_LOW:
        {
            gpio_set_drive_strength(pin.pin_num, GPIO_DRIVE_STRENGTH_4MA);
            break;
        }
        case GPIO_OPT_DRIVE_STRENGTH_VLOW:
        {
            gpio_set_drive_strength(pin.pin_num, GPIO_DRIVE_STRENGTH_2MA);
            break;
        }
        default:
        {
            break;
        }
    }

    const uint8_t pullup_en = BITMASK_COMPARE(pin.pin_num, GPIO_OPT_PULL_UP);
    const uint8_t pulldown_en = BITMASK_COMPARE(pin.pin_num, GPIO_OPT_PULL_DOWN); 
    gpio_set_pulls(pin.pin_num, pullup_en, pulldown_en);

    const uint8_t slew_rate_val = BITMASK_COMPARE(opt, GPIO_OPT_SLEW_RATE_HIGH) ? GPIO_SLEW_RATE_FAST : GPIO_SLEW_RATE_SLOW;
    gpio_set_slew_rate(pin.pin_num, slew_rate_val);
}

gpio_opt_t gpio_get_pin_options(const gpio_pin_t pin) {
    const uint8_t driver_strength = ADD_GPIO_DRIVE_STENGTH_OPT_OFFSET(gpio_get_drive_strength(pin.pin_num));
    const uint8_t slew_rate = (SHIFT_ONE_LEFT_BY_N( (GPIO_OPT_SLEW_RATE_SLOW_POS + gpio_get_slew_rate(pin.pin_num))));
    const uint8_t pull_down_en = gpio_is_pulled_down(pin.pin_num);
    const uint8_t pull_up_en = gpio_is_pulled_up(pin.pin_num);
    const uint8_t options = (SHIFT_ONE_LEFT_BY_N(driver_strength)) | slew_rate | pull_down_en | (pull_up_en << GPIO_OPT_PULL_UP_POS) ;
    return options;
}

void gpio_set_interrupt_on_pin(const gpio_pin_t pin, gpio_irq_opt_t irq_opt) {
}
