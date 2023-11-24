# Atmel SAMD21/51 DMA API Usage

The Direct Memory Access (DMA) module on Atmel SAMD21/51 microcontrollers allows for efficient data transfers without CPU intervention. This module is particularly useful for high-speed, automated data transfer tasks. Here's a guide to using the DMA API on these platforms.

## Platform Specific Settings

Before using the DMA module, certain preparatory steps are necessary:

- [ ] **Initialize Clocks**: Ensure the system clocks are configured, as DMA operations depend on these for timing.
- [ ] **Source and Destination in RAM**: Confirm that both data source and destination are in RAM. The DMA module cannot access data stored in flash memory.

## Function Prototypes

### Initialization
```c
uhal_status_t dma_init(dma_peripheral_t dma_peripheral, 
		               dma_init_opt_t dma_init_options);
```
Initializes the DMA peripheral with specified options.

Within this function the following parameters can be set:
??? info  "dma_peripheral"
    This parameter tells the function which peripheral to use. 
    
    **AFAIK most of the SAMD series have one peripheral: `DMA_PERIPHERAL_0`.**
    
    The enum typedef for the SAMD21 series:
    ```c
    typedef enum {
    DMA_PERIPHERAL_0,
    } dma_peripheral_t;
    ```
    If for instance the microcontroller would have more DMA peripherals than 1 then it would map like this:
    
    DMAC0 -> DMA_PERIPHERAL_0
    
    DMAC1 -> DMA_PERIPHERAL_1

??? info "dma_init_options"
    This parameter gives extra intialization options.
    
    The SAMD21 supports the following extra configuration options:
    
    ```c
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
    ```
    
    - DMA_INIT_OPT_HANDLE_PRIORITY_LEVEL_1 ... _16 allows you to enable certain priority levels. If disabled and a transaction comes in with set priority level then it will be ignored by DMA. 
    
    	The default option when not defining a certain priority level is: **DMA_INIT_OPT_HANDLE_PRIORITY_LEVEL_16**.
    
    - DMA_INIT_OPT_USE_IRQ_PRIORITY_0 ... _3 allows you to set the NVIC IRQ priority to interrupts performed for the DMA.
    
    	The default option when not defining a certain priority level is: **DMA_INIT_OPT_USE_IRQ_PRIORITY_2**

### Data Transfer Setup
There are a few functions which will set this up:

1. **`dma_set_transfer_mem`**: Configures a DMA transfer from one memory location to another.

2. **`dma_set_transfer_peripheral_to_mem`**: Sets up a DMA transfer from a peripheral to a memory location.

3. **`dma_set_transfer_mem_to_peripheral`**: Establishes a DMA transfer from memory to a peripheral.

#### dma_set_transfer_mem function
The **`glorified`** hardware memcpy as I would call it :) 

This function allows you to copy data from one place in ram to another using the SAMD's DMAC peripheral.
```c
uhal_status_t dma_set_transfer_mem(const dma_peripheral_t peripheral,
                                   const dma_channel_t dma_channel, 
                                   const void* src,
                                   void* dst, 
                                   const size_t size,
                                   const dma_opt_t dma_options, 
                                   const uint8_t do_software_trigger);
```

Within this function the following parameters can be set:

??? info "dma_peripheral"
    This parameter tells the function which peripheral to use. 
    
    **AFAIK most of the SAMD series have one peripheral: `DMA_PERIPHERAL_0`.**
    
    The enum typedef for the SAMD21 series:
    ```c
    typedef enum {
    DMA_PERIPHERAL_0,
    } dma_peripheral_t;
    ```
    If for instance the microcontroller would have more DMA peripherals than 1 then it would map like this:
    
    DMAC0 -> DMA_PERIPHERAL_0
    
    DMAC1 -> DMA_PERIPHERAL_1
    
??? info "dma_channel"
    This parameter tells the function which DMA channel to use.
    
    Most SAMx21 based controllers have up to 12 DMA channels, while most SAMx51..54 series have 32 channels.
        
    The enum typedef for the SAMD21:
    ```c
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
    ```
??? info "src"
    The RAM location to copy from.
    
    The DMAC peripheral in the SAMx series supports 8, 16 and 32-bit beat sizes. 
    
    That means that any base type pointer to a variable (or arrays) of 8, 16 or 32-bits can be put in to this parameter.
    
    Some of the types it will surely support:
    
    - byte, uint8_t*, int8_t*, char*
    
    - short, uint16_t*, int16_t*
    
    - int*, unsigned int*, uint32_t*, int32_t*
    
    **This is the memory location to copy from! Make sure that this memory location is in RAM, as the DMA can't access flash memory**
    
??? info "dst"
    The RAM location to copy to.
    
    The DMAC peripheral in the SAMx series supports 8, 16 and 32-bit beat sizes. 
    
    That means that any base type pointer to a variable (or arrays) of 8, 16 or 32-bits can be put in to this parameter.
    
    Some of the types it will surely support:
    
    - byte, uint8_t*, int8_t*, char*
    
    - short, uint16_t*, int16_t*
    
    - int*, unsigned int*, uint32_t*, int32_t*
    
    **This is the memory location to copy to! Make sure that this memory location is in RAM, as the DMA can't access flash memory**
    
??? info "size"
    The number of beats to copy. 
    
    Most of the times this correlates to the amount of elements in your array.
    
    **Another thing to note!** 
     Make sure that the size of your source array matches the size of your destination array. As it would otherwise write out-of-bounds!
     
??? info "dma_options"
    

??? info "do_software_trigger"
    do_software_trigger explenation
#### dma_set_transfer_peripheral_to_mem function
```c
uhal_status_t dma_set_transfer_peripheral_to_mem(const dma_peripheral_t dma_peripheral, 
                                                 const dma_channel_t dma_channel,
                                                 const dma_peripheral_location_t src, 
                                                 void* dst, 
                                                 const size_t size, 
                                                 const dma_opt_t dma_options);
```
Sets up a DMA transfer from a peripheral to a memory location.

Within this function the following parameters can be set:

??? info "dma_peripheral"
    This parameter tells the function which peripheral to use. 
    
    **AFAIK most of the SAMD series have one peripheral: `DMA_PERIPHERAL_0`.**
    
    The enum typedef for the SAMD21 series:
    ```c
    typedef enum {
    DMA_PERIPHERAL_0,
    } dma_peripheral_t;
    ```
    If for instance the microcontroller would have more DMA peripherals than 1 then it would map like this:
    
    DMAC0 -> DMA_PERIPHERAL_0
    
    DMAC1 -> DMA_PERIPHERAL_1

??? info "dma_channel"
    This parameter tells the function which DMA channel to use.
    
    Most SAMx21 based controllers have up to 12 DMA channels, while most SAMx51..54 series have 32 channels.
        
    The enum typedef for the SAMD21:
    ```c
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
    ```
??? info "src"
    The number of beats to copy. 
    
    Most of the times this correlates to the amount of elements in your array.
    
    **Another thing to note!** 
     Make sure that the size of your destination location is bigger or equal to amount of elements you want to copy. As it would otherwise write out-of-bounds!
     
??? info "dst"
    The RAM location to copy to.
    
    The DMAC peripheral in the SAMx series supports 8, 16 and 32-bit beat sizes. 
    
    That means that any base type pointer to a variable (or arrays) of 8, 16 or 32-bits can be put in to this parameter.
    
    Some of the types it will surely support:
    
    - byte, uint8_t*, int8_t*, char*
    
    - short, uint16_t*, int16_t*
    
    - int*, unsigned int*, uint32_t*, int32_t*
    
    **This is the memory location to copy to! Make sure that this memory location is in RAM, as the DMA can't access flash memory**
??? info "size"

??? info "dma_options"
 
#### dma_set_transfer_mem_to_peripheral function
```c
uhal_status_t dma_set_transfer_mem_to_peripheral(const dma_peripheral_t dma_peripheral,
                                                 const dma_channel_t dma_channel, 
                                                 const void* src,
                                                 const dma_peripheral_location_t dst, 
                                                 const size_t size, 
                                                 const dma_opt_t dma_options);
``` 
Establishes a DMA transfer from memory to a peripheral.

Within this function the following parameters can be set:

??? info "dma_peripheral"
    This parameter tells the function which peripheral to use. 
    
    **AFAIK most of the SAMD series have one peripheral: `DMA_PERIPHERAL_0`.**
    
    The enum typedef for the SAMD21 series:
    ```c
    typedef enum {
    DMA_PERIPHERAL_0,
    } dma_peripheral_t;
    ```
    If for instance the microcontroller would have more DMA peripherals than 1 then it would map like this:
    
    DMAC0 -> DMA_PERIPHERAL_0
    
    DMAC1 -> DMA_PERIPHERAL_1

??? info "dma_channel"
    This parameter tells the function which DMA channel to use.
    
    Most SAMx21 based controllers have up to 12 DMA channels, while most SAMx51..54 series have 32 channels.
        
    The enum typedef for the SAMD21:
    ```c
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
    ```
??? info "src"
    The RAM location to copy from.
    
    The DMAC peripheral in the SAMx series supports 8, 16 and 32-bit beat sizes. 
    
    That means that any base type pointer to a variable (or arrays) of 8, 16 or 32-bits can be put in to this parameter.
    
    Some of the types it will surely support:
    
    - byte, uint8_t*, int8_t*, char*
    
    - short, uint16_t*, int16_t*
    
    - int*, unsigned int*, uint32_t*, int32_t*
    
    **This is the memory location to copy from! Make sure that this memory location is in RAM, as the DMA can't access flash memory**
??? info "dst"

??? info "size"
    The number of beats to copy. 
    
    Most of the times this correlates to the amount of elements in your array.
    
??? info "dma_options"

### Trigger Configuration
#### dma_set_trigger function
```c
uhal_status_t dma_set_trigger(const dma_peripheral_t dma_peripheral, 
                              const dma_channel_t dma_channel, 
                              const dma_trigger_t trigger);
```

??? info "dma_peripheral"
    This parameter tells the function which peripheral to use. 
    
    **AFAIK most of the SAMD series have one peripheral: `DMA_PERIPHERAL_0`.**
    
    The enum typedef for the SAMD21 series:
    ```c
    typedef enum {
    DMA_PERIPHERAL_0,
    } dma_peripheral_t;
    ```
    If for instance the microcontroller would have more DMA peripherals than 1 then it would map like this:
    
    DMAC0 -> DMA_PERIPHERAL_0
    
    DMAC1 -> DMA_PERIPHERAL_1

??? info "dma_channel"
    This parameter tells the function which DMA channel to use.
    
    Most SAMx21 based controllers have up to 12 DMA channels, while most SAMx51..54 series have 32 channels.
        
    The enum typedef for the SAMD21:
    ```c
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
    ```
??? info "dma_trigger"

#### dma_reset_trigger function
```c
uhal_status_t dma_reset_trigger(const dma_peripheral_t dma_peripheral, 
                                const dma_channel_t dma_channel, 
                                const dma_trigger_t trigger);
```

??? info "dma_peripheral"
    This parameter tells the function which peripheral to use. 
    
    **AFAIK most of the SAMD series have one peripheral: `DMA_PERIPHERAL_0`.**
    
    The enum typedef for the SAMD21 series:
    ```c
    typedef enum {
    DMA_PERIPHERAL_0,
    } dma_peripheral_t;
    ```
    If for instance the microcontroller would have more DMA peripherals than 1 then it would map like this:
    
    DMAC0 -> DMA_PERIPHERAL_0
    
    DMAC1 -> DMA_PERIPHERAL_1

??? info "dma_channel"
    This parameter tells the function which DMA channel to use.
    
    Most SAMx21 based controllers have up to 12 DMA channels, while most SAMx51..54 series have 32 channels.
        
    The enum typedef for the SAMD21:
    ```c
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
    ```
??? info "dma_trigger"

## Example Configuration

Here's an example of configuring a DMA transfer on the Atmel SAMD21 platform:

```c
#include "hal_dma.h"

// Define source and destination addresses in RAM
uint8_t src_data[] = "Hello world!";
uint8_t dst_data[sizeof(src_data)] = {0};

void setup() {
    // Initialize DMA peripheral with default options
    dma_init(DMA_PERIPHERAL_0, DMA_INIT_OPT_USE_DEFAULT);

    // Set up DMA transfer from src_data to dst_data
    dma_set_transfer_mem(DMA_PERIPHERAL_0, DMA_CHANNEL_0, src_data, dst_data, sizeof(src_data),
                         DMA_OPT_USE_DEFAULT, 1); // 1 for immediate software trigger

    // Additional configurations like setting triggers can be done here
}

void loop() {
    // Your main code here
}
```

In this example, a DMA channel is initialized and configured to transfer data from `src_data` to `dst_data`. Note that this is a basic setup and may vary depending on specific project requirements and hardware configurations. 

Remember, when working with DMA, it's crucial to ensure the data source and destination are correctly set up and the necessary clock systems are initialized.
