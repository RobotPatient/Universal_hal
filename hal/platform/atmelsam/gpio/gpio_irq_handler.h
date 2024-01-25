/**
* \file            gpio_irq_handler.h
* \brief           Header file which implements the default GPIO IRQ Handler
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

#ifndef GPIO_IRQ_HANDLER_H
#define GPIO_IRQ_HANDLER_H

#include <stdint.h>
#include <sam.h>

void gpio_irq_handler(const void *const hw) {
    Eic *eic_inst = (Eic *) hw;
    const uint32_t intflag_val = eic_inst->INTFLAG.reg;
    eic_inst->INTFLAG.reg = intflag_val;
    const uint32_t nmi_intflag_val = eic_inst->NMIFLAG.reg;
    eic_inst->NMIFLAG.reg = nmi_intflag_val;
}

#endif