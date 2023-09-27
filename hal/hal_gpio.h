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
/**
 * The GPIO Module
 *
 * Possibilities:
 * - Configuring the GPIO direction and pin-muxes (gpio_set_pin_mode)
 * @note pin-mux and direction options differ for each microcontroller variant.
 *       The enum name gpio_mode_t will still be the same. Keep this in mind.
 * - Configuring the GPIO output level (gpio_set_pin_lvl)
 * @note Some microcontrollers will support HIGH impedance mode. 
 *       This will be incorporated in to the gpio_level_t enum.
 * - Configuring GPIO options which are non-standard (like pull-ups, pull-downs, etc.)
 * @note The non-standard gpio options differ for almost every other microcontroller variant.
 * - Configuring GPIO interrupts
 * @note For all the pins, one standard ISR handler is used (gpio_irq_handler). Keep this in mind.
 * @note When implementing this gpio_irq_handler just implement the header in your source file like this:
 * @code 
 * void gpio_irq_handler(const void* const hw) {
 * // Platform specific code to handle the request
 * }
 * @endcode
 * - Reading GPIO input & output levels
 * - Reading the GPIO pin-mux and direction options
 * - Reading which non-standard gpio options are set
 * 
 * Impossibilities:
 * - Although the interface is very generic, the ISR is fully dependent on platform specific code.
 *   Keep this in mind when using the ISR.
 * - To make implementations easier, the choice of having one ISR for all the pins was made.
 *   This means that when you're setting multiple ISR's you need to read from the HW peripheral which
 *   pins are triggered.
 * @todo Maybe add a generic interface for the ISR, as some peripherals can be very complex to use.
 * - Some microcontrollers have ERRATA issues, these might sill cause problems when using this module. 
 *   When that is the case the problems will be listed on the wiki (https://hoog-v.github.io/Universal_hal/).
 * - Some microcontrollers use pin numbers, others use a combination of numbers and letters 
 *   and others use only numbers seperated with a dot (.). This module can't easily guess how to interpret the data.
 *   Therefore a gpio_pin_t struct was brought in to life, which has a platform specific declaration for each mcu variant.
 *   Just keep in mind that you have to make sure your pin declarations are up-to-date. 
 *   Especially while porting between mcu variants.
 * 
 * Using this module:
 * 1. Define a gpio pin: const gpio_pin_t blinky_led = {.port_name= <pin_number>,
 *                                                      .pin_number=<pin_number>};
 *    or depending on if your mcu only has pin numbers: const gpio_pin_t blinky_led = {.pin_number = <pin_number>};
 * 2. Set the gpio pin mode: gpio_set_pin_mode(blinky_led, GPIO_MODE_OUTPUT);
 * 3. Set the gpio pin level: gpio_set_pin_lvl(blinky_led, GPIO_HIGH);
 *    or toggle the gpio: gpio_toggle_pin_output(blinky_led);
 */
#ifndef HAL_GPIO_H
#define HAL_GPIO_H
/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <error_handling.h>
#include <gpio_platform_specific.h>

/**
 * @brief Toggles output of given gpio pin.
 * @param pin The pin to toggle
 * @note Requires the pin to be set as output first.
 */
uhal_status_t gpio_toggle_pin_output(const gpio_pin_t pin);

/**
 * @brief Sets the gpio pin to INPUT,OUTPUT or special pin-mux functions (platform dependent).
 * @param pin The pin to set to specific mode.
 * @param pin_mode The pin mode to set the pin to.
 */
uhal_status_t gpio_set_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode);

/**
 * @brief Sets the gpio pin output level (HIGH, LOW, HIGH_IMPEDANCE (if supported) ).
 * @param pin The pin to set the output level of.
 * @param level The level: HIGH, LOW, etc...
 */
uhal_status_t gpio_set_pin_lvl(const gpio_pin_t pin, gpio_level_t level);

/**
 * @brief Gets the current input level or set output level (depending on if pin is set to input or output direction).
 * @param pin The pin to get the in/output level from.
 * @return The in/output level of the pin.
 */
gpio_level_t gpio_get_pin_level(const gpio_pin_t pin);

/**
 * @brief Gets the currently set gpio pin-mode (INPUT, OUTPUT, e.g.).
 * @param pin The pin to get the pin-mode from.
 * @return The pin-mode of the pin.
 */
gpio_mode_t gpio_get_pin_mode(const gpio_pin_t pin);

/**
 * @brief Sets gpio pin options like pull-downs, pull-ups and driver strength among things.
 * @param pin The pin to apply the new options to.
 * @param opt The options to set.
 */
uhal_status_t gpio_set_pin_options(const gpio_pin_t pin, const gpio_opt_t opt);

/**
 * @brief Gets the currently set options for the specified pin.
 * @param pin The pin to get the options from.
 * @return The options set for the given pin.
 */
gpio_opt_t gpio_get_pin_options(const gpio_pin_t pin);

/**
 * @brief Setup interrupts on the given pin.
 * @param pin The pin on which the interrupt has to be triggered.
 * @param irq_opt The options needed to setup the pin with interrupts (channels, trigger options (rising, falling, etc...), e.g. )
 */
uhal_status_t gpio_set_interrupt_on_pin(const gpio_pin_t pin, gpio_irq_opt_t irq_opt);

/**
 * @brief Default GPIO irq handler function.
 * @param hw Pointer to the hw peripheral responsible for the IRQ (Depends on the platform used).
 * @note To use it define this function in one of your own source files (main.c for example), this will override the default handler.
 */
void gpio_irq_handler(const void *const hw) __attribute__((weak));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif  /* HAL_GPIO_H */
