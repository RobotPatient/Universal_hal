/* *******************************************************************************************
 * Copyright (c) 2023 by Victor Hogeweij
 *
 * Author: Victor Hogeweij
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 *
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so,
 *
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************************/
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

