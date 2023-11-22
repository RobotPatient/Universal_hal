# SPI host API

## API functionality

The API for SPI host functionality has the following functions available:

```c
/* SPI driver initialization function (without compile-time parameter checking) */
uhal_status_t spi_host_init(const spi_host_inst_t spi_peripheral_num, 
                            const uint32_t spi_clock_source,
                            const uint32_t spi_clock_source_freq, 
                            const unsigned long spi_bus_frequency,
                            const spi_bus_opt_t spi_extra_configuration_opt);

/* SPI driver initialization function (with compile-time parameter checking) */
uhal_status_t SPI_HOST_INIT(const spi_host_inst_t spi_peripheral_num, 
                            const uint32_t spi_clock_source, 
                            const uint32_t spi_clock_source_freq, 
                            const unsigned long spi_bus_frequency, 
                            const spi_bus_opt_t spi_extra_configuration_opt);

/* SPI driver deinitialization function (without compile-time parameter checking) */
uhal_status_t spi_host_deinit(const spi_host_inst_t spi_peripheral_num);

/* SPI driver deinitialization function (with compile-time parameter checking) */
uhal_status_t SPI_HOST_DEINIT(const spi_host_inst_t spi_peripheral_num);

/* SPI driver start transaction function (without compile-time parameter checking) */
uhal_status_t spi_host_start_transaction(const spi_host_inst_t spi_peripheral_num, 
                                         const gpio_pin_t chip_select_pin,
                                         const spi_extra_dev_opt_t device_specific_config_opt);

/* SPI driver start transaction function (with compile-time parameter checking) */
uhal_status_t SPI_HOST_START_TRANSACTION(const spi_host_inst_t spi_peripheral_num, 
                                         const gpio_pin_t chip_select_pin,
                                         const spi_extra_dev_opt_t device_specific_config_opt);

/* SPI driver end transaction function (without compile-time parameter checking) */
uhal_status_t spi_host_end_transaction(const spi_host_inst_t spi_peripheral_num, 
                                       const gpio_pin_t chip_select_pin);

/* SPI driver end transaction function (with compile-time parameter checking) */
uhal_status_t SPI_HOST_END_TRANSACTION(const spi_host_inst_t spi_peripheral_num, 
                                       const gpio_pin_t chip_select_pin);

/* SPI driver write blocking function (without compile-time parameter checking) */
uhal_status_t spi_host_write_blocking(const spi_host_inst_t spi_peripheral_num, 
                                      const unsigned char *write_buff, 
                                      const size_t size);

/* SPI driver write blocking function (with compile-time parameter checking) */
uhal_status_t SPI_HOST_WRITE_BLOCKING(const spi_host_inst_t spi_peripheral_num, 
                                      const unsigned char *write_buff, 
                                      const size_t size);

/* SPI driver write non-blocking function (without compile-time parameter checking) */
uhal_status_t spi_host_write_non_blocking(const spi_host_inst_t spi_peripheral_num, 
                                          const unsigned char *write_buff, 
                                          const size_t size);

/* SPI driver write non-blocking function (with compile-time parameter checking) */
uhal_status_t SPI_HOST_WRITE_NON_BLOCKING(const spi_host_inst_t spi_peripheral_num, 
                                          const unsigned char *write_buff, 
                                          const size_t size);

/* SPI driver read blocking function (without compile-time parameter checking) */
uhal_status_t spi_host_read_blocking(const spi_host_inst_t spi_peripheral_num, 
                                     unsigned char *read_buff, 
                                     size_t amount_of_bytes);

/* SPI driver read blocking function (with compile-time parameter checking) */
uhal_status_t SPI_HOST_READ_BLOCKING(const spi_host_inst_t spi_peripheral_num, 
                                     unsigned char *read_buff, 
                                     size_t amount_of_bytes);

/* SPI driver read non-blocking function (without compile-time parameter checking) */
uhal_status_t spi_host_read_non_blocking(const spi_host_inst_t spi_peripheral_num, 
                                         unsigned char *read_buff, 
                                         size_t amount_of_bytes);

/* SPI driver read non-blocking function (with compile-time parameter checking) */
uhal_status_t SPI_HOST_READ_NON_BLOCKING(const spi_host_inst_t spi_peripheral_num, 
                                         unsigned char *read_buff, 
                                         size_t amount_of_bytes);


```

## spi_host_init function

```c
/* SPI driver initialization function (without compile-time parameter checking) */
uhal_status_t spi_host_init(const spi_host_inst_t spi_peripheral_num, 
                            const uint32_t spi_clock_source,
                            const uint32_t spi_clock_source_freq, 
                            const unsigned long spi_bus_frequency,
                            const spi_bus_opt_t spi_extra_configuration_opt);

/* SPI driver initialization function (with compile-time parameter checking) */
uhal_status_t SPI_HOST_INIT(const spi_host_inst_t spi_peripheral_num, 
                            const uint32_t spi_clock_source, 
                            const uint32_t spi_clock_source_freq, 
                            const unsigned long spi_bus_frequency, 
                            const spi_bus_opt_t spi_extra_configuration_opt);
```

### Description:

The `spi_host_init` and `SPI_HOST_INIT` functions are designed to initialize an SPI peripheral with specific configurations.

### Error Checking:

- **SPI_HOST_INIT**: Written in uppercase letters, this version of the function includes compile-time parameter checking to ensure the validity of the provided parameters. The parameters such as `spi_peripheral_num` and `spi_clock_source` are checked against valid definitions at compile time. This safeguard helps detect configuration errors early in the development phase.
  
	- **Usage Note**: Utilize this function only when the parameters are known at compile time.

- **spi_host_init**: This function provides the same SPI initialization capabilities but without the compile-time parameter check. 
  
	- **Usage Note**: Opt for this function when dealing with parameters that might be determined at runtime.

### Parameters:

1. **spi_peripheral_num (const spi_host_inst_t)**: The specific SPI peripheral instance to initialize.
2. **spi_clock_source (const uint32_t)**: The clock source that will be used for configuring the SPI peripheral.
3. **spi_clock_source_freq (const uint32_t)**: The frequency of the clock source in Hertz.
4. **spi_bus_frequency (const unsigned long)**: The desired frequency/baud rate for SPI communication.
5. **spi_extra_configuration_opt (const spi_bus_opt_t)**: Extra configuration options for customizing the SPI host driver.

### Return:

- **uhal_status_t**: The function will return a status indicating the success or failure of the initialization process. The exact definitions of possible return values should be referenced in the "error_checking.h" header file.

### Working:

1. The function begins by validating the provided parameters to ensure they're within acceptable ranges.
2. It configures the clock source for the SPI peripheral based on the `spi_clock_source` parameter.
3. The function sets the clock source frequency and SPI bus frequency as specified.
4. Any additional configurations specified in `spi_extra_configuration_opt` are applied.
5. Finally, the function activates the SPI peripheral, making it ready for communication transactions.

## spi_host_deinit function

```c
/* SPI driver deinitialization function (without compile-time parameter checking) */
uhal_status_t spi_host_deinit(const spi_host_inst_t spi_peripheral_num);

/* SPI driver deinitialization function (with compile-time parameter checking) */
uhal_status_t SPI_HOST_DEINIT(const spi_host_inst_t spi_peripheral_num);
```

### Description:
The `spi_host_deinit` and `SPI_HOST_DEINIT` functions are responsible for deinitializing a previously initialized SPI peripheral.

### Error Checking:
- **SPI_HOST_DEINIT**: This uppercase version includes compile-time parameter checking for the peripheral number.
	- **Usage Note**: Use this function when the peripheral number is known at compile time, as it provides an additional layer of safety by ensuring that the provided parameter is valid.
  
- **spi_host_deinit**: This function performs the deinitialization without compile-time parameter checks.
	- **Usage Note**: Use this function when the peripheral number might be determined at runtime or when compile-time checks are not necessary.

### Parameters:
1. **spi_peripheral_num (const spi_host_inst_t)**: The specific SPI peripheral instance to deinitialize.

### Return:
- **uhal_status_t**: Indicates the success or failure status of the deinitialization process. This can be used to verify whether the peripheral was successfully turned off and its resources were released.

### Working:
1. The function first validates the provided SPI peripheral number to ensure it refers to a valid and previously initialized peripheral.
2. It then proceeds to reset any specific settings and configurations that were applied to the SPI peripheral during initialization.
3. Finally, the function turns off the SPI peripheral, effectively releasing its resources and making it unavailable for further communication until reinitialized.

## spi_host_start_transaction function

```c
/* SPI driver start transaction function (without compile-time parameter checking) */
uhal_status_t spi_host_start_transaction(const spi_host_inst_t spi_peripheral_num, 
                                         const gpio_pin_t chip_select_pin,
                                         const spi_extra_dev_opt_t device_specific_config_opt);

/* SPI driver start transaction function (with compile-time parameter checking) */
uhal_status_t SPI_HOST_START_TRANSACTION(const spi_host_inst_t spi_peripheral_num, 
                                         const gpio_pin_t chip_select_pin,
                                         const spi_extra_dev_opt_t device_specific_config_opt);
```

### Description:
The `spi_host_start_transaction` and `SPI_HOST_START_TRANSACTION` functions are designed to initiate a transaction with an SPI peripheral. This involves setting up the specified SPI device and preparing it for either a read or write operation.

### Error Checking:
- **SPI_HOST_START_TRANSACTION**: This uppercase version of the function includes compile-time parameter checking. It verifies the correctness of the provided parameters, ensuring they are valid for the intended SPI transaction.
	- **Usage Note**: This function should be used when parameters are known and can be validated at compile time, offering a safer and more reliable setup.

- **spi_host_start_transaction**: This version performs the transaction setup without compile-time parameter checks. It offers more flexibility for dynamically determined parameters.
	- **Usage Note**: Opt for this function in scenarios where the parameters are not fixed and could be subject to change at runtime.

### Parameters:
1. **spi_peripheral_num (const spi_host_inst_t)**: Identifies the specific SPI peripheral instance to be used for the transaction.
2. **chip_select_pin (const gpio_pin_t)**: Specifies the GPIO pin used for the chip select (CS) line, which is used to activate the SPI peripheral for the transaction.
3. **device_specific_config_opt (const spi_extra_dev_opt_t)**: Additional device-specific configuration options that may be necessary for certain SPI devices or specific transaction requirements.

### Return:
- **uhal_status_t**: The function returns a status indicating the success or failure of initiating the SPI transaction. A successful return value implies that the SPI device is ready and properly configured for the subsequent operation.

### Working:
1. The function first validates the provided SPI peripheral number and the chip select pin to ensure they correspond to a valid and correctly configured SPI device.
2. It then applies any device-specific configuration options provided, which might include settings like SPI mode, bit order, or speed adjustments.
3. Once the setup is complete, the function activates the chip select line, signaling the SPI device that it is about to participate in a transaction.
4. Finally, the SPI peripheral is now ready for a data read or write operation, and the function returns a status indicating the readiness of the device.

## spi_host_end_transaction function

```c
/* SPI driver end transaction function (without compile-time parameter checking) */
uhal_status_t spi_host_end_transaction(const spi_host_inst_t spi_peripheral_num, 
                                       const gpio_pin_t chip_select_pin);

/* SPI driver end transaction function (with compile-time parameter checking) */
uhal_status_t SPI_HOST_END_TRANSACTION(const spi_host_inst_t spi_peripheral_num, 
                                       const gpio_pin_t chip_select_pin);
```

### Description:
The `spi_host_end_transaction` and `SPI_HOST_END_TRANSACTION` functions are used to conclude a transaction with an SPI peripheral. This typically involves deactivating the chip select line and ensuring the SPI peripheral is properly reset or left in a state ready for the next transaction.

### Error Checking:
- **SPI_HOST_END_TRANSACTION**: This uppercase version includes compile-time parameter checking, ensuring the validity and correctness of the SPI peripheral number and the chip select pin.
	- **Usage Note**: Use this function for scenarios where parameters are predetermined and require validation at compile time, as it provides an added layer of safety and error checking.

- **spi_host_end_transaction**: This function operates without compile-time checks, offering more flexibility in scenarios where parameters are dynamically determined or may change at runtime.
	- **Usage Note**: Ideal for situations where the parameters are not known beforehand or are subject to variation during the program's execution.

### Parameters:
1. **spi_peripheral_num (const spi_host_inst_t)**: Specifies the SPI peripheral instance involved in the transaction.
2. **chip_select_pin (const gpio_pin_t)**: The GPIO pin used for the chip select (CS) line, which needs to be deactivated to signal the end of the transaction.

### Return:
- **uhal_status_t**: Indicates the success or failure of ending the SPI transaction. A successful return implies that the transaction has been properly concluded, and the SPI device is either reset or ready for the next operation.

### Working:
1. The function starts by validating the SPI peripheral number and the chip select pin, ensuring they are correct and have been used in the ongoing transaction.
2. It then proceeds to deactivate the chip select line, signaling to the SPI peripheral that the transaction is complete.
3. Any necessary cleanup or reconfiguration steps are performed to ensure the SPI peripheral is left in a consistent and ready state.
4. Finally, the function returns a status indicating whether the transaction was successfully concluded.

## spi_host_write_blocking function

```c
/* SPI driver write blocking function (without compile-time parameter checking) */
uhal_status_t spi_host_write_blocking(const spi_host_inst_t spi_peripheral_num,
                                      const unsigned char *write_buff,
                                      const size_t size);

/* SPI driver write blocking function (with compile-time parameter checking) */
uhal_status_t SPI_HOST_WRITE_BLOCKING(const spi_host_inst_t spi_peripheral_num,
                                      const unsigned char *write_buff,
                                      const size_t size);
```

### Description:
The `spi_host_write_blocking` and `SPI_HOST_WRITE_BLOCKING` functions handle the transmission of data over SPI in a blocking manner, meaning the function will wait until all data is transmitted before returning control.

### Error Checking:
- **SPI_HOST_WRITE_BLOCKING**: This uppercase version includes compile-time parameter checking, ensuring that the provided parameters are correct before the function is executed.
	- **Usage Note**: This function is ideal when parameters are known at compile time and additional safety checks are desired.

- **spi_host_write_blocking**: Performs the data transmission without compile-time checks, allowing for greater flexibility with runtime-determined parameters.
	- **Usage Note**: This function is suitable when parameters are dynamic or determined at runtime.

### Parameters:
1. **spi_peripheral_num (const spi_host_inst_t)**: The specific SPI peripheral instance to use for data transmission.
2. **write_buff (const unsigned char*)**: Pointer to the buffer containing the data to be written.
3. **size (const size_t)**: The number of bytes to write from the buffer.

### Return:
- **uhal_status_t**: This return type indicates the success or failure status of the write operation. A successful return means all data was transmitted correctly.

### Working:
1. The function first validates the parameters and prepares the SPI peripheral for data transmission.
2. It then sends the data byte-by-byte from the provided buffer to the SPI peripheral. During this process, the function blocks any further execution until the entire buffer is transmitted.
3. After all data is sent, the function returns, indicating whether the transmission was successful.


## spi_host_write_non_blocking function

```c
/* SPI driver write non-blocking function (without compile-time parameter checking) */
uhal_status_t spi_host_write_non_blocking(const spi_host_inst_t spi_peripheral_num,
                                          const unsigned char *write_buff,
                                          const size_t size);

/* SPI driver write non-blocking function (with compile-time parameter checking) */
uhal_status_t SPI_HOST_WRITE_NON_BLOCKING(const spi_host_inst_t spi_peripheral_num,
                                          const unsigned char *write_buff,
                                          const size_t size);
```

### Description:
The `spi_host_write_non_blocking` and `SPI_HOST_WRITE_NON_BLOCKING` functions are used to transmit data over SPI in a non-blocking manner. This means the function initiates the transmission and returns control immediately without waiting for the entire data transfer to complete.

### Error Checking:
- **SPI_HOST_WRITE_NON_BLOCKING**: This uppercase version includes compile-time parameter checking, which ensures that parameters passed to the function are valid and correct as per the expected types and values.
	- **Usage Note**: This version is particularly useful when parameters are known at compile time and when it's critical to ensure their correctness before execution.

- **spi_host_write_non_blocking**: Offers the same functionality as its uppercase counterpart but does not include compile-time parameter checking. This version is more flexible for situations where parameters may be determined at runtime.
	- **Usage Note**: Ideal for use cases where parameters are not known beforehand or may change dynamically.

### Parameters:
1. **spi_peripheral_num (const spi_host_inst_t)**: Specifies the SPI peripheral instance to be used for the transmission.
2. **write_buff (const unsigned char*)**: A pointer to the buffer containing the data that needs to be transmitted.
3. **size (const size_t)**: The number of bytes to transmit from the buffer.

### Return:
- **uhal_status_t**: The return status indicates whether the transmission was successfully initiated. A successful return means the data transmission process has begun.

### Working:
1. The function first checks the readiness of the SPI peripheral for data transmission.
2. It then initiates the transmission of data from the provided buffer. This process is non-blocking, meaning the function does not wait for the complete transmission of all bytes.
3. After initiating the transmission, the function quickly returns control to the calling program, allowing other operations to proceed while the SPI peripheral continues to transmit data in the background.

## spi_host_read_blocking function

```c
/* SPI driver read blocking function (without compile-time parameter checking) */
uhal_status_t spi_host_read_blocking(const spi_host_inst_t spi_peripheral_num,
                                     unsigned char *read_buff,
                                     size_t amount_of_bytes);

/* SPI driver read blocking function (with compile-time parameter checking) */
uhal_status_t SPI_HOST_READ_BLOCKING(const spi_host_inst_t spi_peripheral_num,
                                     unsigned char *read_buff,
                                     size_t amount_of_bytes);
```

### Description:
The `spi_host_read_blocking` and `SPI_HOST_READ_BLOCKING` functions are used for receiving data over SPI in a blocking manner. In this mode, the function will wait until the specified amount of data is received before returning.

### Error Checking:
- **SPI_HOST_READ_BLOCKING**: This uppercase version includes compile-time parameter checking. It validates that the parameters provided are correct according to their expected types and values, enhancing safety and reducing runtime errors.
	- **Usage Note**: Opt for this function when parameters are known at compile time and need validation for correctness.

- **spi_host_read_blocking**: Performs the read operation without compile-time checks. This version is more adaptable to situations where parameters may be determined dynamically at runtime.
	- **Usage Note**: Use this version in scenarios where parameters are not fixed at compile time or when they are subject to change.

### Parameters:
1. **spi_peripheral_num (const spi_host_inst_t)**: Identifies the specific SPI peripheral instance to use for reading data.
2. **read_buff (unsigned char*)**: A pointer to the buffer where the received data will be stored.
3. **amount_of_bytes (size_t)**: The number of bytes to read into the buffer.

### Return:
- **uhal_status_t**: The return status indicates the success or failure of the read operation. A successful return means that the requested amount of data has been received and stored in the provided buffer.

### Working:
1. The function first validates the readiness of the SPI peripheral for data reception.
2. It then begins reading data byte-by-byte from the SPI peripheral. This process is blocking, meaning the function will not return until the entire amount of data specified has been received.
3. After the complete reception of data, the function returns, indicating whether the read operation was successful.

## spi_host_read_non_blocking function

```c
/* SPI driver read non-blocking function (without compile-time parameter checking) */
uhal_status_t spi_host_read_non_blocking(const spi_host_inst_t spi_peripheral_num,
                                         unsigned char *read_buff,
                                         size_t amount_of_bytes);

/* SPI driver read non-blocking function (with compile-time parameter checking) */
uhal_status_t SPI_HOST_READ_NON_BLOCKING(const spi_host_inst_t spi_peripheral_num,
                                         unsigned char *read_buff,
                                         size_t amount_of_bytes);
```

### Description:
The `spi_host_read_non_blocking` and `SPI_HOST_READ_NON_BLOCKING` functions facilitate the reception of data over SPI in a non-blocking manner. This approach means the function initiates the read operation and returns control immediately, without waiting for the entire data reception to complete.

### Error Checking:
- **SPI_HOST_READ_NON_BLOCKING**: This uppercase version includes compile-time parameter checking, ensuring the correctness and validity of the provided parameters. This additional check is beneficial for ensuring parameter integrity when they are known at compile time.
	- **Usage Note**: Use this function for scenarios where the parameters are predetermined and where compile-time validation adds an extra layer of safety.

- **spi_host_read_non_blocking**: Offers the same functionality but without compile-time parameter checks, providing greater flexibility in cases where parameters might be determined at runtime.
	- **Usage Note**: This version is suitable for dynamic situations where parameters may vary or are not known in advance.

### Parameters:
1. **spi_peripheral_num (const spi_host_inst_t)**: Specifies which SPI peripheral instance to use for the read operation.
2. **read_buff (unsigned char*)**: Points to the buffer that will store the received data.
3. **amount_of_bytes (size_t)**: Defines the number of bytes to be read into the buffer.

### Return:
- **uhal_status_t**: This return type signifies the success or initiation of the read operation. A successful return implies that the data reception process has started.

### Working:
1. The function begins by ensuring that the SPI peripheral is ready for data reception.
2. It then initiates the read operation, which is designed to be non-blocking. The function does not wait for the full completion of data reception; instead, it starts the process and then returns control to the calling program immediately.
3. The function exits quickly after starting the data reception, allowing other operations to proceed concurrently while the SPI peripheral continues to receive data in the background.


