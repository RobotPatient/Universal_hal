#include <stdbool.h>
#include <i2c_platform_specific.h>
#include <atmel_default_irq_handlers.h>
#include <hal_i2c.h>

volatile bustransaction_t  SercomBusTrans[6] = {{SERCOMACT_NONE, 0, NULL, NULL, 0, 0},
{SERCOMACT_NONE, 0, NULL, NULL, 0, 0},
{SERCOMACT_NONE, 0, NULL, NULL, 0, 0},
{SERCOMACT_NONE, 0, NULL, NULL, 0, 0},
{SERCOMACT_NONE, 0, NULL, NULL, 0, 0},
{SERCOMACT_NONE, 0, NULL, NULL, 0, 0}};

/*
 * @brief Macros used in ISR for acknowledging and finishing the transaction
 * SERCOM_I2C_MASTER_RECV_ACK_AND_REQ_NEW_BYTE will send an acknowledgment bit to the slave device and request the next byte to read
 * SERCOM_I2C_MASTER_NACK_AND_STOP will send a NACK to the slave device and stops the transaction by sending a stop bit.
 */
#define SERCOM_I2C_MASTER_RECV_ACK_AND_REQ_NEW_BYTE SERCOM_I2CM_CTRLB_CMD(2) | SERCOM_I2CM_CTRLB_SMEN
#define SERCOM_I2C_MASTER_NACK_AND_STOP  SERCOM_I2CM_CTRLB_CMD(3) | SERCOM_I2CM_CTRLB_ACKACT | SERCOM_I2CM_CTRLB_SMEN



void i2c_master_data_send_irq(const void *const hw, volatile bustransaction_t* Transaction ) {
    Sercom* sercom_instance = ((Sercom*)hw);
    const bool write_buffer_exists = (Transaction->write_buffer != NULL);
    const bool has_bytes_left_to_write = (Transaction->buf_cnt < Transaction->buf_size);
    if(write_buffer_exists && has_bytes_left_to_write) {
        sercom_instance->I2CM.DATA.reg = Transaction->write_buffer[Transaction->buf_cnt++];
    } else {
        const bool send_stop_bit = (Transaction->transaction_type != SERCOMACT_I2C_DATA_TRANSMIT_NO_STOP);
        if(send_stop_bit){
            sercom_instance->I2CM.CTRLB.reg = SERCOM_I2C_MASTER_NACK_AND_STOP;
        } else {
            sercom_instance->I2CM.INTFLAG.reg = SERCOM_I2CM_INTFLAG_MB;
        }
        Transaction->transaction_type = SERCOMACT_IDLE_I2CM;
        Transaction->buf_cnt = 0;
    }
}

void i2c_slave_data_recv_irq(const void *const hw, volatile bustransaction_t* Transaction) {
    ((Sercom*)hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_DRDY;
    Transaction->transaction_type = SERCOMACT_IDLE_I2CS;
}

void i2c_slave_data_send_irq(const void *const hw, volatile bustransaction_t* Transaction) {
    ((Sercom*)hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_DRDY;
    Transaction->transaction_type = SERCOMACT_IDLE_I2CS;
}

void i2c_slave_stop_irq(const void *const hw, volatile bustransaction_t* Transaction) {
    ((Sercom*)hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_PREC;
    Transaction->transaction_type = SERCOMACT_IDLE_I2CS;
}
void i2c_slave_address_match_irq(const void *const hw, volatile bustransaction_t* Transaction) {
    ((Sercom*)hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_AMATCH;
    Transaction->transaction_type = SERCOMACT_IDLE_I2CS;
}

void i2c_master_data_recv_irq(const void *const hw, volatile bustransaction_t* Transaction){
    Sercom* sercom_instance = ((Sercom*)hw);
    if(Transaction->read_buffer != NULL && Transaction->buf_cnt < Transaction->buf_size) {
        Transaction->read_buffer[Transaction->buf_cnt++] = sercom_instance->I2CM.DATA.reg;
        const bool last_byte_read = Transaction->buf_cnt >= Transaction->buf_size;
        sercom_instance->I2CM.CTRLB.reg = last_byte_read ? SERCOM_I2C_MASTER_NACK_AND_STOP : SERCOM_I2C_MASTER_RECV_ACK_AND_REQ_NEW_BYTE;
    } else {
        sercom_instance->I2CM.CTRLB.reg = SERCOM_I2C_MASTER_NACK_AND_STOP;
        Transaction->transaction_type = SERCOMACT_IDLE_I2CM;
        Transaction->buf_cnt = 0;
    }
}

void i2c_slave_handler(const void *const hw, volatile bustransaction_t* Transaction) {
    Sercom* sercom_instance = ((Sercom*)hw);
    const bool addressMatchInt = sercom_instance->I2CS.INTFLAG.reg & SERCOM_I2CS_INTFLAG_AMATCH;
    const bool stopInt = sercom_instance->I2CS.INTFLAG.reg & SERCOM_I2CS_INTFLAG_PREC;
    const bool dataReadyInt = sercom_instance->I2CS.INTFLAG.reg & SERCOM_I2CS_INTFLAG_DRDY;
    const bool isReadTransaction = sercom_instance->I2CS.STATUS.bit.DIR;
    if(addressMatchInt) {
        i2c_slave_address_match_irq(hw, Transaction);
    }
    if(stopInt) {
        i2c_slave_stop_irq(hw, Transaction);
    }
    if(dataReadyInt && isReadTransaction) {
        i2c_slave_data_send_irq(hw, Transaction);
    }
    if(dataReadyInt && !isReadTransaction) {
        i2c_slave_data_recv_irq(hw, Transaction);
    }
}

__attribute__((used)) void SERCOM5_Handler(void)
{
    if(SERCOM5->I2CM.INTFLAG.bit.SB) {
        i2c_master_data_recv_irq(SERCOM5, &SercomBusTrans[5]);
    } else if (SERCOM5->I2CM.INTFLAG.bit.MB) {
        i2c_master_data_send_irq(SERCOM5, &SercomBusTrans[5]);
    } else if(SERCOM5->I2CS.INTFLAG.bit.AMATCH || SERCOM5->I2CS.INTFLAG.bit.DRDY || SERCOM5->I2CS.INTFLAG.bit.PREC) {
        i2c_slave_handler(SERCOM5, &SercomBusTrans[5]);
    }
}

__attribute__((used)) void SERCOM4_Handler(void)
{
    if(SERCOM4->I2CM.INTFLAG.bit.SB) {
        i2c_master_data_recv_irq(SERCOM4, &SercomBusTrans[4]);
    } else if (SERCOM4->I2CM.INTFLAG.bit.MB) {
        i2c_master_data_send_irq(SERCOM4, &SercomBusTrans[4]);
    } else if(SERCOM4->I2CS.INTFLAG.bit.AMATCH || SERCOM4->I2CS.INTFLAG.bit.DRDY || SERCOM4->I2CS.INTFLAG.bit.PREC) {
        i2c_slave_handler(SERCOM4, &SercomBusTrans[4]);
    }
}

__attribute__((used)) void SERCOM3_Handler(void)
{
    if(SERCOM3->I2CM.INTFLAG.bit.SB) {
        i2c_master_data_recv_irq(SERCOM3, &SercomBusTrans[3]);
    } else if (SERCOM3->I2CM.INTFLAG.bit.MB) {
        i2c_master_data_send_irq(SERCOM3, &SercomBusTrans[3]);
    } else if(SERCOM3->I2CS.INTFLAG.bit.AMATCH || SERCOM3->I2CS.INTFLAG.bit.DRDY || SERCOM3->I2CS.INTFLAG.bit.PREC) {
        i2c_slave_handler(SERCOM3, &SercomBusTrans[3]);
    }
}

__attribute__((used)) void SERCOM2_Handler(void)
{
    if(SERCOM2->I2CM.INTFLAG.bit.SB) {
        i2c_master_data_recv_irq(SERCOM2, &SercomBusTrans[2]);
    } else if (SERCOM2->I2CM.INTFLAG.bit.MB) {
        i2c_master_data_send_irq(SERCOM2, &SercomBusTrans[2]);
    } else if(SERCOM2->I2CS.INTFLAG.bit.AMATCH || SERCOM2->I2CS.INTFLAG.bit.DRDY || SERCOM2->I2CS.INTFLAG.bit.PREC) {
        i2c_slave_handler(SERCOM2, &SercomBusTrans[2]);
    }
}

__attribute__((used)) void SERCOM1_Handler(void)
{
    if(SERCOM1->I2CM.INTFLAG.bit.SB) {
        i2c_master_data_recv_irq(SERCOM1, &SercomBusTrans[1]);
    } else if (SERCOM1->I2CM.INTFLAG.bit.MB) {
        i2c_master_data_send_irq(SERCOM1, &SercomBusTrans[1]);
    } else if(SERCOM1->I2CS.INTFLAG.bit.AMATCH || SERCOM1->I2CS.INTFLAG.bit.DRDY || SERCOM1->I2CS.INTFLAG.bit.PREC) {
        i2c_slave_handler(SERCOM1, &SercomBusTrans[1]);
    }
}

__attribute__((used)) void SERCOM0_Handler(void)
{
    if(SERCOM0->I2CM.INTFLAG.bit.SB) {
        i2c_master_data_recv_irq(SERCOM0, &SercomBusTrans[0]);
    } else if (SERCOM0->I2CM.INTFLAG.bit.MB) {
        i2c_master_data_send_irq(SERCOM0, &SercomBusTrans[0]);
    } else if(SERCOM0->I2CS.INTFLAG.bit.AMATCH || SERCOM0->I2CS.INTFLAG.bit.DRDY || SERCOM0->I2CS.INTFLAG.bit.PREC) {
        i2c_slave_handler(SERCOM0, &SercomBusTrans[0]);
    }
}
