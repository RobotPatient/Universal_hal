#ifndef GPIO_IRQ_HANDLER_H
#define GPIO_IRQ_HANDLER_H
#include <stdint.h>
#include <sam.h>

void gpio_irq_handler(const void* const hw) {
    Eic*           eic_inst = (Eic*)hw;
    const uint32_t intflag_val = eic_inst->INTFLAG.reg;
    eic_inst->INTFLAG.reg = intflag_val;
    const uint32_t nmi_intflag_val = eic_inst->NMIFLAG.reg;
    eic_inst->NMIFLAG.reg = nmi_intflag_val;
}

#endif