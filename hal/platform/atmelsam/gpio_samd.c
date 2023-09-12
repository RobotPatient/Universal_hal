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

#include <../../hal_gpio.h>
#include "gpio_platform_specific.h"
#include <sam.h>

void SetGPIOPinLevel(const GPIOPin pin, const unsigned char level) {
  if (level) {
    PORT->Group[pin.port_num].OUTSET.reg |= 1 << pin.pin_num;
  } else {
    PORT->Group[pin.port_num].OUTCLR.reg |= 1 << pin.pin_num;
  }
}

void ToggleGPIOPin(const GPIOPin pin) {
  PORT->Group[pin.port_num].OUTTGL.reg |= 1 << pin.pin_num;
}

void SetGPIOPinDirection(const GPIOPin pin, const unsigned char direction) {
  if (direction) {
    PORT->Group[pin.port_num].DIRSET.reg |= 1 << pin.pin_num;
  } else {
    PORT->Group[pin.port_num].DIRCLR.reg |= 1 << pin.pin_num;
  }
}

void SetGPIOPinDriveStrength(const GPIOPin pin, GPIODriveStrength driver_strength) {
  if (driver_strength <= 1 && driver_strength > -1) {
    PORT->Group[pin.port_num].PINCFG[pin.pin_num].bit.DRVSTR = driver_strength;
  }
}

void SetGPIOPinFunction(const GPIOPin pin, GPIOPinFunction pin_function) {
  PORT->Group[pin.port_num].PINCFG[pin.pin_num].bit.PMUXEN = 0x01;  // Enable the pin mux function
  // There is a seperate pin mux for even and odd pin numbers (See datasheet)
  if (pin.pin_num % 2 == 0) {
    PORT->Group[pin.port_num].PMUX[pin.pin_num >> 1].bit.PMUXE |= pin_function;
  } else {
    PORT->Group[pin.port_num].PMUX[pin.pin_num >> 1].bit.PMUXO |= pin_function;
  }
}

void SetGPIOPinSamplingMode(const GPIOPin pin, GPIOSamplingMode sampling_mode) {
  if (sampling_mode == kGPIOSampleContinuously) {
    PORT->Group[pin.port_num].CTRL.reg |= 1 << pin.pin_num;
  } else {
    PORT->Group[pin.port_num].CTRL.reg &= ~(1 << pin.pin_num);
  }
}

GPIOPinLevel GetGPIOPinLevel(const GPIOPin pin) {
  return (PORT->Group[pin.port_num].IN.reg >> pin.pin_num) & 1;
}

GPIOPinDirection GetGPIOPinDirection(const GPIOPin pin) {
  return (PORT->Group[pin.port_num].DIR.reg >> pin.pin_num) & 1;
}

GPIODriveStrength GetGPIODriveStrength(const GPIOPin pin) {
  return (PORT->Group[pin.port_num].PINCFG[pin.pin_num].bit.DRVSTR);
}

GPIOPinFunction GetGPIOPinFunction(const GPIOPin pin) {
  if (pin.pin_num % 2 == 0) {
    return PORT->Group[pin.port_num].PMUX[pin.pin_num >> 1].bit.PMUXE;
  } else {
    return PORT->Group[pin.port_num].PMUX[pin.pin_num >> 1].bit.PMUXO;
  }
}

