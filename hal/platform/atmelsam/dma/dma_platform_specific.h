/**
* \file            dma_platform_specific.h
* \brief           DMA module platform defines include file
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
#ifndef ATMELSAMD21_DMA_PLATFORM_SPECIFIC_H
#define ATMELSAMD21_DMA_PLATFORM_SPECIFIC_H
#include <sam.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
    DMA_CHANNEL_0,
    DMA_CHANNEL_1,
    DMA_CHANNEL_2,
    DMA_CHANNEL_3,
    DMA_CHANNEL_4,
    DMA_CHANNEL_5,
    DMA_CHANNEL_6,
    DMA_CHANNEL_7,
    DMA_CHANNEL_8,
    DMA_CHANNEL_9,
    DMA_CHANNEL_10,
    DMA_CHANNEL_11,
} dma_channel_t;

typedef enum {
    DMA_OPT_USE_DEFAULT = 0,
    DMA_OPT_BEAT_SIZE_8_BITS = 1,
    DMA_OPT_BEAT_SIZE_16_BITS = 2,
    DMA_OPT_BEAT_SIZE_32_BITS = 3,
    DMA_OPT_ENABLE_CRC_16 = 4,
    DMA_OPT_ENABLE_CRC_32 = 8,
    DMA_OPT_IRQ_SUSPEND = 16,
    DMA_OPT_IRQ_TRANSFER_COMPLETE = 32,
    DMA_OPT_IRQ_TRANSFER_ERROR = 48,
    DMA_OPT_STEP_SIZE_2 = 64,
    DMA_OPT_STEP_SIZE_4 = 128,
    DMA_OPT_STEP_SIZE_8 = 192,
    DMA_OPT_STEP_SIZE_16 = 256,
    DMA_OPT_STEP_SIZE_32 = 320,
    DMA_OPT_STEP_SIZE_64 = 384,
    DMA_OPT_STEP_SIZE_128 = 448,
    DMA_OPT_DISABLE_SRC_INCREMENT = 512,
    DMA_OPT_DISABLE_DST_INCREMENT = 1024,
    DMA_OPT_APPLY_STEP_SIZE_TO_SRC = 2048,
    DMA_OPT_BLOCKACT_INT = 4096,
    DMA_OPT_BLOCKACT_SUSPEND = 8196,
    DMA_OPT_BLOCKACT_BOTH = 12292,
    DMA_OPT_EVENT_OUTPUT_BLOCK = 16384,
    DMA_OPT_EVENT_OUTPUT_BEAT = 32768,
} dma_opt_t;

typedef enum {
    DMA_TRIGGER_SOFTWARE,
    DMA_TRIGGER_SERCOM0_RX,
    DMA_TRIGGER_SERCOM0_TX,
    DMA_TRIGGER_SERCOM1_RX,
    DMA_TRIGGER_SERCOM1_TX,
    DMA_TRIGGER_SERCOM2_RX,
    DMA_TRIGGER_SERCOM2_TX,
    DMA_TRIGGER_SERCOM3_RX,
    DMA_TRIGGER_SERCOM3_TX,
    DMA_TRIGGER_SERCOM4_RX,
    DMA_TRIGGER_SERCOM4_TX,
    DMA_TRIGGER_SERCOM5_RX,
    DMA_TRIGGER_SERCOM5_TX,
    DMA_TRIGGER_TCC0_OVF,
    DMA_TRIGGER_TCC0_MC0,
    DMA_TRIGGER_TCC0_MC1,
    DMA_TRIGGER_TCC0_MC2,
    DMA_TRIGGER_TCC0_MC3,
    DMA_TRIGGER_TCC1_OVF,
    DMA_TRIGGER_TCC1_MC0,
    DMA_TRIGGER_TCC1_MC1,
    DMA_TRIGGER_TCC2_OVF,
    DMA_TRIGGER_TCC2_MC0,
    DMA_TRIGGER_TCC2_MC1,
    DMA_TRIGGER_TC3_OVF,
    DMA_TRIGGER_TC3_MC0,
    DMA_TRIGGER_TC3_MC1,
    DMA_TRIGGER_TC4_OVF,
    DMA_TRIGGER_TC4_MC0,
    DMA_TRIGGER_TC4_MC1,
    DMA_TRIGGER_TC5_OVF,
    DMA_TRIGGER_TC5_MC0,
    DMA_TRIGGER_TC5_MC1,
    DMA_TRIGGER_TC6_OVF,
    DMA_TRIGGER_TC6_MC0,
    DMA_TRIGGER_TC6_MC1,
    DMA_TRIGGER_TC7_OVF,
    DMA_TRIGGER_TC7_MC0,
    DMA_TRIGGER_TC7_MC1,
    DMA_TRIGGER_ADC_RESRDY,
    DMA_TRIGGER_DAC_EMPTY,
    DMA_TRIGGER_I2S_RX_0,
    DMA_TRIGGER_I2S_RX_1,
    DMA_TRIGGER_I2S_TX_0,
    DMA_TRIGGER_I2S_TX_1,
    DMA_TRIGGER_TCC3_OVF,
    DMA_TRIGGER_TCC3_MC0,
    DMA_TRIGGER_TCC3_MC1,
    DMA_TRIGGER_TCC3_MC2,
    DMA_TRIGGER_TCC3_MC3
} dma_trigger_t;

typedef enum {
    DMA_PERIPHERAL_0
} dma_peripheral_t;

typedef enum {
    DMA_INIT_OPT_USE_DEFAULT = 0,
    DMA_INIT_OPT_HANDLE_PRIORITY_LEVEL_1,
    DMA_INIT_OPT_HANDLE_PRIORITY_LEVEL_2,
    DMA_INIT_OPT_HANDLE_PRIORITY_LEVEL_3,
    DMA_INIT_OPT_HANDLE_PRIORITY_LEVEL_4,
    DMA_INIT_OPT_HANDLE_PRIORITY_LEVEL_5,
    DMA_INIT_OPT_HANDLE_PRIORITY_LEVEL_6,
    DMA_INIT_OPT_HANDLE_PRIORITY_LEVEL_7,
    DMA_INIT_OPT_HANDLE_PRIORITY_LEVEL_8,
    DMA_INIT_OPT_HANDLE_PRIORITY_LEVEL_9,
    DMA_INIT_OPT_HANDLE_PRIORITY_LEVEL_10,
    DMA_INIT_OPT_HANDLE_PRIORITY_LEVEL_11,
    DMA_INIT_OPT_HANDLE_PRIORITY_LEVEL_12,
    DMA_INIT_OPT_HANDLE_PRIORITY_LEVEL_13,
    DMA_INIT_OPT_HANDLE_PRIORITY_LEVEL_14,
    DMA_INIT_OPT_HANDLE_PRIORITY_LEVEL_15,
    DMA_INIT_OPT_HANDLE_PRIORITY_LEVEL_16,
    DMA_INIT_OPT_USE_IRQ_PRIORITY_0 = 0x20,
    DMA_INIT_OPT_USE_IRQ_PRIORITY_1 = 0x30,
    DMA_INIT_OPT_USE_IRQ_PRIORITY_2 = 0X40,
    DMA_INIT_OPT_USE_IRQ_PRIORITY_3 = 0x50
} dma_init_opt_t;

typedef enum {
    DMA_PERIPHERAL_LOCATION_I2C_0 = 0,
    DMA_PERIPHERAL_LOCATION_SPI_0 = 0,
    DMA_PERIPHERAL_LOCATION_UART_0 = 0,
    DMA_PERIPHERAL_LOCATION_I2C_1 = 1,
    DMA_PERIPHERAL_LOCATION_SPI_1 = 1,
    DMA_PERIPHERAL_LOCATION_UART_1 = 1,
    DMA_PERIPHERAL_LOCATION_I2C_2 = 2,
    DMA_PERIPHERAL_LOCATION_SPI_2 = 2,
    DMA_PERIPHERAL_LOCATION_UART_2 = 2,
    DMA_PERIPHERAL_LOCATION_I2C_3 = 3,
    DMA_PERIPHERAL_LOCATION_SPI_3 = 3,
    DMA_PERIPHERAL_LOCATION_UART_3 = 3,
    DMA_PERIPHERAL_LOCATION_I2C_4 = 4,
    DMA_PERIPHERAL_LOCATION_SPI_4 = 4,
    DMA_PERIPHERAL_LOCATION_UART_4 = 4,
    DMA_PERIPHERAL_LOCATION_I2C_5 = 5,
    DMA_PERIPHERAL_LOCATION_SPI_5 = 5,
    DMA_PERIPHERAL_LOCATION_UART_5 = 5
} dma_peripheral_location_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#define DMA_INIT_FUNC_PARAMETER_CHECK(dma_peripheral, dma_init_options)                                                                              \
    do {                                                                                                                                             \
    } while (0);

#define DMA_SET_TRANSFER_MEM_FUNC_PARAMETER_CHECK(dma_peripheral, dma_channel, src, dst, size, dma_options, do_software_trigger)                     \
    do {                                                                                                                                             \
    } while (0);

#define DMA_SET_TRANSFER_PERIPHERAL_TO_MEM_FUNC_PARAMETER_CHECK(dma_peripheral, dma_channel, src, dst, size, dma_options, do_software_trigger)       \
    do {                                                                                                                                             \
    } while (0);

#define DMA_SET_TRANSFER_MEM_TO_PERIPHERAL_FUNC_PARAMETER_CHECK(dma_peripheral, dma_channel, src, dst, size, dma_options, do_software_trigger)       \
    do {                                                                                                                                             \
    } while (0);

#define DMA_TRIGGER_FUNC_PARAMETER_CHECK(dma_peripheral, dma_channel, trigger)                                                                       \
    do {                                                                                                                                             \
    } while (0);

#define DMA_DEINIT_FUNC_PARAMETER_CHECK(dma_peripheral)                                                                                              \
    do {                                                                                                                                             \
    } while (0);

#endif //ATMELSAMD21_DMA_PLATFORM_SPECIFIC_H
