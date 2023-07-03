#ifndef HAL_I2C_H
#define HAL_I2C_H
#include <i2c_platform_specific.h>
#include <stdbool.h>

void i2c_init(I2CInst* I2C_instance, unsigned long baudate);

void i2c_deinit(I2CInst* I2C_instance);

void i2c_set_baudrate(I2CInst* I2C_instance, unsigned long baudate);

void i2c_set_slave_mode(I2CInst* I2C_instance, unsigned short addr);

void i2c_write_blocking(I2CInst* I2C_instance, const unsigned short addr, const unsigned char* write_buff, const unsigned char size, bool stop_bit);

void i2c_write_non_blocking(I2CInst* I2C_instance, const unsigned short addr, const unsigned char* write_buff, const unsigned char size, bool stop_bit);

char i2c_read_blocking(I2CInst* I2C_instance, const unsigned short addr, unsigned char* read_buff);

char i2c_read_non_blocking(I2CInst* I2C_instance, const unsigned short addr, unsigned char* read_buff);

void i2c_attach_read_interrupt(I2CInst* I2C_instance, void* cb_method);

void i2c_attach_write_interrupt(I2CInst* I2C_instance, void* cb_method);


#endif