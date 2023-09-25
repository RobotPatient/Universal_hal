# I2C API

## Platform compatibility:

The standard I2C API for this framework has basic I2C Master and I2C Slave functionality. However not every platform has I2C peripherals or supports both I2C Master and I2C Slave functionality.

| Platform   | I2C Master support  | I2C Slave support   | HW peripheral/Bitbanged |
| ---------- | ------------------- | ------------------- | ----------------------- |
| Atmel SAMD | ✔                   | ✔                   | HW peripheral           |
| RPI RP2040 | Not yet implemented | Not yet implemented | HW peripheral           |

## How does the I2C bus work?

The Inter-Integrated Circuit (I2C) protocol is a popular communication protocol that microcontrollers and microprocessors use to communicate with various external devices, like sensors, memory devices, and other microcontrollers. It’s a simple, two-wire serial interface that’s ideal for interconnecting low-speed peripherals to a motherboard, embedded system, or cellphone.

### Basic Concepts

1. **Master and Slave Devices:**
	- **Master Device:** Initiates and controls the data transfer.
	- **Slave Device:** Responds to the master device.
  
2. **Two-Wire Communication:**
	- **SDA (Serial Data Line):** Carries data.
	- **SCL (Serial Clock Line):** Synchronizes data transfer.

### Working Principle

1. The master device initiates communication.
2. The master device sends the address of the specific slave device it wants to communicate with, and whether it wants to write or read data from the slave device. 
3. The master sends or requests data to/from the slave.
4. The slave sends an acknowledgment bit for each byte received.
5. The master ends the communication.

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
	- A pointer to an instance of the I2C peripheral structure, which contains the information related to the I2C peripheral's configuration and state.

2. baud_rate (unsigned long):
 	- The desired baud rate for I2C communication, representing the frequency of the SCL line. Common values are 100 KHz or 400 KHz, but the function allows for other baud rates, including up to 1 MHz, depending on the hardware peripheral capabilities and the connected slave device requirements.

#### Working:
1. The function takes a pointer to an I2C instance and a baud rate as inputs.
2. It initializes the I2C hardware peripheral pointed to by i2c_instance with the specified baud_rate.
3. The initialization process ensures the I2C peripheral is properly configured and ready for further I2C operations.


### i2c_deinit function

The deinitilize function is responsible for de-initializing the hardware peripheral. De-initializing in this context means completely disabling the hardware peripheral. After deinitializing the hardware peripheral can only be initialized by using the i2c_init function or by manually enabling the hardware peripheral using it's handle. 

### i2c_set_baud_rate function

```c
void i2c_write_blocking(const i2c_periph_inst_t* i2c_instance, unsigned char addr, const unsigned char* write_buff, size_t size, i2c_stop_bit_t stop_bit);
```

#### Description:

The i2c_write_blocking function is used to execute a blocking write operation on an I2C bus. It sends a sequence of bytes to a specific I2C address. The function will block the execution until the write operation is complete.
Parameters:

1. i2c_instance (const i2c_periph_inst_t*):
	- A pointer to an instance of the I2C peripheral structure. This parameter contains the information related to the I2C peripheral's configuration and state.

2. addr (unsigned char):
	- The I2C address of the slave device to which the data will be sent.

3. write_buff (const unsigned char*):
	- A pointer to the buffer containing the data to be written to the slave device.

4. size (size_t):
	- The number of bytes to write from the write_buff to the slave device.

5. stop_bit (i2c_stop_bit_t):
	- An enumeration type to specify whether a STOP bit should be sent after the write operation is complete.

#### Return Type:

void:
	- This function does not return any value.

#### Working:

1. The function begins the write operation by sending the data from the write_buff to the slave device with address addr on the I2C bus defined by i2c_instance.
2. It will write size number of bytes from write_buff to the slave device.
3. The function will wait (block) until the complete data is written.
4. If stop_bit is set, it will send a STOP bit after completing the write operation to release the I2C bus.


