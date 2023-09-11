#ifndef HAL_I2C_H
#define HAL_I2C_H
#include <i2c_platform_specific.h>
#include <stdbool.h>

void i2c_master_data_recv_irq(const void *const hw, volatile bustransaction_t *Transaction) __attribute__((weak));
void i2c_master_data_send_irq(const void *const hw, volatile bustransaction_t *Transaction) __attribute__((weak));

void i2c_slave_adressmatch_irq(const void *const hw)  __attribute__((weak)) ;
void i2c_slave_stop_irq(const void *const hw)  __attribute__((weak)) ;
void i2c_slave_data_recv_irq(const void *const hw)  __attribute__((weak)) ;
void i2c_slave_data_send_irq(const void *const hw)  __attribute__((weak)) ;

void i2c_init(const i2c_periph_inst_t* i2c_instance, const unsigned long baudrate);

void i2c_deinit(const i2c_periph_inst_t* I2C_instance);

void i2c_set_baudrate(const i2c_periph_inst_t* I2C_instance, const unsigned long baudrate);

void i2c_set_slave_mode(const i2c_periph_inst_t* I2C_instance, const unsigned short addr);

void i2c_write_blocking(const i2c_periph_inst_t* I2C_instance, const unsigned char addr, const unsigned char* write_buff, const unsigned char size, bool stop_bit);

void i2c_write_non_blocking(const i2c_periph_inst_t* I2C_instance, const unsigned short addr, const unsigned char* write_buff, const unsigned char size, bool stop_bit);

void i2c_read_blocking(const i2c_periph_inst_t* I2C_instance, const unsigned short addr, unsigned char* read_buff, const unsigned char amount_of_bytes);

void i2c_read_non_blocking(const i2c_periph_inst_t* I2C_instance, const unsigned short addr, unsigned char* read_buff, const unsigned char amount_of_bytes);


#endif