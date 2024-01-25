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

4. **`dma_set_transfer_peripheral_to_peripheral`**: Sets up a DMA transfer from peripheral to peripheral.

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
    This is the list of options you can set on this DMA:
    
    ```c
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
    ```
    - Option BEAT_SIZE_8_BITS will set the beat size to 8-bits (This is the default value)
    
    - Option ENABLE_CRC_x will enable crc module and calculate the CRCx value of the transferred beats
    
    - Option IRQ_SUSPEND will enable the SUSPEND interrupt for this channel, this ISR gets run when the transaction is finished and DMA has no work left to do.
    
    - Option IRQ_TRANSFER_COMPLETE will enable the TRANSFER_COMPLETE interrupt for this channel, this ISR gets run everytime a transaction finishes.
    
    - Option IRQ_TRANSFER_ERROR will enable the TRANSFER_ERROR interrupt for this channel, this ISR gets run everytime a transaction error occurs (hopefully never :) )
    
    - Option STEP_SIZE_x will set the address increment step of the DMA. This allows you to step with 2-128 steps at a time through the source address.
    	
    	Sidenote: This might be useful when you map your structs very carefully, See [type punning](https://en.wikipedia.org/wiki/Type_punning).
    
    - Option DISABLE_SRC_INCREMENT will disable the increments on the Source address, effectively it will send out the same beat everytime for **size** times
    
    - Option DISABLE_DST_INCREMENT will disable the increments on the Destination address, effectively it will fill one place in memory **size** times
    
    **Very important:** It is not possible to set both DISABLE_SRC_INCREMENT and DISABLE_DST_INCREMENT as first the hardware wouldn't allow it, and second it would not be logical, as you would send the same beat to the same address, size times! :)
    
    - Option BLOCKACT_INT will disable the channel and fire an interrupt after block transfer finished and no transfers left.
    
    - Option BLOCKACT_SUSPEND will disable the channel after block transfer finished and no transfers left.
    
    - Option BLOCKACT_BOTH will fire an interrupt and suspend the channel after block transfer finished and no transfers left.
    
    
??? info "do_software_trigger"
    It is possible to manually trigger the DMA. Setting this option will do just that.
    
    It will set the DMA straight to work after filling in the transaction details :)


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
    The SAMD DMA supports transactions straight to peripherals..
    
    This field sets the specific peripheral to copy from.
    
    The enum typedef for the SAMD21:
    ```c
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
    ```
    
    This enum still needs to be expanded see [issue #14](https://github.com/Hoog-V/Universal_hal/issues/14).
     
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
??? info "dma_options"
     This is the list of options you can set on this DMA:
    
    ```c
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
    ```
    - Option BEAT_SIZE_8_BITS will set the beat size to 8-bits (This is the default value)
    
    - Option ENABLE_CRC_x will enable crc module and calculate the CRCx value of the transferred beats
    
    - Option IRQ_SUSPEND will enable the SUSPEND interrupt for this channel, this ISR gets run when the transaction is finished and DMA has no work left to do.
    
    - Option IRQ_TRANSFER_COMPLETE will enable the TRANSFER_COMPLETE interrupt for this channel, this ISR gets run everytime a transaction finishes.
    
    - Option IRQ_TRANSFER_ERROR will enable the TRANSFER_ERROR interrupt for this channel, this ISR gets run everytime a transaction error occurs (hopefully never :) )
    
    - Option STEP_SIZE_x will set the address increment step of the DMA. This allows you to step with 2-128 steps at a time through the source address.
    	
    	Sidenote: This might be useful when you map your structs very carefully, See [type punning](https://en.wikipedia.org/wiki/Type_punning).
    
    - Option DISABLE_SRC_INCREMENT will disable the increments on the Source address, effectively it will send out the same beat everytime for **size** times
    
    - Option DISABLE_DST_INCREMENT will disable the increments on the Destination address, effectively it will fill one place in memory **size** times
    
    **Very important:** It is not possible to set both DISABLE_SRC_INCREMENT and DISABLE_DST_INCREMENT as first the hardware wouldn't allow it, and second it would not be logical, as you would send the same beat to the same address, size times! :)
    
    - Option BLOCKACT_INT will disable the channel and fire an interrupt after block transfer finished and no transfers left.
    
    - Option BLOCKACT_SUSPEND will disable the channel after block transfer finished and no transfers left.
    
    - Option BLOCKACT_BOTH will fire an interrupt and suspend the channel after block transfer finished and no transfers left.
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
    The SAMD DMA supports transactions straight to peripherals..
    
    This field sets the specific peripheral to copy to.
    
    The enum typedef for the SAMD21:
    ```c
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
    ```
    
    This enum still needs to be expanded see [issue #14](https://github.com/Hoog-V/Universal_hal/issues/14).
    
??? info "size"
    The number of beats to copy. 
    
    Most of the times this correlates to the amount of elements in your array.
    
??? info "dma_options"
    This is the list of options you can set on this DMA:
    
    ```c
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
    ```
    - Option BEAT_SIZE_8_BITS will set the beat size to 8-bits (This is the default value)
    
    - Option ENABLE_CRC_x will enable crc module and calculate the CRCx value of the transferred beats
    
    - Option IRQ_SUSPEND will enable the SUSPEND interrupt for this channel, this ISR gets run when the transaction is finished and DMA has no work left to do.
    
    - Option IRQ_TRANSFER_COMPLETE will enable the TRANSFER_COMPLETE interrupt for this channel, this ISR gets run everytime a transaction finishes.
    
    - Option IRQ_TRANSFER_ERROR will enable the TRANSFER_ERROR interrupt for this channel, this ISR gets run everytime a transaction error occurs (hopefully never :) )
    
    - Option STEP_SIZE_x will set the address increment step of the DMA. This allows you to step with 2-128 steps at a time through the source address.
    	
    	Sidenote: This might be useful when you map your structs very carefully, See [type punning](https://en.wikipedia.org/wiki/Type_punning).
    
    - Option DISABLE_SRC_INCREMENT will disable the increments on the Source address, effectively it will send out the same beat everytime for **size** times
    
    - Option DISABLE_DST_INCREMENT will disable the increments on the Destination address, effectively it will fill one place in memory **size** times
    
    **Very important:** It is not possible to set both DISABLE_SRC_INCREMENT and DISABLE_DST_INCREMENT as first the hardware wouldn't allow it, and second it would not be logical, as you would send the same beat to the same address, size times! :)
    
    - Option BLOCKACT_INT will disable the channel and fire an interrupt after block transfer finished and no transfers left.
    
    - Option BLOCKACT_SUSPEND will disable the channel after block transfer finished and no transfers left.
    
    - Option BLOCKACT_BOTH will fire an interrupt and suspend the channel after block transfer finished and no transfers left.

#### dma_set_transfer_peripheral_to_peripheral function
Still needs to be implemented, see [issue #14](https://github.com/Hoog-V/Universal_hal/issues/14).


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
    Besides triggering the DMA manually, it is possible to trigger the DMA automatically when a peripheral for example gets new data or runs out of data.
    
    These are the possible triggers, which can be set:
    ```c
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
    ```
    
    - Trigger SERCOMx_RX is the trigger for incoming messages on any communication bus (SPI, UART, I2C)
    
    - Trigger SERCOMx_TX is the trigger for transmit complete of any message on communication bus (SPI, UART, I2C)
    
    - Trigger TCCx_OVF is the trigger for timer overflow/overrun
    
    - Trigger TCCx_MCx is the trigger for timer compare x
    
    - Trigger ADC_RESRDY is the trigger for ADC sample ready
    
    - Trigger DAC_EMPTY is the trigger for DAC sample queue empty
    
    - Trigger I2S_RX_x is the trigger for incomming messages on I2S bus x
    
    - Trigger I2S_TX_x is the trigger for transmit complete of message on I2S bus x
    



#### dma_reset_trigger function
```c
uhal_status_t dma_reset_trigger(const dma_peripheral_t dma_peripheral, 
                                const dma_channel_t dma_channel, 
                                const dma_trigger_t trigger);
```

**This function will remove an existing trigger on given channel.**

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
    Besides triggering the DMA manually, it is possible to trigger the DMA automatically when a peripheral for example gets new data or runs out of data.
    
    These are the possible triggers, which can be set:
    ```c
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
    ```
    
    - Trigger SERCOMx_RX is the trigger for incoming messages on any communication bus (SPI, UART, I2C)
    
    - Trigger SERCOMx_TX is the trigger for transmit complete of any message on communication bus (SPI, UART, I2C)
    
    - Trigger TCCx_OVF is the trigger for timer overflow/overrun
    
    - Trigger TCCx_MCx is the trigger for timer compare x
    
    - Trigger ADC_RESRDY is the trigger for ADC sample ready
    
    - Trigger DAC_EMPTY is the trigger for DAC sample queue empty
    
    - Trigger I2S_RX_x is the trigger for incomming messages on I2S bus x
    
    - Trigger I2S_TX_x is the trigger for transmit complete of message on I2S bus x
    
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
