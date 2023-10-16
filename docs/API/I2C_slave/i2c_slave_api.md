# I2C Slave API
The I2C slave driver provides a simple API that lets you set up and manage hardware with I2C slave features. It uses an interrupt-driven approach (unless not supported by the MCU). This means it reacts to specific I2C events instead of constantly checking for them. This method was chosen because more complex setups might limit how you can use the driver. Now you have more freedom to handle different I2C situations and can easily create your own data handling methods.


## API functionality
The API for I2C slave functionality has the following functions available:

```c

/* I2C slave initialization function (without compile-time parameter checking) */
uhal_status_t i2c_slave_init(const i2c_periph_inst_t i2c_peripheral_num, 
                             const uint16_t slave_addr, 
                             const i2c_clock_sources_t clock_sources,
                             const i2c_extra_opt_t extra_configuration_options);

/* I2C slave initialization function (with compile-time parameter checking)   */
uhal_status_t I2C_SLAVE_INIT(const i2c_periph_inst_t i2c_peripheral_num, 
                             const uint16_t slave_addr, 
                             const i2c_clock_sources_t clock_sources,
                             const i2c_extra_opt_t extra_configuration_options);

/* I2C slave de-initialization function (without compile-time parameter checking) */
uhal_status_t i2c_slave_deinit(const i2c_periph_inst_t i2c_peripheral_num);

/* I2C slave de-initialization function (with compile-time parameter checking)   */
uhal_status_t I2C_SLAVE_DEINIT(const i2c_periph_inst_t i2c_peripheral_num);

```

## i2c_slave_init function

```c
/* I2C slave initialization function (without compile-time parameter checking) */
uhal_status_t i2c_slave_init(const i2c_periph_inst_t i2c_peripheral_num, 
                             const uint16_t slave_addr, 
                             const i2c_clock_sources_t clock_sources,
                             const i2c_extra_opt_t extra_configuration_options);

/* I2C slave initialization function (with compile-time parameter checking)   */
uhal_status_t I2C_SLAVE_INIT(const i2c_periph_inst_t i2c_peripheral_num, 
                             const uint16_t slave_addr, 
                             const i2c_clock_sources_t clock_sources,
                             const i2c_extra_opt_t extra_configuration_options);
```

### Description:
The `i2c_slave_init` and `I2C_SLAVE_INIT` functions initialize an I2C peripheral to operate in slave mode with the specified address, clock source, and additional configuration options.

### Error Checking:
- **I2C_SLAVE_INIT**: This uppercase version includes compile-time parameter checking.
    - **Usage Note**: Use this when the peripheral number and other parameters are known at compile time.
  
- **i2c_slave_init**: This function does the initialization without compile-time checks.
    - **Usage Note**: Use this when the peripheral number and other parameters might be determined at runtime.

### Parameters:
1. **i2c_peripheral_num (const i2c_periph_inst_t)**: The specific I2C peripheral instance to initialize as a slave.
2. **slave_addr (const uint16_t)**: The slave address for the I2C peripheral.
3. **clock_sources (const i2c_clock_sources_t)**: The source of the clock to be used by the I2C peripheral.
4. **extra_configuration_options (const i2c_extra_opt_t)**: Additional configuration options for the I2C peripheral.

### Return:
- **uhal_status_t**: Success or failure status of the initialization.

### Working:
1. Validates the parameters and ensures the I2C peripheral is not in use.
2. Sets the peripheral to slave mode.
3. Configures the slave address, clock source, and applies extra configuration options.
4. Enables the I2C peripheral in slave mode with the specified settings.

## i2c_slave_deinit function

```c
/* I2C slave de-initialization function (without compile-time parameter checking) */
uhal_status_t i2c_slave_deinit(const i2c_periph_inst_t i2c_peripheral_num);

/* I2C slave de-initialization function (with compile-time parameter checking) */
uhal_status_t I2C_SLAVE_DEINIT(const i2c_periph_inst_t i2c_peripheral_num);
```

### Description:
The `i2c_slave_deinit` and `I2C_SLAVE_DEINIT` functions deinitialize an I2C peripheral that was previously set up in slave mode. This process releases the resources associated with the I2C peripheral, ensuring that it won't interfere with other processes.

### Error Checking:
- **I2C_SLAVE_DEINIT**: This uppercase version includes compile-time parameter checking.
    - **Usage Note**: Use this function when the peripheral number is known at compile time.
  
- **i2c_slave_deinit**: This function does the de-initialization without compile-time checks.
    - **Usage Note**: Use this function when the peripheral number might be determined at runtime.

### Parameters:
1. **i2c_peripheral_num (const i2c_periph_inst_t)**: The specific I2C peripheral instance to deinitialize.

### Return:
- **uhal_status_t**: Success or failure status of the de-initialization.

### Working:
1. Validates the given peripheral number.
2. Ensures the I2C peripheral is not currently involved in any ongoing communications.
3. Disables the I2C peripheral and releases the resources associated with it.
4. Resets the I2C slave configuration to its default state.


## IRQ functionality

The driver exposes four Interrupt Request (IRQ) handlers related to I2C interactions. When certain conditions or events occur on the I2C bus, these IRQ handlers are triggered. By default, these handlers are declared as weak symbols, allowing users to provide custom implementations by defining these functions in a source file outside the Universal HALL.

### 1. Address Match IRQ

Triggered when a start condition with a valid slave address is detected.

```c
void i2c_slave_address_match_irq(const void* hw, volatile bustransaction_t* transaction) __attribute__((weak));
```

### 2. Stop Condition IRQ

Invoked when a stop condition is detected on the I2C bus.

```c
void i2c_slave_stop_irq(const void* hw, volatile bustransaction_t* transaction) __attribute__((weak));
```

### 3. Data Receive IRQ

Executed when a byte of data is received from the host.

```c
void i2c_slave_data_recv_irq(const void* hw, volatile bustransaction_t* transaction) __attribute__((weak));
```

### 4. Data Send IRQ

Called when the host requests a byte of data.

```c
void i2c_slave_data_send_irq(const void* hw, volatile bustransaction_t* transaction) __attribute__((weak));
```


## Use-Case Example:

Imagine you're developing a system where an I2C slave device (e.g., a sensor) needs to send data to a master device (e.g., a microcontroller) upon request.

1. The master initiates communication with a start condition and the slave address. This action triggers the `i2c_slave_address_match_irq`.
2. Once the slave acknowledges, the master may request data, activating the `i2c_slave_data_send_irq` on the slave side.
3. As the slave sends bytes of data, the `i2c_slave_data_recv_irq` might get triggered on the master side to handle incoming data.
4. After the communication ends, the master issues a stop condition, which invokes the `i2c_slave_stop_irq` on the slave side.

By defining custom implementations for these IRQ handlers, developers can specify how the slave device should respond to each of these events.
