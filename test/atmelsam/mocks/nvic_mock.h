#ifndef NVIC_MOCK_H
#define NVIC_MOCK_H
/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ************************************************************************** */
/**  CMSIS DEFINITIONS FOR SAMD21G18A */
/* ************************************************************************** */
/** \defgroup SAMD21G18A_cmsis CMSIS Definitions */
/*@{*/

/** Interrupt Number Definition */
typedef enum IRQn {
    /******  Cortex-M0+ Processor Exceptions Numbers *******************/
    NonMaskableInt_IRQn = -14, /**<  2 Non Maskable Interrupt      */
    HardFault_IRQn = -13,      /**<  3 Hard Fault Interrupt        */
    SVCall_IRQn = -5,          /**< 11 SV Call Interrupt           */
    PendSV_IRQn = -2,          /**< 14 Pend SV Interrupt           */
    SysTick_IRQn = -1,         /**< 15 System Tick Interrupt       */
    /******  SAMD21G18A-specific Interrupt Numbers *********************/
    PM_IRQn = 0,       /**<  0 SAMD21G18A Power Manager (PM) */
    SYSCTRL_IRQn = 1,  /**<  1 SAMD21G18A System Control (SYSCTRL) */
    WDT_IRQn = 2,      /**<  2 SAMD21G18A Watchdog Timer (WDT) */
    RTC_IRQn = 3,      /**<  3 SAMD21G18A Real-Time Counter (RTC) */
    EIC_IRQn = 4,      /**<  4 SAMD21G18A External Interrupt Controller (EIC) */
    NVMCTRL_IRQn = 5,  /**<  5 SAMD21G18A Non-Volatile Memory Controller (NVMCTRL) */
    DMAC_IRQn = 6,     /**<  6 SAMD21G18A Direct Memory Access Controller (DMAC) */
    USB_IRQn = 7,      /**<  7 SAMD21G18A Universal Serial Bus (USB) */
    EVSYS_IRQn = 8,    /**<  8 SAMD21G18A Event System Interface (EVSYS) */
    SERCOM0_IRQn = 9,  /**<  9 SAMD21G18A Serial Communication Interface 0 (SERCOM0) */
    SERCOM1_IRQn = 10, /**< 10 SAMD21G18A Serial Communication Interface 1 (SERCOM1) */
    SERCOM2_IRQn = 11, /**< 11 SAMD21G18A Serial Communication Interface 2 (SERCOM2) */
    SERCOM3_IRQn = 12, /**< 12 SAMD21G18A Serial Communication Interface 3 (SERCOM3) */
    SERCOM4_IRQn = 13, /**< 13 SAMD21G18A Serial Communication Interface 4 (SERCOM4) */
    SERCOM5_IRQn = 14, /**< 14 SAMD21G18A Serial Communication Interface 5 (SERCOM5) */
    TCC0_IRQn = 15,    /**< 15 SAMD21G18A Timer Counter Control 0 (TCC0) */
    TCC1_IRQn = 16,    /**< 16 SAMD21G18A Timer Counter Control 1 (TCC1) */
    TCC2_IRQn = 17,    /**< 17 SAMD21G18A Timer Counter Control 2 (TCC2) */
    TC3_IRQn = 18,     /**< 18 SAMD21G18A Basic Timer Counter 3 (TC3) */
    TC4_IRQn = 19,     /**< 19 SAMD21G18A Basic Timer Counter 4 (TC4) */
    TC5_IRQn = 20,     /**< 20 SAMD21G18A Basic Timer Counter 5 (TC5) */
    ADC_IRQn = 23,     /**< 23 SAMD21G18A Analog Digital Converter (ADC) */
    AC_IRQn = 24,      /**< 24 SAMD21G18A Analog Comparators (AC) */
    DAC_IRQn = 25,     /**< 25 SAMD21G18A Digital Analog Converter (DAC) */
    PTC_IRQn = 26,     /**< 26 SAMD21G18A Peripheral Touch Controller (PTC) */
    I2S_IRQn = 27,     /**< 27 SAMD21G18A Inter-IC Sound Interface (I2S) */

    PERIPH_COUNT_IRQn = 28 /**< Number of peripheral IDs */
} IRQn_Type;

void NVIC_DisableIRQ(IRQn_Type irq_type) {
}

void NVIC_ClearPendingIRQ(IRQn_Type irq_type) {
}

void NVIC_SetPriority(IRQn_Type irq_type, uint8_t priority);

void NVIC_EnableIRQ(IRQn_Type irq_type);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif