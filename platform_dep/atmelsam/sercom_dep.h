#ifndef SERCOM_DEP_H
#define SERCOM_DEP_H
#include <stddef.h>
#include <stdint.h>
typedef enum {
    SERCOMACT_NONE,
    SERCOMACT_I2C_DATA_TRANSMIT_NO_STOP,
    SERCOMACT_I2C_DATA_TRANSMIT_STOP,
    SERCOMACT_I2C_DATA_RECEIVE_STOP
}SercomActions_t;

typedef enum {
    SERCOM_INT_DEINIT,
    SERCOM_INT_I2CM,
    SERCOM_INT_I2CS,
    SERCOM_INT_SPI,
    SERCOM_INT_UART
}SercomInterfaceType_t;



#endif