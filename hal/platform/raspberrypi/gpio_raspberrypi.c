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

#include <hal_gpio.h>
#include <gpio_platform_specific.h>
#include <gpio.h>

void SetGPIOPinDriveStrength(const GPIOPin pin, GPIODriveStrength driver_strength) {
    gpio_set_drive_strength(pin.pin_num, driver_strength);
}

GPIODriveStrength GetGPIODriveStrength(const GPIOPin pin) {
    return gpio_get_drive_strength(pin.pin_num);
}

void SetGPIOPinFunction(const GPIOPin pin, GPIOPinFunction pin_function) {
    gpio_set_function(pin.pin_num, pin_function);
}

void SetGPIOPull(const GPIOPin pin, GPIOPull PullMode) {
    bool up = (PullMode == (kGPIOPullBusKeep || kGPIOPullUp));
    bool down = (PullMode == (kGPIOPullBusKeep || kGPIOPullDown));
    gpio_set_pulls(pin.pin_num, up, down);
}

GPIOPinFunction GetGPIOPinFunction(const GPIOPin pin) {
return gpio_get_function(pin.pin_num);
}

void SetGPIOPinDirection(const GPIOPin pin, const unsigned char direction) {
gpio_set_function(pin.pin_num, GPIO_FUNC_SIO);
gpio_set_dir(pin.pin_num, direction);
}

void SetGPIOPinLevel(const GPIOPin pin, const unsigned char level) {
gpio_put(pin.pin_num, level);
}

GPIOPinDirection GetGPIOPinDirection(const GPIOPin pin) {
return gpio_get_dir(pin.pin_num);
}

GPIOPinLevel GetGPIOPinLevel(GPIOPin pin) {
    return gpio_get(pin.pin_num);
}

void ToggleGPIOPin(const GPIOPin pin) {
    uint32_t mask = 1ul << pin.pin_num;
    gpio_xor_mask(mask);
}

