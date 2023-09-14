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

/**
 * @brief Toggles output of given gpio pin.
 * @param pin The pin to toggle
 * @note Requires the pin to be set as output first.
 */
void toggle_gpio_pin_output(const gpio_pin_t pin);

/**
 * @brief Sets the gpio pin to INPUT,OUTPUT or special pin-mux functions (platform dependent).
 * @param pin The pin to set to specific mode.
 * @param pin_mode The pin mode to set the pin to.
 */
void set_gpio_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode);

/**
 * @brief Sets the gpio pin output level (HIGH, LOW, HIGH_IMPEDANCE (if supported) ).
 * @param pin The pin to set the output level of.
 * @param level The level: HIGH, LOW, etc...
 */
void set_gpio_pin_lvl(const gpio_pin_t pin, gpio_level_t level);

/**
 * @brief Gets the current input level or set output level (depending on if pin is set to input or output direction).
 * @param pin The pin to get the in/output level from.
 * @return The in/output level of the pin.
 */
gpio_level_t get_gpio_pin_level(const gpio_pin_t pin);

/**
 * @brief Gets the currently set gpio pin-mode (INPUT, OUTPUT, e.g.).
 * @param pin The pin to get the pin-mode from.
 * @return The pin-mode of the pin.
 */
gpio_mode_t get_gpio_pin_mode(const gpio_pin_t pin);

/**
 * @brief Sets gpio pin options like pull-downs, pull-ups and driver strength among things.
 * @param pin The pin to apply the new options to.
 * @param opt The options to set.
 */
void set_gpio_pin_options(const gpio_pin_t pin, const gpio_opt_t opt);

/**
 * @brief Gets the currently set options for the specified pin.
 * @param pin The pin to get the options from.
 * @return The options set for the given pin.
 */
gpio_opt_t get_gpio_pin_options(const gpio_pin_t pin);

/**
 * @brief Setup interrupts on the given pin.
 * @param pin The pin on which the interrupt has to be triggered.
 * @param irq_opt The options needed to setup the pin with interrupts (channels, trigger options (rising, falling, etc...), e.g. )
 */
void set_gpio_interrupt(const gpio_pin_t pin, gpio_irq_opt_t irq_opt);

/**
 * @brief Default GPIO irq handler function.
 * @param hw Pointer to the hw peripheral responsible for the IRQ (Depends on the platform used).
 * @note To use it define this function in one of your own source files (main.c for example), this will override the default handler.
 */
void gpio_irq_handler(const void* const hw) __attribute__((weak));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
