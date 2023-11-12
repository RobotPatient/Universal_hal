#ifndef SPI_SLAVE_IRQ_HANDLER_H
#define SPI_SLAVE_IRQ_HANDLER_H
#include <stddef.h>
#include <sam.h>
#include "irq/sercom_stuff.h"

void spi_slave_chip_select_irq(const void* hw, volatile bustransaction_t* transaction) {
    ((Sercom*)hw)->SPI.INTFLAG.reg = SERCOM_SPI_INTFLAG_SSL;
    transaction->transaction_type = SERCOMACT_IDLE_SPI_SLAVE;
}

void spi_slave_data_recv_irq(const void* hw, volatile bustransaction_t* transaction) {
    ((Sercom*)hw)->SPI.INTFLAG.reg = SERCOM_SPI_INTFLAG_RXC;
    transaction->transaction_type = SERCOMACT_IDLE_SPI_SLAVE;
}

void spi_slave_data_send_irq(const void* hw, volatile bustransaction_t* transaction) {
    ((Sercom*)hw)->SPI.INTFLAG.reg = SERCOM_SPI_INTFLAG_TXC;
    transaction->transaction_type = SERCOMACT_IDLE_SPI_SLAVE;
}
#endif