/**
* \file            hal_dma.h
* \brief           DMA module include file
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
#ifndef ATMELSAMD21_HAL_DMA_H
#define ATMELSAMD21_HAL_DMA_H

#include <error_handling.h>
#include "dma/dma_platform_specific.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Function to initialize the internal hardware DMA peripheral
 * @param dma_peripheral The DMA peripheral instance to use (some devices have multiple DMA peripherals)
 * @param dma_init_options The platform dependent init options -> See dma_platform_specific.h
 * @return The status of the operation: UHAL_STATUS_OK means it was successful.
 */
uhal_status_t dma_init(dma_peripheral_t dma_peripheral, dma_init_opt_t dma_init_options);



/**
 * @brief Function to initialize a DMA transaction from memory to memory location
 * @param peripheral The DMA peripheral instance to use (some devices have multiple DMA peripherals)
 * @param dma_channel The DMA peripheral channel to use
 * @param src The address to copy the bytes from
 * @param dst The address to copy the bytes to
 * @param size The amount of elements to copy
 * @param dma_options The platform dependent options like:
 *                    - Element size (32-, 16- or 8-bit)
 *                    - Interrupts when transfer is complete
 *                    - Other options that may apply to this transaction See -> dma_platform_specific.h
 * @param do_software_trigger Whether or not to trigger the DMA immediately using a software trigger
 * @return UHAL_STATUS_OK when no errors have occurred
 */
uhal_status_t dma_set_transfer_mem(const dma_peripheral_t peripheral,
                                   const dma_channel_t dma_channel,
                                   const void *src,
                                   void *dst,
                                   const size_t size,
                                   const dma_opt_t dma_options,
                                   const uint8_t do_software_trigger);

/**
 * @brief Function to initialize a DMA transaction/channel to transfer data from a peripheral to a memory location.
 * @param dma_peripheral The DMA peripheral instance to use (some devices have multiple DMA peripherals)
 * @param dma_channel The DMA peripheral channel to use
 * @param src The external hardware peripheral to copy from
 * @param dst The memory location to copy the bytes to
 * @param size The amount of elements to copy
 * @param dma_options The platform dependent options like:
 *                    - Element size (32-, 16- or 8-bit)
 *                    - Interrupts when transfer is complete
 *                    - Other options that may apply to this transaction See -> dma_platform_specific.h
 * @return UHAL_STATUS_OK when no errors have occurred
 */
uhal_status_t
dma_set_transfer_peripheral_to_mem(const dma_peripheral_t dma_peripheral, const dma_channel_t dma_channel,
                                   const dma_peripheral_location_t src, void *dst, const size_t size,
                                   const dma_opt_t dma_options);

/**
 * @brief Function to initialize a DMA transaction/channel to transfer data from a memory location to a peripheral.
 * @param dma_peripheral The DMA peripheral instance to use (some devices have multiple DMA peripherals)
 * @param dma_channel The DMA peripheral channel to use
 * @param src The memory location to copy from
 * @param dst The external hardware peripheral to copy to
 * @param size The amount of elements to copy
 * @param dma_options  The platform dependent options like:
 *                    - Element size (32-, 16- or 8-bit)
 *                    - Interrupts when transfer is complete
 *                    - Other options that may apply to this transaction See -> dma_platform_specific.h
 * @return UHAL_STATUS_OK when no errors have occurred
 */
uhal_status_t
dma_set_transfer_mem_to_peripheral(const dma_peripheral_t dma_peripheral, const dma_channel_t dma_channel,
                                   const void *src, const dma_peripheral_location_t dst,
                                   const size_t size, const dma_opt_t dma_options);

/**
 * @brief Function to set an extra DMA trigger on given channel
 * @param dma_peripheral The DMA peripheral instance to use (some devices have multiple DMA peripherals)
 * @param dma_channel The DMA peripheral channel to use
 * @param trigger The trigger to react on
 * @return UHAL_STATUS_OK when no errors have occurred
 */
uhal_status_t
dma_set_trigger(const dma_peripheral_t dma_peripheral, const dma_channel_t dma_channel, const dma_trigger_t trigger);


/**
 * @brief Function to remove an existing DMA trigger from given channel
 * @param dma_peripheral The DMA peripheral instance to use (some devices have multiple DMA peripherals)
 * @param dma_channel The DMA peripheral channel to use
 * @param trigger The trigger to remove
 * @return UHAL_STATUS_OK when no errors have occurred
 */
uhal_status_t
dma_reset_trigger(const dma_peripheral_t dma_peripheral, const dma_channel_t dma_channel, const dma_trigger_t trigger);


/**
 * @brief Function to de-initialize a given DMA peripheral
 * @param dma_peripheral The DMA peripheral to de-initialize
 * @return UHAL_STATUS_OK when no errors have occured
 */
uhal_status_t dma_deinit(const dma_peripheral_t dma_peripheral);


/**
 * @brief The default ISR handler for the DMA peripheral
 * @param hw Pointer to the settings of the DMA peripheral on which the transaction occurred
 */
void dma_irq_handler(const void *const hw) __attribute__((weak));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //ATMELSAMD21_HAL_DMA_H
