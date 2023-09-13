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
#include <gpio_platform_specific.h>
#include <gpio_error_handling.h>

typedef enum {
    kGPIODirInput,
    kGPIODirOutput
}GPIOPinDirection;

typedef enum {
    kGPIOLow,
    kGPIOHigh,
}GPIOPinLevel;

#if SUPPORT_PIN_PULL
void SetGPIOPull(const GPIOPin pin, GPIOPull PullMode);
#endif

#if SUPPORT_DRIVE_STRENGTH_SETTING
void SetGPIOPinDriveStrength(const GPIOPin pin, GPIODriveStrength driver_strength);
GPIODriveStrength GetGPIODriveStrength(const GPIOPin pin);
#endif


#if SUPPORT_PINMUX
void SetGPIOPinFunction(const GPIOPin pin, GPIOPinFunction pin_function);
GPIOPinFunction GetGPIOPinFunction(const GPIOPin pin);
#endif

#if SUPPORT_PIN_SAMPLING_MODE_SELECT
void SetGPIOPinSamplingMode(const GPIOPin pin, GPIOSamplingMode sampling_mode);
#endif

#if SUPPORT_PIN_TOGGLE
void ToggleGPIOPin(const GPIOPin pin);
#endif

void SetGPIOPinDirection(const GPIOPin pin, const unsigned char direction);

void SetGPIOPinLevel(const GPIOPin pin, const unsigned char level);

GPIOPinDirection GetGPIOPinDirection(const GPIOPin pin);

GPIOPinLevel GetGPIOPinLevel(GPIOPin pin);

#endif
