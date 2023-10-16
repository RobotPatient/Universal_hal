# I2C API

## API functionality

The API for I2C host functionality has the following functions available:

```c
/* I2C driver initialization function (without compile-time parameter checking)*/
uhal_status_t i2c_host_init(const i2c_periph_inst_t i2c_peripheral_num, 
							const i2c_clock_sources_t clock_sources,
                    		const uint32_t periph_clk_freq, 
                    		const uint32_t baud_rate_freq,
                    		const i2c_extra_opt_t extra_configuration_options);
/* I2C driver initialization function (with compile-time parameter checking)   */
uhal_status_t I2C_HOST_INIT(const i2c_periph_inst_t i2c_peripheral_num, 
							const i2c_clock_sources_t clock_sources, 
							const uint32_t periph_clk_freq, 
							const uint32_t baud_rate_freq, 
							const i2c_extra_opt_t extra_configuration_options);


/* I2C driver deinitialization function (without compile-time parameter checking) */
uhal_status_t i2c_host_deinit(const i2c_periph_inst_t i2c_peripheral_num);

/* I2C driver deinitialization function (with compile-time parameter checking) */
uhal_status_t I2C_HOST_DEINIT(const i2c_periph_inst_t i2c_peripheral_num);

/* I2C driver write blocking function (without compile-time parameter checking) */
uhal_status_t i2c_host_write_blocking(const i2c_periph_inst_t i2c_peripheral_num,
                             const uint16_t addr,
                             const uint8_t *write_buff,
                             const size_t size,
                             const i2c_stop_bit_t stop_bit);

/* I2C driver write blocking function (with compile-time parameter checking) */
uhal_status_t I2C_HOST_WRITE_BLOCKING(const i2c_periph_inst_t i2c_peripheral_num,
                             const uint16_t addr,
                             const uint8_t *write_buff,
                             const size_t size,
                             const i2c_stop_bit_t stop_bit);

/* I2C driver write non-blocking function (without compile-time parameter checking) */
uhal_status_t i2c_host_write_non_blocking(const i2c_periph_inst_t i2c_peripheral_num,
                                 const uint16_t addr,
                                 const uint8_t *write_buff,
                                 const size_t size,
                                 const i2c_stop_bit_t stop_bit);

/* I2C driver write non-blocking function (with compile-time parameter checking) */
uhal_status_t I2C_HOST_WRITE_NON_BLOCKING(const i2c_periph_inst_t i2c_peripheral_num,
                                 const uint16_t addr,
                                 const uint8_t *write_buff,
                                 const size_t size,
                                 const i2c_stop_bit_t stop_bit);

/* I2C driver read blocking function (without compile-time parameter checking) */
uhal_status_t i2c_host_read_blocking(const i2c_periph_inst_t i2c_peripheral_num,
                            const uint16_t addr,
                            uint8_t *read_buff,
                            const size_t amount_of_bytes);

/* I2C driver read blocking function (with compile-time parameter checking) */
uhal_status_t I2C_HOST_READ_BLOCKING(const i2c_periph_inst_t i2c_peripheral_num,
                            const uint16_t addr,
                            uint8_t *read_buff,
                            const size_t amount_of_bytes);

/* I2C driver read non-blocking function (without compile-time parameter checking) */
uhal_status_t i2c_host_read_non_blocking(const i2c_periph_inst_t i2c_peripheral_num,
                                const uint16_t addr,
                                uint8_t *read_buff,
                                const size_t amount_of_bytes);

/* I2C driver read non-blocking function (with compile-time parameter checking) */
uhal_status_t I2C_HOST_READ_NON_BLOCKING(const i2c_periph_inst_t i2c_peripheral_num,
                                const uint16_t addr,
                                uint8_t *read_buff,
                                const size_t amount_of_bytes);



```

## i2c_host_init function

```c
/* I2C driver initialization function (without compile-time parameter checking)*/
uhal_status_t i2c_host_init(const i2c_periph_inst_t i2c_peripheral_num, 
                            const i2c_clock_sources_t clock_sources,
                            const uint32_t periph_clk_freq, 
                            const uint32_t baud_rate_freq,
                            const i2c_extra_opt_t extra_configuration_options);

/* I2C driver initialization function (with compile-time parameter checking)   */
uhal_status_t I2C_HOST_INIT(const i2c_periph_inst_t i2c_peripheral_num, 
                            const i2c_clock_sources_t clock_sources, 
                            const uint32_t periph_clk_freq, 
                            const uint32_t baud_rate_freq, 
                            const i2c_extra_opt_t extra_configuration_options);
```

### Description:

The `i2c_host_init` and `I2C_HOST_INIT` functions are designed to initialize an I2C peripheral with specific configurations.

### Error Checking:

- **I2C_HOST_INIT**: Written in uppercase letters, this version of the function includes compile-time parameter checking to ensure the validity of the provided parameters. The parameters such as `i2c_peripheral_num` and `clock_sources` are checked against valid definitions at compile time. This safeguard helps detect configuration errors early in the development phase.
  
	- **Usage Note**: Utilize this function only when the parameters are known at compile time.

- **i2c_host_init**: This function provides the same I2C initialization capabilities but without the compile-time parameter check. 
  
	- **Usage Note**: Opt for this function when dealing with parameters that might be determined at runtime.

### Parameters:

1. **i2c_peripheral_num (const i2c_periph_inst_t)**: The specific I2C peripheral instance to initialize.
2. **clock_sources (const i2c_clock_sources_t)**: The clock source that will be used for the I2C peripheral.
3. **periph_clk_freq (const uint32_t)**: The frequency of the peripheral clock in Hertz.
4. **baud_rate_freq (const uint32_t)**: The desired baud rate frequency in Hertz for I2C communication.
5. **extra_configuration_options (const i2c_extra_opt_t)**: Additional configuration options to further customize the I2C peripheral behavior.

### Return:

- **uhal_status_t**: The function will return a status indicating the success or failure of the initialization process. The exact definitions of possible return values should be referenced in the "error_checking.h" header file.

### Working:

1. The function starts by validating the provided parameters, ensuring they're within acceptable ranges.
2. It then configures the clock source for the I2C peripheral based on the `clock_sources` parameter.
3. The function sets the peripheral clock frequency and baud rate as specified.
4. Any additional configurations specified in `extra_configuration_options` are applied.
5. Finally, the function activates the I2C peripheral, making it ready for communication.


## i2c_host_deinit function

```c
/* I2C driver deinitialization function (without compile-time parameter checking) */
uhal_status_t i2c_host_deinit(const i2c_periph_inst_t i2c_peripheral_num);

/* I2C driver deinitialization function (with compile-time parameter checking) */
uhal_status_t I2C_HOST_DEINIT(const i2c_periph_inst_t i2c_peripheral_num);
```

### Description:
The `i2c_host_deinit` and `I2C_HOST_DEINIT` functions deinitialize a previously initialized I2C peripheral.

### Error Checking:
- **I2C_HOST_DEINIT**: This uppercase version includes compile-time parameter checking for the peripheral number.
	- **Usage Note**: Use this when the peripheral number is known at compile time.
  
- **i2c_host_deinit**: This function does the deinitialization without compile-time checks.
	- **Usage Note**: Use this when the peripheral number might be determined at runtime.

### Parameters:
1. **i2c_peripheral_num (const i2c_periph_inst_t)**: The specific I2C peripheral instance to deinitialize.

### Return:
- **uhal_status_t**: Success or failure status of the deinitialization.

### Working:
1. Validates the peripheral number.
2. Resets the I2C peripheral settings and configurations.
3. Turns off the I2C peripheral, releasing its resources.



## i2c_host_write_blocking function

```c
/* I2C driver write blocking function (without compile-time parameter checking) */
uhal_status_t i2c_host_write_blocking(const i2c_periph_inst_t i2c_peripheral_num,
                             const uint16_t addr,
                             const uint8_t *write_buff,
                             const size_t size,
                             const i2c_stop_bit_t stop_bit);

/* I2C driver write blocking function (with compile-time parameter checking) */
uhal_status_t I2C_HOST_WRITE_BLOCKING(const i2c_periph_inst_t i2c_peripheral_num,
                             const uint16_t addr,
                             const uint8_t *write_buff,
                             const size_t size,
                             const i2c_stop_bit_t stop_bit);
```

### Description:
The `i2c_host_write_blocking` and `I2C_HOST_WRITE_BLOCKING` functions write data to a specified I2C address in a blocking manner.

### Error Checking:
- **I2C_HOST_WRITE_BLOCKING**: This uppercase version includes compile-time parameter checking.
	- **Usage Note**: Use this when parameters are known at compile time.

- **i2c_host_write_blocking**: Executes the write operation without compile-time checks.
	- **Usage Note**: Use this when parameters might be determined at runtime.

### Parameters:
1. **i2c_peripheral_num (const i2c_periph_inst_t)**: The specific I2C peripheral instance to use.
2. **addr (const uint16_t)**: The I2C address to write to.
3. **write_buff (const uint8_t*)**: Pointer to the buffer containing data to be written.
4. **size (const size_t)**: Amount of bytes to write.
5. **stop_bit (const i2c_stop_bit_t)**: Determines if a stop bit should be sent after the write.

### Return:
- **uhal_status_t**: Success or failure status of the write operation.

### Working:
1. Validates the parameters and ensures the I2C bus is ready.
2. Sends the data from the buffer to the specified address.
3. Waits for the operation to complete before returning.


## i2c_host_write_non_blocking function

```c
/* I2C driver write non-blocking function (without compile-time parameter checking) */
uhal_status_t i2c_host_write_non_blocking(const i2c_periph_inst_t i2c_peripheral_num,
                                 const uint16_t addr,
                                 const uint8_t *write_buff,
                                 const size_t size,
                                 const i2c_stop_bit_t stop_bit);

/* I2C driver write non-blocking function (with compile-time parameter checking) */
uhal_status_t I2C_HOST_WRITE_NON_BLOCKING(const i2c_periph_inst_t i2c_peripheral_num,
                                 const uint16_t addr,
                                 const uint8_t *write_buff,
                                 const size_t size,
                                 const i2c_stop_bit_t stop_bit);
```

### Description:
The `i2c_host_write_non_blocking` and `I2C_HOST_WRITE_NON_BLOCKING` functions write data to an I2C address without blocking the executing thread.

### Error Checking:
- **I2C_HOST_WRITE_NON_BLOCKING**: This uppercase version includes compile-time parameter checking.
	- **Usage Note**: Use this when parameters are known at compile time.

- **i2c_host_write_non_blocking**: Executes the write operation without compile-time checks.
	- **Usage Note**: Use this when parameters might be determined at runtime.

### Parameters:
The parameters are the same as the blocking version.

### Return:
- **uhal_status_t**: Success or failure status of the write operation.

### Working:
1. Validates the parameters and ensures the I2C bus is ready.
2. Starts the write operation and immediately returns.


## i2c_host_read_blocking function

```c
/* I2C driver read blocking function (without compile-time parameter checking) */
uhal_status_t i2c_host_read_blocking(const i2c_periph_inst_t i2c_peripheral_num,
                            const uint16_t addr,
                            uint8_t *read_buff,
                            const size_t amount_of_bytes);



/* I2C driver read blocking function (with compile-time parameter checking) */
uhal_status_t I2C_HOST_READ_BLOCKING(const i2c_periph_inst_t i2c_peripheral_num,
                            const uint16_t addr,
                            uint8_t *read_buff,
                            const size_t amount_of_bytes);
```

### Description:
The `i2c_host_read_blocking` and `I2C_HOST_READ_BLOCKING` functions read data from a specified I2C address, waiting for the operation to complete.

### Error Checking:
- **I2C_HOST_READ_BLOCKING**: This uppercase version includes compile-time parameter checking.
	- **Usage Note**: Use this when parameters are known at compile time.

- **i2c_host_read_blocking**: Executes the read operation without compile-time checks.
	- **Usage Note**: Use this when parameters might be determined at runtime.

### Parameters:
1. **i2c_peripheral_num (const i2c_periph_inst_t)**: The specific I2C peripheral instance to use.
2. **addr (const uint16_t)**: The I2C address to read from.
3. **read_buff (uint8_t*)**: Buffer where the read data will be stored.
4. **amount_of_bytes (const size_t)**: Amount of bytes to read.

### Return:
- **uhal_status_t**: Success or failure status of the read operation.

### Working:
1. Validates the parameters and ensures the I2C bus is ready.
2. Reads the data from the specified address and stores it in the buffer.
3. Waits for the operation to complete before returning.


## i2c_host_read_non_blocking function

```c
/* I2C driver read non-blocking function (without compile-time parameter checking) */
uhal_status_t i2c_host_read_non_blocking(const i2c_periph_inst_t i2c_peripheral_num,
                                const uint16_t addr,
                                uint8_t *read_buff,
                                const size_t amount_of_bytes);

/* I2C driver read non-blocking function (with compile-time parameter checking) */
uhal_status_t I2C_HOST_READ_NON_BLOCKING(const i2c_periph_inst_t i2c_peripheral_num,
                                const uint16_t addr,
                                uint8_t *read_buff,
                                const size_t amount_of_bytes);
```

### Description:
The `i2c_host_read_non_blocking` and `I2C_HOST_READ_NON_BLOCKING` functions read data from an I2C address without blocking the executing thread.

### Error Checking:
- **I2C_HOST_READ_NON_BLOCKING**: This uppercase version includes compile-time parameter checking.
	- **Usage Note**: Use this when parameters are known at compile time.

- **i2c_host_read_non_blocking**: Executes the read operation without compile-time checks.
	- **Usage Note**: Use this when parameters might be determined at runtime.

### Parameters:
The parameters are the same as the blocking version.

### Return:
- **uhal_status_t**: Success or failure status of the read operation.

### Working:
1. Validates the parameters and ensures the I2C bus is ready.
2. Starts the read operation and immediately returns.

