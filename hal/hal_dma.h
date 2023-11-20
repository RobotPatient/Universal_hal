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


uhal_status_t dma_init(dma_peripheral_t dma_peripheral, dma_init_opt_t dma_init_options);

uhal_status_t dma_set_transfer_mem(const dma_peripheral_t peripheral,
                                   const dma_channel_t dma_channel,
                                   const void *src,
                                   void *dst,
                                   const size_t size,
                                   const dma_opt_t dma_options,
                                   const uint8_t do_software_trigger);

uhal_status_t
dma_set_transfer_peripheral_to_mem(const dma_peripheral_t dma_peripheral, const dma_channel_t dma_channel,
                                   const dma_peripheral_location_t src, void *dst, const size_t size,
                                   const dma_opt_t dma_options);

uhal_status_t
dma_set_transfer_mem_to_peripheral(const dma_peripheral_t dma_peripheral, const dma_channel_t dma_channel,
                                   const void *src, const dma_peripheral_location_t dst, const size_t size,
                                   const dma_trigger_t dma_trigger, const dma_opt_t dma_options);

uhal_status_t
set_dma_trigger(const dma_peripheral_t dma_peripheral, const dma_channel_t dma_channel, const dma_trigger_t trigger);


uhal_status_t
reset_dma_trigger(const dma_peripheral_t dma_peripheral, const dma_channel_t dma_channel, const dma_trigger_t trigger);


uhal_status_t dma_deinit(const dma_peripheral_t dma_peripheral);



void dma_irq_handler(const void *const hw) __attribute__((weak));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //ATMELSAMD21_HAL_DMA_H
