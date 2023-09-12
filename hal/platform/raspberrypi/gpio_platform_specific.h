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
 */

#ifndef GPIO_PLATFORM_SPECIFIC
#define GPIO_PLATFORM_SPECIFIC
#include <gpio.h>

#define SUPPORT_PIN_PULL 1
#define HAS_NO_GPIO_PORT_DESIGNATORS 1
#define SUPPORT_DRIVE_STRENGTH_SETTING 1
#define SUPPORT_PINMUX 1
#define SUPPORT_PIN_TOGGLE 1
#define SUPPORT_PIN_SAMPLING_MODE_SELECT 0

#define GPIODriveStrength enum gpio_drive_strength

#define GPIOPinFunction enum gpio_function

typedef enum {
  kGPIOPullBusKeep,
  kGPIOPullDown,
  kGPIOPullUp,
  kGPIONoPull
} GPIOPull;


typedef struct {
  unsigned char pin_num;
} GPIOPin;
#endif
