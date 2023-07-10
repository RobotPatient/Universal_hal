#ifndef SERCOM_DEP_H
#define SERCOM_DEP_H
#include <stddef.h>
#include <stdint.h>
typedef enum {
    SERCOMACT_NONE,
    SERCOMACT_I2C_DATA_TRANSMIT,
    SERCOMACT_I2C_DATA_RECEIVE
}SercomActions_t;

typedef enum {
    SERCOM_INT_DEINIT,
    SERCOM_INT_I2CM,
    SERCOM_INT_I2CS,
    SERCOM_INT_SPI,
    SERCOM_INT_UART
}SercomInterfaceType_t;

typedef struct {
const uint8_t* buf;
uint8_t buf_size;
uint8_t buf_cnt;
SercomActions_t CurrAction;
SercomInterfaceType_t CurrInterface;
}SercomData_t;

SercomData_t SERCOMData[6] = {{NULL, 0, 0, SERCOMACT_NONE, SERCOM_INT_DEINIT},
                              {NULL, 0, 0, SERCOMACT_NONE, SERCOM_INT_DEINIT},
                              {NULL, 0, 0, SERCOMACT_NONE, SERCOM_INT_DEINIT},
                              {NULL, 0, 0, SERCOMACT_NONE, SERCOM_INT_DEINIT},
                              {NULL, 0, 0, SERCOMACT_NONE, SERCOM_INT_DEINIT},
                              {NULL, 0, 0, SERCOMACT_NONE, SERCOM_INT_DEINIT}};

void i2c_master_handler(const void *const hw, SercomData_t* Data);
#endif