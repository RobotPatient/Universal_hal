/**
* \file            dma.c
* \brief           DMA module source file
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
#include <string.h>
#include "hal_dma.h"
#include "bit_manipulation.h"

struct dmacdescriptor {
    uint16_t btctrl;
    uint16_t btcnt;
    uint32_t srcaddr;
    uint32_t dstaddr;
    uint32_t descaddr;
};

// 12 channels
volatile struct dmacdescriptor wrb[12] __attribute__ ((aligned (16)));
struct dmacdescriptor descriptor_section[12] __attribute__ ((aligned (16)));
uint8_t dma_done[12];

static inline uint8_t get_step_size(dma_opt_t dma_options) {
    uint16_t step_size = (BITMASK_COMPARE(dma_options, DMA_OPT_STEP_SIZE_128)) >> 6;
    return step_size;
}

static inline uint32_t calculate_dstaddr(uint32_t dst_addr, size_t size, dma_opt_t dma_options) {
    uint32_t dstaddr;
    if(BITMASK_COMPARE(dma_options, DMA_OPT_APPLY_STEP_SIZE_TO_DST)) {
        const uint8_t step_size = get_step_size(dma_options);
        dstaddr = (uint32_t)dst_addr + (size*(3)) * (1<<step_size);
    } else {
        dstaddr = (uint32_t)dst_addr + (size*(3));
    }
    return dstaddr;
}

uhal_status_t dma_init(dma_init_opt_t dma_init_options) {
    PM->AHBMASK.reg |= PM_AHBMASK_DMAC;
    PM->APBBMASK.reg |= PM_APBBMASK_DMAC;
    while (GCLK->STATUS.bit.SYNCBUSY);

    DMAC->BASEADDR.reg = (uint32_t)descriptor_section;
    DMAC->WRBADDR.reg = (uint32_t)wrb;
    const uint8_t lvlen = (dma_init_options != DMA_INIT_OPT_USE_DEFAULT) ? dma_init_options : 0xf;
    DMAC->CTRL.reg = DMAC_CTRL_DMAENABLE | DMAC_CTRL_LVLEN(lvlen);

    NVIC_ClearPendingIRQ(DMAC_IRQn);
    NVIC_EnableIRQ(DMAC_IRQn);
    NVIC_SetPriority(DMAC_IRQn, 2);
    return UHAL_STATUS_OK;
}

uhal_status_t
dma_set_transfer_mem(dma_channel_t dma_channel, void *src, void *dst, size_t size, dma_trigger_t dma_trigger,
                     dma_opt_t dma_options) {
    struct dmacdescriptor descriptor __attribute__ ((aligned (16)));
    DMAC->CHID.reg = DMAC_CHID_ID(dma_channel);
    DMAC->CHINTENSET.reg = DMAC_CHINTENSET_MASK ; // enable all 3 interrupts
    descriptor.descaddr = 0;
    if(BITMASK_COMPARE(dma_options, DMA_OPT_BEAT_SIZE_16_BITS)) {

        descriptor.srcaddr = (uint32_t)src + size;
    }


    descriptor.btcnt =  n/4;
    descriptor.btctrl =  DMAC_BTCTRL_BEATSIZE_WORD | DMAC_BTCTRL_DSTINC | DMAC_BTCTRL_SRCINC | DMAC_BTCTRL_VALID;
    memcpy(&descriptor_section[dma_channel],&descriptor, sizeof(struct dmacdescriptor));
    DMAC->CHCTRLA.reg |= DMAC_CHCTRLA_ENABLE;
    DMAC->SWTRIGCTRL.reg |= (1 <<dma_channel);  // trigger channel
    while(!dma_done[dma_channel]);  // await DMA done isr
    return UHAL_STATUS_OK;
}

uhal_status_t
dma_set_transfer_peripheral_to_mem(dma_channel_t dma_channel, void *src, void *dst, size_t size,
                                   dma_trigger_t dma_trigger, dma_opt_t dma_options) {
    return UHAL_STATUS_OK;
}

uhal_status_t
dma_set_transfer_mem_to_peripheral(dma_channel_t dma_channel, void *src, void *dst, size_t size,
                                   dma_trigger_t dma_trigger, dma_opt_t dma_options) {
    return UHAL_STATUS_OK;
}

uhal_status_t dma_deinit() {
    return UHAL_STATUS_OK;
}