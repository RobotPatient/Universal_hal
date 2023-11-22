# SPI Slave API

## API Functionality

The API for SPI slave functionality has the following functions available:

```c
/* SPI slave driver initialization function (without compile-time parameter checking) */
uhal_status_t spi_slave_init(const spi_host_inst_t spi_peripheral_num,
                             const uint32_t spi_clock_source,
                             const uint32_t spi_clock_source_freq,
                             const unsigned long spi_bus_frequency,
                             const spi_bus_opt_t spi_extra_configuration_opt);

/* SPI slave driver initialization function (with compile-time parameter checking) */
uhal_status_t SPI_SLAVE_INIT(const spi_host_inst_t spi_peripheral_num,
                             const uint32_t spi_clock_source,
                             const uint32_t spi_clock_source_freq,
                             const unsigned long spi_bus_frequency,
                             const spi_bus_opt_t spi_extra_configuration_opt);

/* SPI slave driver deinitialization function (without compile-time parameter checking) */
uhal_status_t spi_slave_deinit(const spi_host_inst_t spi_peripheral_num);

/* SPI slave driver deinitialization function (with compile-time parameter checking) */
uhal_status_t SPI_SLAVE_DEINIT(const spi_host_inst_t spi_peripheral_num);
```

## spi_slave_init function

```c
uhal_status_t spi_slave_init(const spi_slave_inst_t spi_peripheral_num, 
                             const uint32_t spi_clock_source,
                             const spi_bus_opt_t spi_extra_configuration_opt);
```

### Description:

The `spi_slave_init` function is designed for initializing an SPI peripheral in slave mode. It configures the peripheral with specific settings, including the clock source and additional options that dictate the behavior of the SPI bus.

### Error Checking:

- **SPI_SLAVE_INIT**: This function, written in uppercase, includes compile-time parameter checking. It verifies the correctness of the provided `spi_peripheral_num`, `spi_clock_source`, and `spi_extra_configuration_opt`, ensuring that they are valid and suitable for initializing an SPI slave peripheral. This level of verification is crucial for preventing configuration errors, particularly in scenarios where incorrect parameter values could lead to malfunction or improper operation of the SPI peripheral.

    - **Usage Note**: Use this function when there is certainty about the peripheral number, clock source, and configuration options at compile time. It provides an additional layer of safety by ensuring that the parameters used for initialization are correct and within acceptable ranges.

- **spi_slave_init**: This version performs the initialization of the SPI slave peripheral without compile-time parameter checks. It is more adaptable for situations where the peripheral number, clock source, or extra configuration options might be determined at runtime or are subject to change.

    - **Usage Note**: Opt for this function in dynamic systems or when dealing with parameters that are not known beforehand or might vary. This version allows for greater flexibility and is suitable for applications with changing or runtime-determined configurations.
    
### Parameters:

1. **spi_peripheral_num (const spi_slave_inst_t)**: 
   - Specifies the SPI peripheral instance to initialize as a slave. This parameter identifies the particular SPI peripheral that should operate in slave mode.

2. **spi_clock_source (const uint32_t)**: 
   - Determines the clock source for the SPI peripheral. This setting is critical as it affects the timing and synchronization of SPI communications.

3. **spi_extra_configuration_opt (const spi_bus_opt_t)**: 
   - Provides additional configuration options for the SPI bus. These options can include settings for clock polarity, phase, data order, character size, and pad-specific configurations necessary for the correct operation of the SPI slave peripheral.

### Return:

- **uhal_status_t**: This function returns a status code that indicates whether the initialization was successful. It provides feedback on the setup process, helping to identify any issues that might have occurred during the initialization.

### Working:

1. The initialization process begins with the assignment of the specified `spi_peripheral_num` to the desired SPI slave peripheral. This step ensures that the correct hardware unit is being configured.

2. The `spi_clock_source` is then configured, setting up the timing and synchronization aspect of the SPI communication based on the microcontroller's clock system.

3. Additional configurations, as specified by `spi_extra_configuration_opt`, are applied. These settings enable the customization of the SPI operation according to the specific requirements of the application, such as adjusting clock polarity, data order, and other bus-specific parameters.

4. The SPI peripheral is then enabled in slave mode, ready to participate in SPI communications governed by an external SPI master device. 



## spi_slave_deinit function

```c
/* SPI slave driver deinitialization function (without compile-time parameter checking) */
uhal_status_t spi_slave_deinit(const spi_host_inst_t spi_peripheral_num);

/* SPI slave driver deinitialization function (with compile-time parameter checking) */
uhal_status_t SPI_SLAVE_DEINIT(const spi_host_inst_t spi_peripheral_num);
```

### Description:

The `spi_slave_deinit` and `SPI_SLAVE_DEINIT` functions are used to deinitialize a previously initialized SPI peripheral operating in slave mode. These functions effectively disable the SPI slave operation and release the associated hardware resources.

### Error Checking:

- **SPI_SLAVE_DEINIT**: This function, written in uppercase, includes compile-time parameter checking. It ensures the validity of the provided `spi_peripheral_num`, verifying that it refers to a correctly initialized and existing SPI peripheral. This check is crucial for avoiding errors related to deinitializing a non-existent or already deinitialized peripheral.
  
	- **Usage Note**: This function should be used when there is certainty about the peripheral number at compile time, allowing for an additional layer of safety during the deinitialization process.

- **spi_slave_deinit**: This version performs the deinitialization without compile-time checks, offering greater flexibility for scenarios where the peripheral number may be determined at runtime or may change during the course of operation.
  
	- **Usage Note**: Ideal for dynamic systems where the peripheral to be deinitialized might not be known beforehand or might vary based on runtime conditions.

### Parameters:

- **spi_peripheral_num (const spi_host_inst_t)**: Specifies the SPI peripheral instance to deinitialize. This parameter identifies the specific SPI slave peripheral that should be disabled.

### Return:

- **uhal_status_t**: The function returns a status indicating the success or failure of the deinitialization process. A successful return value implies that the SPI slave peripheral has been properly disabled and its resources are now free for other uses or reinitialization.

### Working:

1. The function commences by validating the provided SPI peripheral number to ensure it corresponds to an active and previously initialized SPI slave peripheral.
2. It proceeds to disable the SPI peripheral, terminating any ongoing slave operations and ensuring the peripheral is no longer active in the SPI communication.
3. The hardware resources associated with the SPI slave peripheral are then released, making them available for other purposes or for reinitialization in the future.
4. Finally, the function returns a status to indicate whether the deinitialization was successful, providing feedback on the operation's outcome.

## SPI Slave IRQ Functionality

The SPI Slave driver within the Universal HAL provides several Interrupt Request (IRQ) handlers specifically tailored for SPI interactions. These IRQ handlers are invoked in response to different events occurring on the SPI bus. By default, they are declared as weak symbols, meaning you can override them with custom implementations in your source code outside the Universal HAL.

### 1. Chip Select IRQ

Triggered when the SPI chip select pin is pulled low, indicating the start of a communication session with the SPI slave device.

```c
void spi_slave_chip_select_irq(const void *hw, volatile bustransaction_t *transaction) __attribute__((weak));
```

### 2. Data Receive IRQ

Executed when a byte of data is received from the SPI master. This handler is crucial for processing incoming data efficiently and effectively.

```c
void spi_slave_data_recv_irq(const void *hw, volatile bustransaction_t *transaction) __attribute__((weak));
```

### 3. Data Send IRQ

Called when the SPI master requests a byte of data from the SPI slave. This handler is responsible for sending data back to the master.

```c
void spi_slave_data_send_irq(const void *hw, volatile bustransaction_t *transaction) __attribute__((weak));
```

### Use-Case Example:

Consider a scenario where you're developing a system with an SPI slave device (like a sensor) that interacts with an SPI master device (like a microcontroller).

1. **Chip Select Activation**: The master initiates communication by pulling the chip select line low, signaling the start of a transaction. This event triggers the `spi_slave_chip_select_irq` in the slave device.

2. **Data Request**: The master may then request data, leading to the invocation of the `spi_slave_data_send_irq` on the slave side. This handler is responsible for preparing and sending the requested data back to the master.

3. **Data Reception**: As the master sends data to the slave, the `spi_slave_data_recv_irq` is triggered on the slave side to process the received data.

By providing custom implementations for these IRQ handlers, you can define specific actions for the slave device to perform in response to each of these events, ensuring efficient and tailored communication between the master and slave devices.

