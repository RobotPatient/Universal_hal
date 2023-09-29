# I2C API

## API functionality

The API for I2C host functionality has the following functions available:

```c
void _i2c_host_init(const i2c_periph_inst_t i2c_peripheral_num, 
					const i2c_clock_sources_t clock_sources,
                    const uint32_t periph_clk_freq, 
                    const uint32_t baud_rate_freq,
                    const i2c_extra_opt_t extra_configuration_options);
                    

void _i2c_host_write_blocking(const i2c_periph_inst_t i2c_peripheral_num, 
							  const unsigned char addr,
                         	  const unsigned char *write_buff,
                         	  const size_t size,
                         	  const i2c_stop_bit_t stop_bit);
                         	  
void _i2c_host_write_non_blocking(const i2c_periph_inst_t i2c_peripheral_num, 
								  const unsigned short addr,
                             	  const unsigned char *write_buff,
                             	  const size_t size,
                             	  const i2c_stop_bit_t stop_bit);
                             	  
void _i2c_host_read_blocking(const i2c_periph_inst_t i2c_peripheral_num, 
							 const unsigned short addr, 
							 unsigned char *read_buff,
                        	 const size_t amount_of_bytes);
                        	 
void _i2c_host_read_non_blocking(const i2c_periph_inst_t i2c_peripheral_num, 
								 const unsigned short addr, 
								 unsigned char* read_buff, 
								 const size_t amount_of_bytes);
								 
void _i2c_host_deinit(const i2c_periph_inst_t i2c_peripheral_num);
```

### _i2c_host_init function

```c
void _i2c_host_init(const i2c_periph_inst_t i2c_peripheral_num, 
					const i2c_clock_sources_t clock_sources,
                    const uint32_t periph_clk_freq, 
                    const uint32_t baud_rate_freq,
                    const i2c_extra_opt_t extra_configuration_options);
```

#### Description:

The _i2c_host_init function is employed to initialize the I2C peripheral within a microcontroller for I2C communication. It does not involve any pin assignments or clock generator setups but ensures the hardware peripheral is ready for I2C operations.

#### Parameters:

1. i2c_peripheral_num (const i2c_periph_inst_t):
	
	 The hardware peripheral to use. See the platform dependent `i2c_periph_inst_t` enum and the API platform -> platform_name -> Usage page for the details.

2. clock_sources (const i2c_clock_sources_t):

	The hardware clock source to use for configuring the hardware peripheral. 
	
3. periph_clk_freq (const uint32_t)

	The frequency of clock on which the hardware peripheral will be configured.
	
4. baud_rate_freq (unsigned long):
	
	The desired baud rate for I2C communication, representing the frequency of the SCL line. Common values are 100 KHz or 400 KHz, but the function allows for other baud rates, including up to 1 MHz, depending on the hardware peripheral capabilities and the connected slave device requirements.

5. extra_configuration_options (const i2c_extra_opt_t):

	Extra configuration options that are platform-dependent can be set here. Things like DMA use, IRQ priority's, etc.

#### Return Type:

void:

- This function does not return any value.

#### Working:

1. The function takes the configuration options as inputs.
2. It initializes the I2C hardware peripheral using the given clock settings with the specified baud_rate.
3. The initialization process ensures the I2C peripheral is properly configured and ready for further I2C operations.

### _i2c_host_write_blocking function

```c
void _i2c_host_write_blocking(const i2c_periph_inst_t i2c_peripheral_num, 
							  const unsigned char addr,
                         	  const unsigned char *write_buff,
                         	  const size_t size,
                         	  const i2c_stop_bit_t stop_bit);
```

#### Description:

The _i2c_host_write_blocking function is used to execute a blocking write operation on an I2C bus. It sends a sequence of bytes to a specific I2C address. The function will block the execution until the write operation is complete.

#### Parameters:

1. i2c_peripheral_num (const i2c_periph_inst_t):
   
	The hardware peripheral to use. See the platform dependent `i2c_periph_inst_t` enum and the API platform -> platform_name -> Usage page for the details.

2. addr (const unsigned char):
   
	The I2C address of the slave device to which the data will be sent.

3. write_buff (const unsigned char*):
   
	A pointer to the buffer containing the data to be written to the slave device.

4. size (const size_t):
   
	The number of bytes to write from the write_buff to the slave device.

5. stop_bit (const i2c_stop_bit_t):
   
	An enumeration type to specify whether a STOP bit should be sent after the write operation is complete.

#### Return Type:

void:

- This function does not return any value.

#### Working:

1. The function begins the write operation by sending the data from the write_buff to the slave device with address addr on the I2C bus defined by i2c_peripheral_num.
2. It will write size number of bytes from write_buff to the slave device.
3. The function will wait (block) until the complete data is written.
4. If stop_bit is set, it will send a STOP bit after completing the write operation to release the I2C bus.

### _i2c_host_write_non_blocking function

```c
void _i2c_host_write_non_blocking(const i2c_periph_inst_t i2c_peripheral_num, 
								  const unsigned short addr,
                             	  const unsigned char *write_buff,
                             	  const size_t size,
                             	  const i2c_stop_bit_t stop_bit);
```

#### Description:

The _i2c_host_write_non_blocking function executes a non-blocking write operation on an I2C bus. It sends a sequence of bytes to a specific I2C address and allows the program to continue executing while the write operation is underway.

#### Parameters:

1. i2c_peripheral_num (const i2c_periph_inst_t):
   
	The hardware peripheral to use. See the platform dependent `i2c_periph_inst_t` enum and the API platform -> platform_name -> Usage page for the details.

2. addr (const unsigned short):
   
	The I2C address of the slave device to which the data will be sent.

3. write_buff (const unsigned char*):
   
	A pointer to the buffer containing the data to be written to the slave device.

4. size (const size_t):
   
	The number of bytes to be written from write_buff to the slave device.

5. stop_bit (const i2c_stop_bit_t):
   
	An enumeration indicating whether a STOP bit should be sent after the write operation.

#### Return:

void:

- The function does not return any value, indicating it’s used for performing the read operation and storing the read data into the provided buffer without any success or failure indication.

#### Working:

1. The function starts the write operation by sending the data from write_buff to the slave device with address addr on the I2C bus specified by i2c_peripheral_num.
2. It will write size number of bytes from write_buff to the slave device.
   **Unlike the blocking write function, _i2c_host_write_non_blocking will not halt the program execution until the write operation is complete.**
3. If stop_bit is set, a STOP bit will be sent after the write operation to release the I2C bus.

### _i2c_host_read_blocking function

```c
void _i2c_host_read_blocking(const i2c_periph_inst_t i2c_peripheral_num, 
							 const unsigned short addr, 
							 unsigned char *read_buff,
                        	 const size_t amount_of_bytes);
```

#### Description:

The _i2c_host_read_blocking function performs a blocking read operation on an I2C bus. It reads a sequence of bytes from a specified I2C address into a buffer, halting the execution until the entire read operation is completed.

#### Parameters:

1. i2c_peripheral_num (const i2c_periph_inst_t):
   
	 The hardware peripheral to use. See the platform dependent `i2c_periph_inst_t` enum and the API platform -> platform_name -> Usage page for the details.

2. addr (const unsigned short):
   
	 The I2C address of the slave device from which the data will be read.

3. read_buff (unsigned char*):
   
	 A pointer to the buffer where the read data will be stored.

4. amount_of_bytes (const size_t):
   
	The number of bytes to be read from the slave device and stored into read_buff.

#### Return:

void:

- The function does not return any value, indicating it’s used for performing the read operation and storing the read data into the provided buffer without any success or failure indication.

#### Working:

1. The function begins the read operation by addressing the slave device with address addr on the I2C bus specified by i2c_peripheral_num.
2. It will read amount_of_bytes number of bytes from the slave device and store them into read_buff.
3. The _i2c_host_read_blocking function will block or halt the program execution until the entire read operation is finished.

### _i2c_host_read_non_blocking function

```c
void _i2c_host_read_non_blocking(const i2c_periph_inst_t i2c_peripheral_num, 
								 const unsigned short addr, 
								 unsigned char* read_buff, 
								 const size_t amount_of_bytes);
```

#### Description:

The _i2c_host_read_non_blocking function performs a non-blocking read operation on an I2C bus. It reads a sequence of bytes from a specified I2C address into a buffer, allowing the program to continue executing while the read operation is underway.

#### Parameters:

1. i2c_peripheral_num (const i2c_periph_inst_t):
   
	The hardware peripheral to use. See the platform dependent `i2c_periph_inst_t` enum and the API platform -> platform_name -> Usage page for the details.

2. addr (const unsigned short):
   
	The I2C address of the slave device from which the data will be read.

3. read_buff (unsigned char*):
   
	A pointer to the buffer where the read data will be stored.

4. amount_of_bytes (const size_t):
   
	The number of bytes to be read from the slave device and stored into read_buff.

#### Return:

void:

- The function does not return any value, indicating it’s used for performing the read operation and storing the read data into the provided buffer without any success or failure indication.

#### Working:

1. The function initiates the read operation by addressing the slave device with address addr on the I2C bus specified by i2c_peripheral_num.

2. It will read amount_of_bytes number of bytes from the slave device and store them into read_buff.

3. Unlike the blocking read function, i2c_read_non_blocking will not halt the program execution until the read operation is complete.

### _i2c_host_deinit function

```c
void _i2c_host_deinit(const i2c_periph_inst_t i2c_peripheral_num);
```

#### Description:

The _i2c_host_deinit function is used to de-initialize or reset the I2C peripheral to a non-operational state. This helps in conserving power and preventing further I2C operations, ensuring that the I2C peripheral is safely turned off.

#### Parameters:

1. i2c_peripheral_num (const i2c_periph_inst_t):
	
	The hardware peripheral to use. See the platform dependent `i2c_periph_inst_t` enum and the API platform -> platform_name -> Usage page for the details.

#### Return Type:

void:

- This function does not return any value.

#### Working:

1. The _i2c_host_deinit function takes the I2C peripheral instance as an input.

2. It resets the I2C peripheral pointed to by i2c_peripheral_num, rendering it inactive and unavailable for further I2C communication.

This process is crucial for power management and ensuring the secure termination of I2C operations.



