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
#include <stdbool.h>
#include "bit_manipulation.h"
#include "hal_gpio.h"
#include "hal_i2c_host.h"
#include "i2c_common/i2c_platform_specific.h"

/**
 * @brief Each SERCOM peripheral gets its own SercomBusTransaction.
 *        This will eventually be replaced with a ringbuffer implementation
 *        so the non-blocking functions can function as they should.
 *
 * @todo Replace this implementation with a ringbuffer
 */
volatile bustransaction_t sercom_bustrans_buffer[6] = {{SERCOMACT_NONE, 0, NULL, NULL, 0, 0}, {SERCOMACT_NONE, 0, NULL, NULL, 0, 0},
                                                       {SERCOMACT_NONE, 0, NULL, NULL, 0, 0}, {SERCOMACT_NONE, 0, NULL, NULL, 0, 0},
                                                       {SERCOMACT_NONE, 0, NULL, NULL, 0, 0}, {SERCOMACT_NONE, 0, NULL, NULL, 0, 0}};

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
void i2c_host_data_send_irq(const void* hw, volatile bustransaction_t* transaction) {
    Sercom*    sercom_instance = ((Sercom*)hw);
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

void spi_host_data_send_irq(const void* hw, volatile bustransaction_t* transaction) {
    Sercom* sercom_instance = ((Sercom*)hw);
    if (transaction->transaction_type == SERCOMACT_SPI_DATA_TRANSMIT) {
        const bool write_buffer_exists = (transaction->write_buffer != NULL);
        const bool has_bytes_left_to_write = (transaction->buf_cnt < transaction->buf_size);
        if (write_buffer_exists && has_bytes_left_to_write) {
            sercom_instance->SPI.DATA.reg = transaction->write_buffer[transaction->buf_cnt++];
        } else {
            sercom_instance->SPI.INTFLAG.reg = SERCOM_SPI_INTFLAG_TXC;
            sercom_instance->SPI.INTENCLR.reg = SERCOM_SPI_INTFLAG_DRE;
            transaction->transaction_type = SERCOMACT_IDLE_SPI_HOST;
        }
    } else {
        sercom_instance->SPI.INTFLAG.reg = 0xFF;
    }
}

void spi_host_data_recv_irq(const void* hw, volatile bustransaction_t* transaction) {
    Sercom* sercom_instance = ((Sercom*)hw);
    if (transaction->transaction_type == SERCOMACT_SPI_DATA_RECEIVE) {
        if (transaction->read_buffer != NULL && transaction->buf_cnt < transaction->buf_size) {
            transaction->read_buffer[transaction->buf_cnt++] = sercom_instance->SPI.DATA.reg;
            const bool last_byte_read = transaction->buf_cnt >= transaction->buf_size;
            if (!last_byte_read) {
                sercom_instance->SPI.DATA.reg = 0;
            }
        } else {
            uint32_t reg = sercom_instance->SPI.CTRLB.reg;
            reg &= ~(SERCOM_SPI_CTRLB_RXEN);
            sercom_instance->SPI.CTRLB.reg = reg;
            sercom_instance->SPI.INTENCLR.reg = SERCOM_SPI_INTFLAG_DRE;
            transaction->transaction_type = SERCOMACT_IDLE_SPI_HOST;
        }
    } else {
        sercom_instance->SPI.INTFLAG.reg = 0xFF;
    }
}

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

void i2c_slave_data_recv_irq(const void* const hw, volatile bustransaction_t* Transaction) {
    ((Sercom*)hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_DRDY;
    Transaction->transaction_type = SERCOMACT_IDLE_I2CS;
}

void i2c_slave_data_send_irq(const void* const hw, volatile bustransaction_t* Transaction) {
    ((Sercom*)hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_DRDY;
    Transaction->transaction_type = SERCOMACT_IDLE_I2CS;
}

void i2c_slave_stop_irq(const void* const hw, volatile bustransaction_t* Transaction) {
    ((Sercom*)hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_PREC;
    Transaction->transaction_type = SERCOMACT_IDLE_I2CS;
}

void i2c_slave_address_match_irq(const void* const hw, volatile bustransaction_t* transaction) {
    ((Sercom*)hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_AMATCH;
    transaction->transaction_type = SERCOMACT_IDLE_I2CS;
}

void i2c_host_data_recv_irq(const void* hw, volatile bustransaction_t* transaction) {
    Sercom* sercom_instance = ((Sercom*)hw);
    if (transaction->read_buffer != NULL && transaction->buf_cnt < transaction->buf_size) {
        transaction->read_buffer[transaction->buf_cnt++] = sercom_instance->I2CM.DATA.reg;
        const bool last_byte_read = transaction->buf_cnt >= transaction->buf_size;
        sercom_instance->I2CM.CTRLB.reg = last_byte_read ? SERCOM_I2C_MASTER_NACK_AND_STOP : SERCOM_I2C_MASTER_RECV_ACK_AND_REQ_NEW_BYTE;
    } else {
        sercom_instance->I2CM.CTRLB.reg = SERCOM_I2C_MASTER_NACK_AND_STOP;
        transaction->transaction_type = SERCOMACT_IDLE_I2CM;
        transaction->buf_cnt = 0;
    }
}

void i2c_slave_handler(const void* const hw, volatile bustransaction_t* Transaction) {
    Sercom*    sercom_instance = ((Sercom*)hw);
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

void gpio_irq_handler(const void* const hw) {
    Eic*           eic_inst = (Eic*)hw;
    const uint32_t intflag_val = eic_inst->INTFLAG.reg;
    eic_inst->INTFLAG.reg = intflag_val;
    const uint32_t nmi_intflag_val = eic_inst->NMIFLAG.reg;
    eic_inst->NMIFLAG.reg = nmi_intflag_val;
}

static inline void default_isr_handler(const void* const hw, volatile bustransaction_t* transaction) {
    Sercom* sercom_instance = ((Sercom*)hw);
    switch (transaction->transaction_type) {
        case SERCOMACT_IDLE_I2CS: {
            i2c_slave_handler(sercom_instance, transaction);
            break;
        }
        case SERCOMACT_I2C_DATA_TRANSMIT_NO_STOP:
        case SERCOMACT_I2C_DATA_TRANSMIT_STOP: {
            i2c_host_data_send_irq(sercom_instance, transaction);
            break;
        }
        case SERCOMACT_I2C_DATA_RECEIVE_STOP: {
            i2c_host_data_recv_irq(sercom_instance, transaction);
            break;
        }
        case SERCOMACT_SPI_DATA_RECEIVE: {
            spi_host_data_recv_irq(sercom_instance, transaction);
            break;
        }
        case SERCOMACT_SPI_DATA_TRANSMIT: {
            spi_host_data_send_irq(sercom_instance, transaction);
            break;
        }
        case SERCOMACT_IDLE_SPI_HOST: {
            uint8_t spi_intflag = sercom_instance->SPI.INTFLAG.reg;
            sercom_instance->SPI.INTFLAG.reg = spi_intflag;
            break;
        }
        case SERCOMACT_IDLE_SPI_SLAVE: {
            const uint8_t spi_intflag = sercom_instance->SPI.INTFLAG.reg;
            if (BITMASK_COMPARE(spi_intflag, SERCOM_SPI_INTFLAG_TXC)) {
                spi_slave_data_send_irq(sercom_instance, transaction);
            } else if (BITMASK_COMPARE(spi_intflag, SERCOM_SPI_INTFLAG_RXC)) {
                spi_slave_data_recv_irq(sercom_instance, transaction);
            } else if (BITMASK_COMPARE(spi_intflag, SERCOM_SPI_INTFLAG_SSL)) {
                spi_slave_chip_select_irq(sercom_instance, transaction);
            }
        }
        default: {
            uint8_t SPI_INTFLAG = sercom_instance->SPI.INTFLAG.reg;
            sercom_instance->SPI.INTFLAG.reg = SPI_INTFLAG;
            break;
        }
    }
}

__attribute__((used)) void SERCOM5_Handler(void) {
    volatile bustransaction_t* bustransaction = &sercom_bustrans_buffer[5];
    Sercom*                    sercom_instance = SERCOM5;
    default_isr_handler(sercom_instance, bustransaction);
}

__attribute__((used)) void SERCOM4_Handler(void) {
    volatile bustransaction_t* bustransaction = &sercom_bustrans_buffer[4];
    Sercom*                    sercom_instance = SERCOM4;
    default_isr_handler(sercom_instance, bustransaction);
}

__attribute__((used)) void SERCOM3_Handler(void) {
    volatile bustransaction_t* bustransaction = &sercom_bustrans_buffer[3];
    Sercom*                    sercom_instance = SERCOM3;
    default_isr_handler(sercom_instance, bustransaction);
}

__attribute__((used)) void SERCOM2_Handler(void) {
    volatile bustransaction_t* bustransaction = &sercom_bustrans_buffer[2];
    Sercom*                    sercom_instance = SERCOM2;
    default_isr_handler(sercom_instance, bustransaction);
}

__attribute__((used)) void SERCOM1_Handler(void) {
    volatile bustransaction_t* bustransaction = &sercom_bustrans_buffer[1];
    Sercom*                    sercom_instance = SERCOM1;
    default_isr_handler(sercom_instance, bustransaction);
}

__attribute__((used)) void SERCOM0_Handler(void) {
    volatile bustransaction_t* bustransaction = &sercom_bustrans_buffer[0];
    Sercom*                    sercom_instance = SERCOM0;
    default_isr_handler(sercom_instance, bustransaction);
}

__attribute__((used)) void EIC_Handler(void) {
    gpio_irq_handler(EIC);
}

__attribute__((used)) void NonMaskableInt_Handler(void) {
    gpio_irq_handler(EIC);
}