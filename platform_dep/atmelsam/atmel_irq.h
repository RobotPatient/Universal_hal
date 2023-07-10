#ifndef ATMEL_IRQ_H
#define ATMEL_IRQ_H

#include "sam.h"


void i2c_slave_handler(const void *const hw);

void SERCOM5_Handler(void);

void SERCOM4_Handler(void);

void SERCOM3_Handler(void);

void SERCOM2_Handler(void);

void SERCOM1_Handler(void);

#endif