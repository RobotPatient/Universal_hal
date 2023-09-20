/**
* \file            default_irq_handlers.c
* \brief           Source file with default interrupt handlers for MICROCHIP SAM peripherals
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
#include "default_irq_handlers.h"
#include "hal_i2c.h"
#include "hal_gpio.h"
#include "i2c_platform_specific.h"
#include <stdbool.h>

/**
 * @brief Each SERCOM peripheral gets its own SercomBusTransaction.
 *        This will eventually be replaced with a ringbuffer implementation
 *        so the non-blocking functions can function as they should.
 *
 * @todo Replace this implementation with a ringbuffer
 */
volatile bustransaction_t SercomBusTrans[6] = {{SERCOMACT_NONE, 0, NULL, NULL, 0, 0},
                                               {SERCOMACT_NONE, 0, NULL, NULL, 0, 0},
                                               {SERCOMACT_NONE, 0, NULL, NULL, 0, 0},
                                               {SERCOMACT_NONE, 0, NULL, NULL, 0, 0},
                                               {SERCOMACT_NONE, 0, NULL, NULL, 0, 0},
                                               {SERCOMACT_NONE, 0, NULL, NULL, 0, 0}};

/**
 * @brief Macros used in ISR for acknowledging and finishing the transaction
 * SERCOM_I2C_MASTER_RECV_ACK_AND_REQ_NEW_BYTE will send an acknowledgment bit to the slave device and request the next byte to read
 * SERCOM_I2C_MASTER_NACK_AND_STOP will send a NACK to the slave device and stops the transaction by sending a stop bit.
 */
#define SERCOM_I2C_MASTER_RECV_ACK_AND_REQ_NEW_BYTE SERCOM_I2CM_CTRLB_CMD(2) | SERCOM_I2CM_CTRLB_SMEN
#define SERCOM_I2C_MASTER_NACK_AND_STOP             SERCOM_I2CM_CTRLB_CMD(3) | SERCOM_I2CM_CTRLB_ACKACT | SERCOM_I2CM_CTRLB_SMEN

/**
 * @brief Default IRQ Handler for the I2C master data send interrupt
 * @param hw Pointer to the HW peripheral to be manipulated
 * @param transaction The current transaction information
 */
void i2c_master_data_send_irq(const void *const hw, volatile bustransaction_t *transaction) {
    Sercom *sercom_instance = ((Sercom *) hw);
    const bool write_buffer_exists = (transaction->write_buffer != NULL);
    const bool has_bytes_left_to_write = (transaction->buf_cnt < transaction->buf_size);
    if (write_buffer_exists && has_bytes_left_to_write) {
        sercom_instance->I2CM.DATA.reg = transaction->write_buffer[transaction->buf_cnt++];
    } else {
        const bool send_stop_bit = (transaction->transaction_type != SERCOMACT_I2C_DATA_TRANSMIT_NO_STOP);
        if (send_stop_bit) {
            sercom_instance->I2CM.CTRLB.reg = SERCOM_I2C_MASTER_NACK_AND_STOP;
        } else {
            sercom_instance->I2CM.INTFLAG.reg = SERCOM_I2CM_INTFLAG_MB;
        }
        transaction->transaction_type = SERCOMACT_IDLE_I2CM;
        transaction->buf_cnt = 0;
    }
}


void spi_master_handler(const void *const hw, volatile bustransaction_t *transaction) {
    Sercom *sercom_instance = ((Sercom *) hw);
    if(transaction->transaction_type == SERCOMACT_SPI_DATA_TRANSMIT) {
        const bool write_buffer_exists = (transaction->write_buffer != NULL);
        const bool has_bytes_left_to_write = (transaction->buf_cnt < transaction->buf_size);
        if (write_buffer_exists && has_bytes_left_to_write) {
            sercom_instance->SPI.DATA.reg = transaction->write_buffer[transaction->buf_cnt++];
        } else {
            sercom_instance->SPI.INTFLAG.reg = SERCOM_SPI_INTFLAG_TXC;
            sercom_instance->SPI.INTENCLR.reg = SERCOM_SPI_INTFLAG_DRE;
            transaction->transaction_type = SERCOMACT_IDLE_SPI;
        }
    } else if(transaction->transaction_type == SERCOMACT_SPI_DATA_RECEIVE) {
        if (transaction->read_buffer != NULL && transaction->buf_cnt < transaction->buf_size) {
            transaction->read_buffer[transaction->buf_cnt++] = sercom_instance->SPI.DATA.reg;
            const bool last_byte_read = transaction->buf_cnt >= transaction->buf_size;
            if(!last_byte_read) {
                sercom_instance->SPI.DATA.reg = 0;
            }
        } else {
            uint32_t reg = sercom_instance->SPI.CTRLB.reg;
            reg &= ~(SERCOM_SPI_CTRLB_RXEN);
            sercom_instance->SPI.CTRLB.reg = reg;
            sercom_instance->SPI.INTENCLR.reg = SERCOM_SPI_INTFLAG_DRE;
            transaction->transaction_type = SERCOMACT_IDLE_SPI;
        }
    } else {
        sercom_instance->SPI.INTFLAG.reg = 0xFF;
    }
}

void i2c_slave_data_recv_irq(const void *const hw, volatile bustransaction_t *Transaction) {
    ((Sercom *) hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_DRDY;
    Transaction->transaction_type = SERCOMACT_IDLE_I2CS;
}

void i2c_slave_data_send_irq(const void *const hw, volatile bustransaction_t *Transaction) {
    ((Sercom *) hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_DRDY;
    Transaction->transaction_type = SERCOMACT_IDLE_I2CS;
}

void i2c_slave_stop_irq(const void *const hw, volatile bustransaction_t *Transaction) {
    ((Sercom *) hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_PREC;
    Transaction->transaction_type = SERCOMACT_IDLE_I2CS;
}

void i2c_slave_address_match_irq(const void *const hw, volatile bustransaction_t *transaction) {
    ((Sercom *) hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_AMATCH;
    transaction->transaction_type = SERCOMACT_IDLE_I2CS;
}

void i2c_master_data_recv_irq(const void *const hw, volatile bustransaction_t *transaction) {
    Sercom *sercom_instance = ((Sercom *) hw);
    if (transaction->read_buffer != NULL && transaction->buf_cnt < transaction->buf_size) {
        transaction->read_buffer[transaction->buf_cnt++] = sercom_instance->I2CM.DATA.reg;
        const bool last_byte_read = transaction->buf_cnt >= transaction->buf_size;
        sercom_instance->I2CM.CTRLB.reg = last_byte_read ? SERCOM_I2C_MASTER_NACK_AND_STOP :
                                          SERCOM_I2C_MASTER_RECV_ACK_AND_REQ_NEW_BYTE;
    } else {
        sercom_instance->I2CM.CTRLB.reg = SERCOM_I2C_MASTER_NACK_AND_STOP;
        transaction->transaction_type = SERCOMACT_IDLE_I2CM;
        transaction->buf_cnt = 0;
    }
}

void i2c_slave_handler(const void *const hw, volatile bustransaction_t *Transaction) {
    Sercom *sercom_instance = ((Sercom *) hw);
    const bool addressMatchInt = sercom_instance->I2CS.INTFLAG.reg & SERCOM_I2CS_INTFLAG_AMATCH;
    const bool stopInt = sercom_instance->I2CS.INTFLAG.reg & SERCOM_I2CS_INTFLAG_PREC;
    const bool dataReadyInt = sercom_instance->I2CS.INTFLAG.reg & SERCOM_I2CS_INTFLAG_DRDY;
    const bool isReadTransaction = sercom_instance->I2CS.STATUS.bit.DIR;
    if (addressMatchInt) {
        i2c_slave_address_match_irq(hw, Transaction);
    }
    if (stopInt) {
        i2c_slave_stop_irq(hw, Transaction);
    }
    if (dataReadyInt && isReadTransaction) {
        i2c_slave_data_send_irq(hw, Transaction);
    }
    if (dataReadyInt && !isReadTransaction) {
        i2c_slave_data_recv_irq(hw, Transaction);
    }
}

void gpio_irq_handler(const void *const hw) {
    Eic *eic_inst = (Eic *) hw;
    const uint32_t intflag_val = eic_inst->INTFLAG.reg;
    eic_inst->INTFLAG.reg = intflag_val;
    const uint32_t nmi_intflag_val = eic_inst->NMIFLAG.reg;
    eic_inst->NMIFLAG.reg = nmi_intflag_val;
}

__attribute__((used)) void SERCOM5_Handler(void) {
    if (SERCOM5->I2CM.INTFLAG.bit.SB) {
        i2c_master_data_recv_irq(SERCOM5, &SercomBusTrans[5]);
    } else if (SERCOM5->I2CM.INTFLAG.bit.MB) {
        i2c_master_data_send_irq(SERCOM5, &SercomBusTrans[5]);
    } else if (SERCOM5->I2CS.INTFLAG.bit.AMATCH || SERCOM5->I2CS.INTFLAG.bit.DRDY || SERCOM5->I2CS.INTFLAG.bit.PREC) {
        i2c_slave_handler(SERCOM5, &SercomBusTrans[5]);
    }
}

__attribute__((used)) void SERCOM4_Handler(void) {
    if (SERCOM4->I2CM.INTFLAG.bit.SB) {
        i2c_master_data_recv_irq(SERCOM4, &SercomBusTrans[4]);
    } else if (SERCOM4->I2CM.INTFLAG.bit.MB) {
        i2c_master_data_send_irq(SERCOM4, &SercomBusTrans[4]);
    } else if (SERCOM4->I2CS.INTFLAG.bit.AMATCH || SERCOM4->I2CS.INTFLAG.bit.DRDY || SERCOM4->I2CS.INTFLAG.bit.PREC) {
        i2c_slave_handler(SERCOM4, &SercomBusTrans[4]);
    }
}

__attribute__((used)) void SERCOM3_Handler(void) {
    if (SERCOM3->I2CM.INTFLAG.bit.SB) {
        i2c_master_data_recv_irq(SERCOM3, &SercomBusTrans[3]);
    } else if (SERCOM3->I2CM.INTFLAG.bit.MB) {
        i2c_master_data_send_irq(SERCOM3, &SercomBusTrans[3]);
    } else if (SERCOM3->I2CS.INTFLAG.bit.AMATCH || SERCOM3->I2CS.INTFLAG.bit.DRDY || SERCOM3->I2CS.INTFLAG.bit.PREC) {
        i2c_slave_handler(SERCOM3, &SercomBusTrans[3]);
    }
}

__attribute__((used)) void SERCOM2_Handler(void) {
//    if (SERCOM2->I2CM.INTFLAG.bit.SB) {
//        i2c_master_data_recv_irq(SERCOM2, &SercomBusTrans[2]);
//    } else if (SERCOM2->I2CM.INTFLAG.bit.MB) {
//        i2c_master_data_send_irq(SERCOM2, &SercomBusTrans[2]);
//    } else if (SERCOM2->I2CS.INTFLAG.bit.AMATCH || SERCOM2->I2CS.INTFLAG.bit.DRDY || SERCOM2->I2CS.INTFLAG.bit.PREC) {
//        i2c_slave_handler(SERCOM2, &SercomBusTrans[2]);
//    } else
    if (SERCOM2->SPI.INTFLAG.bit.TXC || SERCOM2->SPI.INTFLAG.bit.DRE || SERCOM2->SPI.INTFLAG.bit.RXC) {
        spi_master_handler(SERCOM2, &SercomBusTrans[2]);
    }
}

__attribute__((used)) void SERCOM1_Handler(void) {
    if (SERCOM1->I2CM.INTFLAG.bit.SB) {
        i2c_master_data_recv_irq(SERCOM1, &SercomBusTrans[1]);
    } else if (SERCOM1->I2CM.INTFLAG.bit.MB) {
        i2c_master_data_send_irq(SERCOM1, &SercomBusTrans[1]);
    } else if (SERCOM1->I2CS.INTFLAG.bit.AMATCH || SERCOM1->I2CS.INTFLAG.bit.DRDY || SERCOM1->I2CS.INTFLAG.bit.PREC) {
        i2c_slave_handler(SERCOM1, &SercomBusTrans[1]);
    }
}

__attribute__((used)) void SERCOM0_Handler(void) {
    if (SERCOM0->I2CM.INTFLAG.bit.SB) {
        i2c_master_data_recv_irq(SERCOM0, &SercomBusTrans[0]);
    } else if (SERCOM0->I2CM.INTFLAG.bit.MB) {
        i2c_master_data_send_irq(SERCOM0, &SercomBusTrans[0]);
    } else if (SERCOM0->I2CS.INTFLAG.bit.AMATCH || SERCOM0->I2CS.INTFLAG.bit.DRDY || SERCOM0->I2CS.INTFLAG.bit.PREC) {
        i2c_slave_handler(SERCOM0, &SercomBusTrans[0]);
    }
}

__attribute__((used)) void EIC_Handler(void) {
    gpio_irq_handler(EIC);
}

__attribute__((used)) void NonMaskableInt_Handler(void) {
    gpio_irq_handler(EIC);
}