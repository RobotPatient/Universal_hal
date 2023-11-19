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
#include "irq/irq_bindings.h"

#define SHIFT_BLOCKACT_TO_BTCTRL_POS(dma_options) (BITMASK_COMPARE(dma_options, DMA_OPT_BLOCKACT_BOTH) >> 9)
#define SHIFT_EVENT_OUTPUT_TO_BTCTRL_POS(dma_options) (BITMASK_COMPARE(dma_options, (DMA_OPT_EVENT_OUTPUT_BLOCK | DMA_OPT_EVENT_OUTPUT_BEAT)) >> 13)
#define SHIFT_SUSPEND_IRQ_TO_CHINTENSET_POS(dma_options) (BITMASK_COMPARE(dma_options, DMA_OPT_IRQ_SUSPEND) >> 2)
#define SHIFT_TRANSFER_COMPLETE_IRQ_TO_CHINTENSET_POS(dma_options) (BITMASK_COMPARE(dma_options, DMA_OPT_IRQ_TRANSFER_COMPLETE) >> 4)
#define SHIFT_ERROR_IRQ_TO_CHINTENSET_POS(dma_options) (BITMASK_COMPARE(dma_options, DMA_OPT_IRQ_TRANSFER_ERROR) == DMA_OPT_IRQ_TRANSFER_ERROR)

#define DEFAULT_IRQ_PRIORITY 2

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

static inline uint8_t get_step_size(dma_opt_t dma_options) {
    uint16_t step_size = (BITMASK_COMPARE(dma_options, DMA_OPT_STEP_SIZE_128)) >> 6;
    return step_size;
}

static inline uint32_t calculate_addr(const void* addr, const uint8_t beat_size, const size_t size, const uint8_t step_size) {
    uint32_t res;
    if(step_size) {
        res = (uint32_t)addr + (size*(beat_size)) * (1<<step_size);
    } else {
        res = (uint32_t)addr + (size*(beat_size));
    }
    return res;
}

static inline uint8_t get_irq_priority(dma_init_opt_t dma_init_opt) {
    uint8_t res = BITMASK_COMPARE(dma_init_opt, 0x70) >> 4;
    return res ? res-1 : DEFAULT_IRQ_PRIORITY;
}

uhal_status_t dma_init(dma_init_opt_t dma_init_options) {
    PM->AHBMASK.reg |= PM_AHBMASK_DMAC;
    PM->APBBMASK.reg |= PM_APBBMASK_DMAC;
    while (GCLK->STATUS.bit.SYNCBUSY);

    DMAC->BASEADDR.reg = (uint32_t)descriptor_section;
    DMAC->WRBADDR.reg = (uint32_t)wrb;
    const uint8_t lvlen = (dma_init_options != DMA_INIT_OPT_USE_DEFAULT) ? dma_init_options : 0xf;
    DMAC->CTRL.reg = DMAC_CTRL_DMAENABLE | DMAC_CTRL_LVLEN(lvlen);
    const uint8_t irq_prio = get_irq_priority(dma_init_options);
    enable_irq_handler(DMAC_IRQn, irq_prio);
    return UHAL_STATUS_OK;
}

uhal_status_t dma_set_transfer_mem(const dma_channel_t dma_channel, const void *src, void *dst, const size_t size, const dma_opt_t dma_options,
                                   const uint8_t do_software_trigger) {
    struct dmacdescriptor descriptor __attribute__ ((aligned (16)));
    DMAC->CHID.reg = DMAC_CHID_ID(dma_channel);
    descriptor.descaddr = 0;

    const uint8_t beat_size_opt = BITMASK_COMPARE(dma_options, DMA_OPT_BEAT_SIZE_32_BITS);
    const uint8_t beat_size = beat_size_opt ? beat_size_opt : 1;
    const uint8_t step_size = get_step_size(dma_options);

    descriptor.dstaddr = calculate_addr(dst, beat_size, size, step_size);

    /*
     * Apply the step size to the source address if enabled.
     */
    const uint16_t src_step_size_en = BITMASK_COMPARE(dma_options, DMA_OPT_APPLY_STEP_SIZE_TO_SRC) << 1;
    const uint16_t src_step_size = src_step_size_en ? step_size : 0;
    descriptor.srcaddr = calculate_addr(src, beat_size, size, src_step_size);

    /*
     * Determine bt_cnt based on the calculated beat size
     */
    descriptor.btcnt =  beat_size < DMA_OPT_BEAT_SIZE_32_BITS ? beat_size : 4;  // Every other value than the 32-bits option correspond directly to
                                                                                // the number of bytes.
                                                                                // Alter to the right value if the 32-bits option is selected.
    descriptor.btcnt = size/descriptor.btcnt ;

    const uint16_t src_incr_en = BITMASK_COMPARE(dma_options, DMA_OPT_DISABLE_SRC_INCREMENT) ? 0 : DMAC_BTCTRL_SRCINC;
    const uint16_t dst_incr_en = BITMASK_COMPARE(dma_options, DMA_OPT_DISABLE_DST_INCREMENT) ? 0 : DMAC_BTCTRL_DSTINC;
    const uint16_t block_act = SHIFT_BLOCKACT_TO_BTCTRL_POS(dma_options);
    const uint32_t event_output = SHIFT_EVENT_OUTPUT_TO_BTCTRL_POS(dma_options);
    descriptor.btctrl = DMAC_BTCTRL_BEATSIZE(beat_size-1) |src_incr_en | dst_incr_en | DMAC_BTCTRL_VALID | DMAC_BTCTRL_STEPSIZE(step_size) | src_step_size_en | block_act | event_output;
    memcpy(&descriptor_section[dma_channel],&descriptor, sizeof(struct dmacdescriptor));
    DMAC->CHCTRLA.reg = DMAC_CHCTRLA_ENABLE;

    DMAC->CHINTENSET.reg |= SHIFT_SUSPEND_IRQ_TO_CHINTENSET_POS(dma_options) |
                            SHIFT_TRANSFER_COMPLETE_IRQ_TO_CHINTENSET_POS(dma_options) |
                            SHIFT_ERROR_IRQ_TO_CHINTENSET_POS(dma_options);

    if(do_software_trigger) {
        DMAC->SWTRIGCTRL.reg |= (1 << dma_channel); // trigger channel
    }
    return UHAL_STATUS_OK;
}


uhal_status_t set_dma_trigger(const dma_channel_t dma_channel, const dma_trigger_t trigger) {
    return UHAL_STATUS_OK;
}

uhal_status_t unset_dma_trigger(const dma_channel_t dma_channel, const dma_trigger_t trigger) {
    return UHAL_STATUS_OK;
}

uhal_status_t
dma_set_transfer_peripheral_to_mem(const dma_channel_t dma_channel, const void *src, void *dst, const size_t size,
                                   const dma_trigger_t dma_trigger, const dma_opt_t dma_options) {
    return UHAL_STATUS_OK;
}

uhal_status_t
dma_set_transfer_mem_to_peripheral(const dma_channel_t dma_channel, const void *src, void *dst, const size_t size,
                                   const dma_trigger_t dma_trigger, const dma_opt_t dma_options) {
    return UHAL_STATUS_OK;
}

uhal_status_t dma_deinit() {
    return UHAL_STATUS_OK;
}