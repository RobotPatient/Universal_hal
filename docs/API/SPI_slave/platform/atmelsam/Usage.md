# Atmel SAMD21/51 SPI Slave API Usage

The SPI Slave API for Atmel SAMD21/51 microcontrollers is designed with platform-specific nuances, particularly in clock system linkage and peripheral management. To address these intricacies, the API employs a generic approach, enabling the configuration of key parameters while accommodating specific platform requirements.

## Platform Specific Settings

For effective use of the SPI slave peripheral, follow these essential steps:

- [ ] Configure the clocks for the SPI peripheral (skip if using ASF or Arduino).
- [ ] Set up the GPIO pins for the SPI peripheral (details in the GPIO section).
- [ ] Use the `spi_slave_init` function with the right configuration settings.

!!! note
    You can click on the checkmarks above to mark your progress.

### Clocks

The SPI implementation does not set up clock generators; it connects the SPI peripheral to existing clock generators. This setup is the responsibility of the integrating code or software framework.

Frameworks like Arduino and ASF typically handle most configurations. Both set clock generator 0 at 48 MHz (Arduino) or 8 MHz (ASF) and clock generator 1 at 32.768 KHz.

!!! Warning
    The SPI peripheral needs two clocks: a slow one (< 100 KHz) for internal timing and synchronization, and a fast one (≥ 2x SCK frequency) for operational purposes in slave mode.

### GPIO Pinmux Settings

Apart from configuring the `spi_slave_init()` function, you must link the GPIO pins to the hardware peripheral using the SAMD's pinmux with `gpio_set_pin_mode`.

```c
void gpio_set_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode);
```

??? question "Setting the right pin mode"
    Consult the SAMD21 or SAMD51 datasheet for the correct `pin_mode`. For example, to configure PA4 and PA5 of the SAMD21 for SERCOM 0's SDA and SCL:
    ```c
    const gpio_pin_t SDA_PIN = GPIO_PIN_PA4;
    const gpio_pin_t SCL_PIN = GPIO_PIN_PA5;

    gpio_set_pin_mode(SDA_PIN, GPIO_MODE_D);
    gpio_set_pin_mode(SCL_PIN, GPIO_MODE_D);
    ```

### spi_slave_init Function

Use `spi_slave_init` to initialize the SPI hardware peripheral. Compile-time parameter validation is possible with `SPI_SLAVE_INIT`.

```c
void spi_slave_init(const spi_host_inst_t spi_peripheral_num, 
                    const uint32_t spi_clock_source,
                    const uint32_t spi_clock_source_freq, 
                    const unsigned long spi_bus_frequency,
                    const spi_bus_opt_t spi_extra_configuration_opt);
```

Set the following parameters:

??? info "spi_peripheral_num"
    Specifies the SERCOM number for the SPI module.

    !!! Warning
        These peripherals are shared with UART, SPI, and I2C. Ensure exclusive use to avoid configuration conflicts.
    
    Configuration for SAMD21:
    
    ```c
    typedef enum {
        SPI_PERIPHERAL_0,
        ...
        SPI_PERIPHERAL_5
    } spi_host_inst_t;
    ```

??? info "clock_sources"
    Identifies the clock sources for the SPI peripheral.

    SAMD21 options:
    ```c
    typedef enum {
        SPI_CLK_SOURCE_USE_DEFAULT = 0x00,
        ...
        SPI_CLK_SOURCE_SLOW_CLKGEN7 = 0x800,
    } spi_clock_sources_t;
    ```
    Combine a fast and a slow clock using an OR operation.

??? info "periph_clk_freq"
    The output frequency of the fast clock generator.

    Typical values:
    
    | Clock generator | Platform | Framework | Frequency |
    | --------------- | -------- | --------- | --------- |
    | Clock gen 0     | SAMD51   | Arduino   | 120 MHz   |
    | Clock gen 0     | SAMD21   | Arduino   | 48 MHz    |
    | Clock gen 0     | SAMD21   | ASF       | 8 MHz     |

    Use `F_CPU` with Arduino.

??? info "spi_bus_frequency"
    The SPI bus frequency.

    Supported frequencies for SAMD:
    ```c
    #define PERIPH_CLOCK_FREQ 48000000
    #define SPI_CLOCK_FREQ 1000000
    
    SPI_SLAVE_INIT(SPI_PERIPHERAL_0, SPI_CLK_SOURCE_USE_DEFAULT, PERIPH_CLOCK_FREQ, SPI_CLOCK_FREQ, ...
    ```

??? info "extra_configuration_options"
    Extra configuration options for the SPI peripheral.

    Available options:
    
    - **SPI_BUS_OPT_USE_DEFAULT**: Default SPI settings.
    - **SPI_BUS_OPT_CLOCK_POLARITY_SCK_HIGH (0x01)**: Clock line high in idle state.
    - **SPI_BUS_OPT_DATA_ORDER_LSB_FIRST (0x02)**: Least significant bit first.
    - **SPI_BUS_OPT_CHAR_SIZE_10_BIT (0x04)**: 10-bit character size.
    - **SPI_BUS_OPT_DOPO_PAD_0 to PAD_3**: Defines MOSI pin mapping.
    - **SPI_BUS_OPT_DIPO_PAD_0 to PAD_3**: Defines MISO pin mapping.

    Combine options with OR:
    ```c
    spi_bus_opt_t config = SPI_BUS_OPT_CLOCK_POLARITY_SCK_HIGH | SPI_BUS_OPT_DATA_ORDER_LSB_FIRST;
    ```

## Example Configuration

!!! example "Adafruit Feather M0 (SAMD21) SPI Slave"
    ```c
    #include <hal_gpio.h>
    #include <hal_spi_slave.h>

    #ifndef F_CPU
    #define F_CPU 48000000
    #endif

    /* Define GPIO pins */
    #define MISO_PIN GPIO_PIN_PA22
    #define MOSI_PIN GPIO_PIN_PA24
    #define SCK_PIN GPIO_PIN_PA25
    #define SS_PIN GPIO_PIN_PA23

    /* SPI settings */
    #define SPI_CLOCK_SPEED 1000000
    #define SPI_PERIPHERAL SPI_PERIPHERAL_3
    #define SPI_CLOCK_SOURCE SPI_CLK_SOURCE_USE_DEFAULT
    #define SPI_CONFIG_OPTIONS SPI_BUS_OPT_USE_DEFAULT

    void setup() {
       gpio_set_pin_mode(MISO_PIN, GPIO_MODE_C);
       gpio_set_pin_mode(MOSI_PIN, GPIO_MODE_C);
       gpio_set_pin_mode(SCK_PIN, GPIO_MODE_C);
       gpio_set_pin_mode(SS_PIN, GPIO_MODE_C);
       SPI_SLAVE_INIT(SPI_PERIPHERAL, SPI_CLOCK_SOURCE, F_CPU, SPI_CLOCK_SPEED, SPI_CONFIG_OPTIONS);
    }
    ...
    ```

