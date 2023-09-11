#ifndef I2C_MASTER_PLATFORM_SPECIFIC
#define I2C_MASTER_PLATFORM_SPECIFIC

#include <samd21g18a.h>
#include <atmel_irq.h>

typedef enum {
    I2COpModeMaster,
    I2COpModeSlave
}I2COperatingMode;

typedef enum {
    SERCOM_NUM_0,
    SERCOM_NUM_1,
    SERCOM_NUM_2,
    SERCOM_NUM_3,
    SERCOM_NUM_4,
    SERCOM_NUM_5
} SercomNum;

typedef enum {
    CLKGEN_0,
    CLKGEN_1,
    CLKGEN_2,
    CLKGEN_3,
    CLKGEN_4,
    CLKGEN_5,
    CLKGEN_6,
    CLKGEN_7,
    CLKGEN_8
} CLKGEN;

typedef struct {
SercomNum Sercom_inst_num;
Sercom* SercomInst;
CLKGEN ClockGenSlow;
CLKGEN ClockGenFast;
uint32_t ClockFrequency;
I2COperatingMode OpMode;
unsigned short I2CAddr;
}I2CInst;



#endif