# I2C API

## API functionality

The API for I2C host functionality has the following functions available:

```c
void i2c_init(const i2c_periph_inst_t* i2c_instance, unsigned long baud_rate);
void i2c_deinit(const i2c_periph_inst_t* i2c_instance);
void i2c_set_baud_rate(const i2c_periph_inst_t* i2c_instance, unsigned long baud_rate);
void i2c_write_blocking(const i2c_periph_inst_t* i2c_instance, unsigned char addr, const unsigned char* write_buff, size_t size, i2c_stop_bit_t stop_bit);
void i2c_write_non_blocking(const i2c_periph_inst_t* i2c_instance, unsigned short addr, const unsigned char* write_buff, size_t size, i2c_stop_bit_t stop_bit);
void i2c_read_blocking(const i2c_periph_inst_t* i2c_instance, unsigned short addr, unsigned char* read_buff, size_t amount_of_bytes);
void i2c_read_non_blocking(const i2c_periph_inst_t* i2c_instance, unsigned short addr, unsigned char* read_buff, size_t amount_of_bytes);
```

### i2c_init function

```c
void i2c_init(const i2c_periph_inst_t* i2c_instance, unsigned long baud_rate);
```

#### Description:

The i2c_init function is employed to initialize the I2C peripheral within a microcontroller for I2C communication. It does not involve any pin assignments or clock generator setups but ensures the hardware peripheral is ready for I2C operations.

#### Parameters:

1. i2c_instance (const i2c_periph_inst_t*):
	
	 A pointer to an instance of the I2C peripheral structure, which contains the information related to the I2C peripheral's configuration and state.
	 
2. baud_rate (unsigned long):
	
	 The desired baud rate for I2C communication, representing the frequency of the SCL line. Common values are 100 KHz or 400 KHz, but the function allows for other baud rates, including up to 1 MHz, depending on the hardware peripheral capabilities and the connected slave device requirements.

#### Return Type:

void:

- This function does not return any value.

#### Working:

1. The function takes a pointer to an I2C instance and a baud rate as inputs.
2. It initializes the I2C hardware peripheral pointed to by i2c_instance with the specified baud_rate.
3. The initialization process ensures the I2C peripheral is properly configured and ready for further I2C operations.

### i2c_deinit function

```c
void i2c_deinit(const i2c_periph_inst_t* i2c_instance);
```

#### Description:

The i2c_deinit function is used to de-initialize or reset the I2C peripheral to a non-operational state. This helps in conserving power and preventing further I2C operations, ensuring that the I2C peripheral is safely turned off.

#### Parameters:

1. i2c_instance (const i2c_periph_inst_t*):
   - A pointer to an instance of the I2C peripheral structure. This parameter holds information regarding the I2C peripheral's configuration and state.

#### Return Type:

void:

- This function does not return any value.

#### Working:

The i2c_deinit function takes a pointer to an I2C peripheral instance as an input.
It resets the I2C peripheral pointed to by i2c_instance, rendering it inactive and unavailable for further I2C communication.
This process is crucial for power management and ensuring the secure termination of I2C operations.

### i2c_set_baudrate function

```c
void i2c_set_baud_rate(const i2c_periph_inst_t* i2c_instance, unsigned long baud_rate);
```

#### Description:

The i2c_set_baud_rate function is used to reconfigure the baud rate of an already initialized I2C peripheral for communication. Adjusting the baud rate is essential for ensuring reliable communication with devices on the I2C bus, especially when changing the communication requirements or working with various devices with different baud rate specifications.

#### Parameters:

1. i2c_instance (const i2c_periph_inst_t*):

	A pointer to an instance of the I2C peripheral structure, holding the configuration and state information for the I2C peripheral.

2. baud_rate (unsigned long):
   
	The desired new baud rate for I2C communication. It represents the frequency of the SCL line. This value is typically 100 KHz or 400 KHz, but other values can be selected based on the hardware's capability and the slave devices' requirements.

#### Return:

void: 

- The function does not return any value. It’s used only for adjusting the baud rate of the I2C peripheral and doesn’t compute or check for a successful setup.

#### Working:

    The i2c_set_baud_rate function takes a pointer to an already initialized I2C peripheral instance and a desired new baud rate as inputs.
    It reconfigures the I2C peripheral pointed to by i2c_instance to operate at the specified baud_rate, allowing for adaptation to new communication speeds as required.

### i2c_write_blocking function

```c
void i2c_write_blocking(const i2c_periph_inst_t* i2c_instance, unsigned char addr, const unsigned char* write_buff, size_t size, i2c_stop_bit_t stop_bit);
```

#### Description:

The i2c_write_blocking function is used to execute a blocking write operation on an I2C bus. It sends a sequence of bytes to a specific I2C address. The function will block the execution until the write operation is complete.

#### Parameters:

1. i2c_instance (const i2c_periph_inst_t*):
   
	A pointer to an instance of the I2C peripheral structure. This parameter contains the information related to the I2C peripheral's configuration and state.

2. addr (unsigned char):
   
	The I2C address of the slave device to which the data will be sent.

3. write_buff (const unsigned char*):
   
	A pointer to the buffer containing the data to be written to the slave device.

4. size (size_t):
   
	The number of bytes to write from the write_buff to the slave device.

5. stop_bit (i2c_stop_bit_t):
   
	An enumeration type to specify whether a STOP bit should be sent after the write operation is complete.

#### Return Type:

void:

- This function does not return any value.

#### Working:

1. The function begins the write operation by sending the data from the write_buff to the slave device with address addr on the I2C bus defined by i2c_instance.
2. It will write size number of bytes from write_buff to the slave device.
3. The function will wait (block) until the complete data is written.
4. If stop_bit is set, it will send a STOP bit after completing the write operation to release the I2C bus.

### i2c_write_non_blocking function

```c
void i2c_write_non_blocking(const i2c_periph_inst_t* i2c_instance, unsigned short addr, const unsigned char* write_buff, size_t size, i2c_stop_bit_t stop_bit);
```

#### Description:

The i2c_write_non_blocking function executes a non-blocking write operation on an I2C bus. It sends a sequence of bytes to a specific I2C address and allows the program to continue executing while the write operation is underway.

#### Parameters:

1. i2c_instance (const i2c_periph_inst_t*):
   
	A pointer to an instance of the I2C peripheral structure, which houses the configuration and state information for the I2C peripheral.

2. addr (unsigned short):
   
	The I2C address of the slave device to which the data will be sent.

3. write_buff (const unsigned char*):
   
	A pointer to the buffer containing the data to be written to the slave device.

4. size (size_t):
   
	The number of bytes to be written from write_buff to the slave device.

5. stop_bit (i2c_stop_bit_t):
   
	An enumeration indicating whether a STOP bit should be sent after the write operation.

#### Return:

void:

- The function does not return any value, indicating it’s used for performing the read operation and storing the read data into the provided buffer without any success or failure indication.

#### Working:

1. The function starts the write operation by sending the data from write_buff to the slave device with address addr on the I2C bus specified by i2c_instance.
2. It will write size number of bytes from write_buff to the slave device.
   **Unlike the blocking write function, i2c_write_non_blocking will not halt the program execution until the write operation is complete.**
3. If stop_bit is set, a STOP bit will be sent after the write operation to release the I2C bus.

### i2c_read_blocking function

```c
void i2c_read_blocking(const i2c_periph_inst_t* i2c_instance, unsigned short addr, unsigned char* read_buff, size_t amount_of_bytes);
```

#### Description:

The i2c_read_blocking function performs a blocking read operation on an I2C bus. It reads a sequence of bytes from a specified I2C address into a buffer, halting the execution until the entire read operation is completed.

#### Parameters:

1. i2c_instance (const i2c_periph_inst_t*):
   
	 A pointer to an instance of the I2C peripheral structure. This parameter holds information about the I2C peripheral's configuration and state.

2. addr (unsigned short):
   
	 The I2C address of the slave device from which the data will be read.

3. read_buff (unsigned char*):
   
	 A pointer to the buffer where the read data will be stored.

4. amount_of_bytes (size_t):
   
	The number of bytes to be read from the slave device and stored into read_buff.

#### Return:

void:

- The function does not return any value, indicating it’s used for performing the read operation and storing the read data into the provided buffer without any success or failure indication.

#### Working:

1. The function begins the read operation by addressing the slave device with address addr on the I2C bus specified by i2c_instance.
2. It will read amount_of_bytes number of bytes from the slave device and store them into read_buff.
3. The i2c_read_blocking function will block or halt the program execution until the entire read operation is finished.

### i2c_read_non_blocking function

```c
void i2c_read_non_blocking(const i2c_periph_inst_t* i2c_instance, unsigned short addr, unsigned char* read_buff, size_t amount_of_bytes);
```

#### Description:

The i2c_read_non_blocking function performs a non-blocking read operation on an I2C bus. It reads a sequence of bytes from a specified I2C address into a buffer, allowing the program to continue executing while the read operation is underway.

#### Parameters:

1. i2c_instance (const i2c_periph_inst_t*):
   
	A pointer to an instance of the I2C peripheral structure. This parameter holds information about the I2C peripheral's configuration and state.

2. addr (unsigned short):
   
	The I2C address of the slave device from which the data will be read.

3. read_buff (unsigned char*):
   
	A pointer to the buffer where the read data will be stored.

4. amount_of_bytes (size_t):
   
	The number of bytes to be read from the slave device and stored into read_buff.

#### Return:

void:

- The function does not return any value, indicating it’s used for performing the read operation and storing the read data into the provided buffer without any success or failure indication.

#### Working:

1. The function initiates the read operation by addressing the slave device with address addr on the I2C bus specified by i2c_instance.

2. It will read amount_of_bytes number of bytes from the slave device and store them into read_buff.

3. Unlike the blocking read function, i2c_read_non_blocking will not halt the program execution until the read operation is complete.
