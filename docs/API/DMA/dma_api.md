# DMA API

## API Functionality

The API for DMA (Direct Memory Access) functionality includes the following functions:

```c
/* DMA driver initialization function (without compile-time parameter checking) */
uhal_status_t dma_init(dma_peripheral_t dma_peripheral, dma_init_opt_t dma_init_options);

/* DMA driver initialization function (with compile-time parameter checking) */
uhal_status_t DMA_INIT(dma_peripheral_t dma_peripheral, dma_init_opt_t dma_init_options);

/* DMA memory-to-memory transfer setup function (without compile-time parameter checking) */
uhal_status_t dma_set_transfer_mem(dma_peripheral_t peripheral, dma_channel_t dma_channel, const void *src, void *dst, size_t size, dma_opt_t dma_options, uint8_t do_software_trigger);

/* DMA memory-to-memory transfer setup function (with compile-time parameter checking) */
uhal_status_t DMA_SET_TRANSFER_MEM(dma_peripheral_t peripheral, dma_channel_t dma_channel, const void *src, void *dst, size_t size, dma_opt_t dma_options, uint8_t do_software_trigger);

/* DMA peripheral-to-memory transfer setup function (without compile-time parameter checking) */
uhal_status_t dma_set_transfer_peripheral_to_mem(dma_peripheral_t dma_peripheral, dma_channel_t dma_channel, dma_peripheral_location_t src, void *dst, size_t size, dma_opt_t dma_options);

/* DMA peripheral-to-memory transfer setup function (with compile-time parameter checking) */
uhal_status_t DMA_SET_TRANSFER_PERIPHERAL_TO_MEM(dma_peripheral_t dma_peripheral, dma_channel_t dma_channel, dma_peripheral_location_t src, void *dst, size_t size, dma_opt_t dma_options);

/* DMA memory-to-peripheral transfer setup function (without compile-time parameter checking) */
uhal_status_t dma_set_transfer_mem_to_peripheral(dma_peripheral_t dma_peripheral, dma_channel_t dma_channel, const void *src, dma_peripheral_location_t dst, size_t size, dma_opt_t dma_options);

/* DMA memory-to-peripheral transfer setup function (with compile-time parameter checking) */
uhal_status_t DMA_SET_TRANSFER_MEM_TO_PERIPHERAL(dma_peripheral_t dma_peripheral, dma_channel_t dma_channel, const void *src, dma_peripheral_location_t dst, size_t size, dma_opt_t dma_options);

/* Function to set extra DMA trigger on a channel (without compile-time parameter checking) */
uhal_status_t set_dma_trigger(dma_peripheral_t dma_peripheral, dma_channel_t dma_channel, dma_trigger_t trigger);

/* Function to set extra DMA trigger on a channel (with compile-time parameter checking) */
uhal_status_t SET_DMA_TRIGGER(dma_peripheral_t dma_peripheral, dma_channel_t dma_channel, dma_trigger_t trigger);

/* Function to reset DMA trigger from a channel (without compile-time parameter checking) */
uhal_status_t reset_dma_trigger(dma_peripheral_t dma_peripheral, dma_channel_t dma_channel, dma_trigger_t trigger);

/* Function to reset DMA trigger from a channel (with compile-time parameter checking) */
uhal_status_t RESET_DMA_TRIGGER(dma_peripheral_t dma_peripheral, dma_channel_t dma_channel, dma_trigger_t trigger);

/* DMA driver deinitialization function (without compile-time parameter checking) */
uhal_status_t dma_deinit(dma_peripheral_t dma_peripheral);

/* DMA driver deinitialization function (with compile-time parameter checking) */
uhal_status_t DMA_DEINIT(dma_peripheral_t dma_peripheral);
```

## dma_set_transfer_mem function

```c
/* DMA memory-to-memory transfer setup function (without compile-time parameter checking) */
uhal_status_t dma_set_transfer_mem(const dma_peripheral_t peripheral,
                                   const dma_channel_t dma_channel,
                                   const void *src,
                                   void *dst,
                                   const size_t size,
                                   const dma_opt_t dma_options,
                                   const uint8_t do_software_trigger);

/* DMA memory-to-memory transfer setup function (with compile-time parameter checking) */
uhal_status_t DMA_SET_TRANSFER_MEM(const dma_peripheral_t peripheral,
                                   const dma_channel_t dma_channel,
                                   const void *src,
                                   void *dst,
                                   const size_t size,
                                   const dma_opt_t dma_options,
                                   const uint8_t do_software_trigger);
```

### Description:

The `dma_set_transfer_mem` function configures a DMA channel for memory-to-memory data transfer. This function is crucial for efficiently moving data within memory without CPU intervention.

### Error Checking:

- **DMA_SET_TRANSFER_MEM**: Includes compile-time checks to ensure that the parameters such as `peripheral`, `dma_channel`, and `dma_options` are correctly defined. This check aids in preventing configuration errors.
  
	- **Usage Note**: Use this function when parameters are static and known at compile time.

- **dma_set_transfer_mem**: Performs the same functionality as the uppercase variant but without compile-time parameter checking.
  
	- **Usage Note**: Suitable for dynamic scenarios where parameters may change at runtime.

### Parameters:

1. **peripheral (dma_peripheral_t)**: The DMA peripheral instance to use.
2. **dma_channel (dma_channel_t)**: The specific DMA channel to configure for the transfer.
3. **src (const void\*)**: Source memory address for the data transfer.
4. **dst (void\*)**: Destination memory address for the data transfer.
5. **size (const size_t)**: The number of elements to transfer.
6. **dma_options (dma_opt_t)**: Platform-specific options, like element size, interrupt triggers, etc.
7. **do_software_trigger (const uint8_t)**: Specifies whether to immediately trigger the transfer via software.

### Return:

- **uhal_status_t**: Indicates the status of the setup process, with success or failure of the configuration.

### Working:

1. Validates the `peripheral`, `dma_channel`, and other parameters.
2. Configures the DMA channel for a memory-to-memory transfer based on the provided `src`, `dst`, `size`, and `dma_options`.
3. If `do_software_trigger` is set, triggers the DMA transfer immediately, otherwise, the transfer awaits an external trigger.

## dma_set_transfer_peripheral_to_mem function

```c
/* DMA peripheral-to-memory transfer setup function (without compile-time parameter checking) */
uhal_status_t dma_set_transfer_peripheral_to_mem(const dma_peripheral_t dma_peripheral,
                                                 const dma_channel_t dma_channel,
                                                 const dma_peripheral_location_t src,
                                                 void *dst,
                                                 const size_t size,
                                                 const dma_opt_t dma_options);

/* DMA peripheral-to-memory transfer setup function (with compile-time parameter checking) */
uhal_status_t DMA_SET_TRANSFER_PERIPHERAL_TO_MEM(const dma_peripheral_t dma_peripheral,
                                                 const dma_channel_t dma_channel,
                                                 const dma_peripheral_location_t src,
                                                 void *dst,
                                                 const size_t size,
                                                 const dma_opt_t dma_options);
```

### Description:

The `dma_set_transfer_peripheral_to_mem` function configures a DMA channel to transfer data from a hardware peripheral to a memory location. This setup is commonly used to receive data from peripherals like ADCs or UARTs efficiently, minimizing CPU load.

### Error Checking:

- **DMA_SET_TRANSFER_PERIPHERAL_TO_MEM**: This uppercase variant includes compile-time parameter checking to validate the DMA peripheral and channel, as well as the source and destination addresses.
  
	- **Usage Note**: Ideal for use cases where DMA channel and peripheral configurations are known at compile time.

- **dma_set_transfer_peripheral_to_mem**: Performs the same function without compile-time checks.
  
	- **Usage Note**: Suitable for dynamic scenarios where DMA parameters are determined at runtime or need flexibility.

### Parameters:

1. **dma_peripheral (dma_peripheral_t)**: The DMA peripheral instance to configure.
2. **dma_channel (dma_channel_t)**: The designated DMA channel for the operation.
3. **src (dma_peripheral_location_t)**: The hardware peripheral address serving as the data source.
4. **dst (void\*)**: The memory location where the data will be stored.
5. **size (const size_t)**: Number of elements to be transferred.
6. **dma_options (dma_opt_t)**: Options for transfer configuration, such as element size and interrupt settings.

### Return:

- **uhal_status_t**: Indicates whether the DMA channel configuration was successful or encountered errors.

### Working:

1. Checks the compatibility and correctness of the given parameters.
2. Sets up the DMA channel to link the specified source peripheral to the destination memory address.
3. Implements the provided `dma_options` for the transfer, adjusting settings like element size and interrupt behavior.
4. Prepares the DMA channel for operation, ready to move data upon the specified trigger or software initiation.


## dma_set_transfer_mem_to_peripheral function

```c
/* DMA memory-to-peripheral transfer setup function (without compile-time parameter checking) */
uhal_status_t dma_set_transfer_mem_to_peripheral(const dma_peripheral_t dma_peripheral,
                                                 const dma_channel_t dma_channel,
                                                 const void *src,
                                                 const dma_peripheral_location_t dst,
                                                 const size_t size,
                                                 const dma_opt_t dma_options);

/* DMA memory-to-peripheral transfer setup function (with compile-time parameter checking) */
uhal_status_t DMA_SET_TRANSFER_MEM_TO_PERIPHERAL(const dma_peripheral_t dma_peripheral,
                                                 const dma_channel_t dma_channel,
                                                 const void *src,
                                                 const dma_peripheral_location_t dst,
                                                 const size_t size,
                                                 const dma_opt_t dma_options);
```

### Description:

The `dma_set_transfer_mem_to_peripheral` function configures a DMA channel for transferring data from a memory location to a hardware peripheral. This setup is commonly used for sending data to peripherals like SPI or UART without CPU intervention.

### Error Checking:

- **DMA_SET_TRANSFER_MEM_TO_PERIPHERAL**: Implements compile-time checks for parameters such as `dma_peripheral` and `dma_channel`, ensuring that the configuration is valid and safe.
  
	- **Usage Note**: Recommended when the DMA setup parameters are known and constant at compile time.

- **dma_set_transfer_mem_to_peripheral**: Provides identical functionality without compile-time parameter verification.
  
	- **Usage Note**: Suitable for scenarios where DMA parameters may be determined or modified at runtime.

### Parameters:

1. **dma_peripheral (dma_peripheral_t)**: The specific DMA peripheral instance to use.
2. **dma_channel (dma_channel_t)**: The designated DMA channel for the operation.
3. **src (const void\*)**: Source memory address for the data.
4. **dst (dma_peripheral_location_t)**: Destination peripheral to receive the data.
5. **size (const size_t)**: Number of elements to transfer.
6. **dma_options (dma_opt_t)**: Configuration options like element size, interrupt behavior, etc.

### Return:

- **uhal_status_t**: Indicates the success or failure of the DMA channel configuration.

### Working:

1. Validates provided parameters for compatibility and correctness.
2. Configures the DMA channel, specifying the source memory address and destination peripheral.
3. Applies specified `dma_options` for the transfer, such as element size and interrupt generation.
4. Enables the DMA channel for the transfer, readying it to move data upon trigger or software initiation.

## dma_set_trigger function

```c
/* DMA trigger setting function (without compile-time parameter checking) */
uhal_status_t dma_set_trigger(const dma_peripheral_t dma_peripheral, 
                              const dma_channel_t dma_channel, 
                              const dma_trigger_t trigger);

/* DMA trigger setting function (with compile-time parameter checking) */
uhal_status_t DMA_SET_TRIGGER(const dma_peripheral_t dma_peripheral, 
                              const dma_channel_t dma_channel, 
                              const dma_trigger_t trigger);
```

### Description:

The `dma_set_trigger` function configures a trigger for a specific DMA channel. This trigger, typically an event or a peripheral action, initiates the data transfer process managed by the DMA controller.

### Error Checking:

- **DMA_SET_TRIGGER**: The uppercase version of the function checks the validity of the DMA peripheral, channel, and trigger at compile time. This ensures that the parameters align with the device's capabilities and specifications.
  
	- **Usage Note**: Recommended when the DMA configuration is determined at compile time, providing an additional layer of validation.

- **dma_set_trigger**: Functions identically but without compile-time checks.
  
	- **Usage Note**: Suitable for applications where DMA configurations are dynamic or not known beforehand.

### Parameters:

1. **dma_peripheral (dma_peripheral_t)**: Identifies the DMA peripheral to be configured.
2. **dma_channel (dma_channel_t)**: Specifies the DMA channel to set the trigger for.
3. **trigger (dma_trigger_t)**: Defines the trigger condition that initiates the DMA transfer.

### Return:

- **uhal_status_t**: Represents the success or failure of the function. A status of `UHAL_STATUS_OK` indicates successful configuration.

### Working:

1. Validates the specified DMA peripheral and channel, ensuring compatibility and availability.
2. Configures the DMA channel with the provided trigger condition.
3. Enables the DMA controller to respond to the trigger, allowing automated data transfer when the trigger event occurs.

## dma_reset_trigger function

```c
/* Function to remove an existing DMA trigger from a given channel (without compile-time parameter checking) */
uhal_status_t dma_reset_trigger(const dma_peripheral_t dma_peripheral, 
                                const dma_channel_t dma_channel, 
                                const dma_trigger_t trigger);

/* Function to remove an existing DMA trigger from a given channel (with compile-time parameter checking) */
uhal_status_t DMA_RESET_TRIGGER(const dma_peripheral_t dma_peripheral, 
                                const dma_channel_t dma_channel, 
                                const dma_trigger_t trigger);
```

### Description:

The `dma_reset_trigger` function is used to remove a previously set trigger from a specific DMA channel. Triggers in DMA are used to start transfers based on certain events or conditions. Removing a trigger stops these automatic initiations.

### Error Checking:

- **DMA_RESET_TRIGGER**: In this uppercase version, compile-time parameter checking is performed to validate the DMA peripheral, channel, and trigger parameters. This ensures compatibility and correctness of the configuration.
  
	- **Usage Note**: Best used when DMA settings are known at compile time, providing extra reliability in the configuration.

- **dma_reset_trigger**: This version operates identically but without compile-time checks.
  
	- **Usage Note**: Ideal for scenarios where DMA configurations may change dynamically or are determined at runtime.

### Parameters:

1. **dma_peripheral (dma_peripheral_t)**: Specifies the DMA peripheral associated with the trigger.
2. **dma_channel (dma_channel_t)**: Identifies the DMA channel from which the trigger is to be removed.
3. **trigger (dma_trigger_t)**: The specific trigger condition to be removed.

### Return:

- **uhal_status_t**: Indicates the success or failure of the operation. A return value of `UHAL_STATUS_OK` means the trigger was successfully removed.

### Working:

1. The function first checks the validity of the given DMA peripheral and channel.
2. It then proceeds to remove the specified trigger from the DMA channel.
3. This removal disables the previously configured automatic initiation of data transfer for the given condition, reverting the channel to manual or other trigger-based operations.

## dma_deinit function

```c
/* DMA driver deinitialization function (without compile-time parameter checking) */
uhal_status_t dma_deinit(const dma_peripheral_t dma_peripheral);

/* DMA driver deinitialization function (with compile-time parameter checking) */
uhal_status_t DMA_DEINIT(const dma_peripheral_t dma_peripheral);
```

### Description:

The `dma_deinit` and `DMA_DEINIT` functions are designed to deinitialize a specified DMA peripheral. Deinitialization is essential when a DMA peripheral is no longer needed, ensuring that resources are properly released and can be reallocated if necessary.

### Error Checking:

- **DMA_DEINIT**: This uppercase version includes compile-time parameter checking. It validates the `dma_peripheral` to ensure it corresponds to a correctly initialized and existing DMA peripheral. This check prevents errors related to deinitializing an invalid or non-existent peripheral.
  
	- **Usage Note**: Opt for this function when the DMA peripheral number is known at compile time, offering a more robust and error-proof deinitialization.

- **dma_deinit**: Performs the deinitialization process without compile-time checks, offering flexibility for scenarios where the DMA peripheral may be dynamically assigned or changed during operation.
  
	- **Usage Note**: Suitable for dynamic systems where the DMA peripheral might not be predetermined or varies based on runtime conditions.

### Parameters:

1. **dma_peripheral (dma_peripheral_t)**: The specific DMA peripheral instance to deinitialize.

### Return:

- **uhal_status_t**: Indicates the success or failure of the deinitialization process. A return value of `UHAL_STATUS_OK` means the peripheral was successfully deinitialized.

### Working:

1. The function validates the specified DMA peripheral.
2. It then proceeds to reset and release any resources or configurations associated with the DMA peripheral.
3. This process ensures that the DMA peripheral is turned off and is no longer consuming system resources, making it available for other tasks or reinitialization in the future.

## DMA IRQ Functionality

The DMA (Direct Memory Access) driver provides an essential Interrupt Request (IRQ) handler to manage various DMA events. This handler plays a critical role in responding to and managing the operations associated with DMA transactions.

### dma_irq_handler

This IRQ is triggered to handle events related to DMA operations.

```c
void dma_irq_handler(const void *const hw) __attribute__((weak));
```

### Functionality:

- **dma_irq_handler**: This default ISR (Interrupt Service Routine) for the DMA peripheral is invoked in response to events such as the completion of a data transfer, an error during an operation, or other DMA-specific triggers. By default, it is marked as a weak symbol, allowing developers to override it with their own implementation for more specialized handling of DMA events.

### Use-Case Example:

Consider a scenario where you're developing an embedded system that requires high-speed data transfer between different memory locations, or between memory and a hardware peripheral, without CPU intervention.

1. **Setting Up DMA**: You configure DMA channels to transfer data from a sensor (as a peripheral) to a memory buffer.
   
2. **Data Transfer Completion**: Once the data transfer is complete, the `dma_irq_handler` is automatically invoked. Here, you can check if the transfer was successful or if any errors occurred.

3. **Handling DMA Events**: Inside your custom `dma_irq_handler`, you can implement code to process the data, initiate another transfer, or handle errors.

4. **Continuous Operation**: For continuous data acquisition systems, the IRQ handler can be used to reset the DMA channel for the next data transfer, ensuring seamless data flow.

By customizing the `dma_irq_handler`, you can efficiently manage DMA operations tailored to the specific needs of your application, enhancing performance and reliability.

