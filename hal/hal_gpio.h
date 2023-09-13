/**
* \file            hal_gpio.h
* \brief           GPIO module include file
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

#ifndef GPIO_HPP
#define GPIO_HPP
/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <gpio_error_handling.h>
#include <gpio_platform_specific.h>


void toggle_gpio_pin_output(const gpio_pin_t pin);

void set_gpio_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode);

void set_gpio_pin_lvl(const gpio_pin_t pin, gpio_level_t level);

gpio_level_t get_gpio_pin_level(const gpio_pin_t pin);

gpio_mode_t get_gpio_pin_mode(const gpio_pin_t pin);

void set_gpio_pin_options(const gpio_pin_t pin, const gpio_opt_t opt);

gpio_opt_t get_gpio_pin_options(const gpio_pin_t pin);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
