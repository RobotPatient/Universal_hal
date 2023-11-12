#ifndef IRQ_BINDINGS_H
#define IRQ_BINDINGS_H
#include <sam.h>

void enable_irq_handler(IRQn_Type irq_type, uint8_t priority);

#endif