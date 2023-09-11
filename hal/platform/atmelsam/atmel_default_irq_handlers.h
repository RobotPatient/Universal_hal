#ifndef ATMELSAMD21_ATMEL_DEFAULT_IRQ_HANDLERS_H
#define ATMELSAMD21_ATMEL_DEFAULT_IRQ_HANDLERS_H

void SERCOM0_Handler(void) __attribute__((used));
void SERCOM1_Handler(void) __attribute__((used));
void SERCOM2_Handler(void) __attribute__((used));
void SERCOM3_Handler(void) __attribute__((used));
void SERCOM4_Handler(void) __attribute__((used));
void SERCOM5_Handler(void) __attribute__((used));

extern volatile bustransaction_t SercomBusTrans[6];
#endif //ATMELSAMD21_ATMEL_DEFAULT_IRQ_HANDLERS_H
