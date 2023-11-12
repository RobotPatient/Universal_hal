#ifndef I2C_HOST_IRQ_HANDLER_H
#define I2C_HOST_IRQ_HANDLER_H
#include <stdbool.h>
#include <stddef.h>
#include <sam.h>
#include "irq/sercom_stuff.h"
#include "error_handling.h"

/**
 * @brief Macros used in ISR for acknowledging and finishing the transaction
 * SERCOM_I2C_MASTER_RECV_ACK_AND_REQ_NEW_BYTE will send an acknowledgment bit to the slave device and request the next byte to read
 * SERCOM_I2C_MASTER_NACK_AND_STOP will send a NACK to the slave device and stops the transaction by sending a stop bit.
 */
#define SERCOM_I2C_MASTER_RECV_ACK_AND_REQ_NEW_BYTE SERCOM_I2CM_CTRLB_CMD(2) | SERCOM_I2CM_CTRLB_SMEN
#define SERCOM_I2C_MASTER_NACK_AND_STOP             SERCOM_I2CM_CTRLB_CMD(3) | SERCOM_I2CM_CTRLB_ACKACT | SERCOM_I2CM_CTRLB_SMEN


void update_i2c_host_bus_transaction_state(Sercom* sercom_instance, volatile bustransaction_t* transaction) {
  transaction->status = ((sercom_instance->I2CM.STATUS.bit.RXNACK) * UHAL_STATUS_I2C_NACK);
  transaction->status = ((sercom_instance->I2CM.STATUS.bit.LENERR) * UHAL_STATUS_I2C_LENERR);
  transaction->status = ((sercom_instance->I2CM.STATUS.bit.ARBLOST) * UHAL_STATUS_I2C_ARBSTATE_LOST);
  transaction->status = ((sercom_instance->I2CM.STATUS.bit.BUSERR) * UHAL_STATUS_I2C_BUSERR);
}


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
  update_i2c_host_bus_transaction_state(sercom_instance, transaction);
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
  update_i2c_host_bus_transaction_state(sercom_instance, transaction);
}
#endif