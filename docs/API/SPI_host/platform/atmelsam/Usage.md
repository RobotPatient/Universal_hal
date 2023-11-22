# Atmel SAMD21/51 SPI API usage

The general API for SPI communication on Atmel SAMD21/51 microcontrollers, while consistent across various models, involves platform-specific intricacies, particularly in clock system linkage. These specifics cannot be generalized through simple abstractions.

Consequently, the design approach was to decouple these details via a generic API, allowing for the configuration of essential parameters while accommodating platform-specific requirements.

## Platform specific settings

To correctly utilize the SPI peripheral, certain preparatory steps are essential. The following checklist should be completed:

- [ ] Set up the clocks for the SPI peripheral (this step can be skipped when using ASF or Arduino frameworks).
- [ ] Configure the pins for the SPI peripheral (refer to the GPIO section on this page).
- [ ] Utilize the spi_host_init function with appropriate configuration settings.
- [ ] Initialize the SPI hardware peripheral using the spi_host_init() function.

!!! note
    Click on the checkmarks above to track your progress.

### Clocks

This implementation does not configure clock generators but merely connects the SPI peripheral to pre-existing clock generators. These generators must be set up either by your integrating code or by the software framework you are using.

Frameworks like Arduino and ASF, which utilize the Atmel Start tool, typically handle most of the configuration. Both these frameworks default to using clock generator 0 set at 48 MHz (Arduino) or 8 MHz (ASF) and clock generator 1 set at 32.768 KHz.

!!! Warning
    The SPI peripheral requires two clocks for proper functioning: a slow one (< 100 KHz) primarily for internal timing and synchronization, and a fast one (≥ twice the frequency of SCK). The fast clock is used for operational purposes in host mode.

### GPIO pinmux settings

In addition to configuring the spi_host_init() function with the correct settings, the pins must be linked to the hardware peripheral using the SAMD's built-in pinmux. This is achieved with the gpio_set_pin_mode function:

```c
void gpio_set_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode);
```

??? question "How to set the right pin mode?"
    Refer to [Table 7-1 of the SAMD21 datasheet](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU32/ProductDocuments/DataSheets/SAM-D21-DA1-Family-Data-Sheet-DS40001882H.pdf) or [Table 6-1 of the SAMD51 datasheet](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU32/ProductDocuments/DataSheets/SAM-D5x-E5x-Family-Data-Sheet-DS60001507.pdf) to determine the correct `pin_mode`.

    For instance, configuring PA4 and PA5 of the SAMD21 for the SDA and SCL output of SERCOM 0 would require setting these pins to pin mode D. The function calls would be as follows:
    ```c
    const gpio_pin_t SDA_PIN = GPIO_PIN_PA4;
    const gpio_pin_t SCL_PIN = GPIO_PIN_PA5;

    gpio_set_pin_mode(SDA_PIN, GPIO_MODE_D);
    gpio_set_pin_mode(SCL_PIN, GPIO_MODE_D);
    ```

### spi_host_init function

The spi_host_init function is used to initialize the SPI hardware peripheral with the required settings. Compile-time validation of each setting is possible using the SPI_HOST_INIT() macro, which checks parameters before replacing the function call with spi_host_init.

```c
void spi_host_init(const spi_host_inst_t spi_peripheral_num, 
                   const spi_clock_sources_t clock_sources,
                   const uint32_t periph_clk_freq, 
                   const unsigned long spi_bus_frequency,
                   const spi_bus_opt_t extra_configuration_options);
```

The following parameters can be set within this function:

??? info "spi_peripheral_num"
    Specifies the SERCOM number to couple the module to.
    
    !!! Warning
        Be aware that these peripherals are shared with the UART, SPI, and I2C drivers. Ensure that this driver is the sole user of the selected peripheral to prevent overwriting any previous configurations. These peripherals can only be used for one communication interface type at a time.
    
    Example SAMD21 microcontroller configuration values:
    ```c
    typedef enum {
        SPI_PERIPHERAL_0,
        SPI_PERIPHERAL_1,
        ...
        SPI_PERIPHERAL_5
    } spi_host_inst_t;
    ```
    These values directly map to the SERCOM_x peripherals.
    
    Example usage:
    | Sercom     | spi_peripheral_num value |
    | ---------- | ---------------------- |
    | Sercom 0   | SPI_PERIPHERAL_0       |
    | Sercom ... | SPI_PERIPHERAL_...     |

??? info "clock_sources"
    Designates the clock sources for the SPI peripheral.
    
    Configuration options for the SAMD21 microcontrollers:
    ```c
    typedef enum {
        SPI_CLK_SOURCE_USE_DEFAULT = 0x00,
        SPI_CLK_SOURCE_FAST_CLKGEN0 = 0x01,
        ...
        SPI_CLK_SOURCE_SLOW_CLKGEN7 = 0x800,
    } spi_clock_sources_t;
    ```
    The SPI peripheral requires both a fast and a slow clock. The fast clock is used for operational purposes, while the slow one is for synchronization.

    To select both clocks, use an OR operation on the flags, such as:
    ```c
    #define SPI_PERIPHERAL_CLK_SOURCES (SPI_CLK_SOURCE_FAST_CLKGEN0 | SPI_CLK_SOURCE_SLOW_CLKGEN3)
    
    SPI_HOST_INIT(SPI_PERIPHERAL_0, SPI_PERIPHERAL_CLK_SOURCES, ...
    ```
    
    For default clock settings, use `SPI_CLK_SOURCE_USE_DEFAULT`, especially with ASF or Arduino frameworks.

??? info "periph_clk_freq"
    Specifies the output frequency of the selected fast frequency clock generator.
    
    Typical values:
    
    | Clock generator   | Platform | Framework | periph_clk_freq      |
    | ----------------- | -------- | --------- | -------------------- |
    | Clock generator 0 | SAMD51   | Arduino   | 120000000 (120 MHz)  |
    | Clock generator 0 | SAMD51   | ASF       | 8000000  (8 MHz)     |
    | Clock generator 0 | SAMD21   | Arduino   | 48000000 (48 MHz)    |
    | Clock generator 0 | SAMD21   | ASF       | 8000000 (8 MHz)      |
    
    With Arduino's default clock system, use `F_CPU` for this setting.

??? info "spi_bus_frequency"
    Sets the SPI bus frequency.
    
    The SAMD series supports SPI frequencies from 100 KHz to 24 MHz.
    
    !!! Warning
        The `periph_clk_freq` must be at least twice the desired SPI bus frequency.
    
    Example:
    ```c
    #define PERIPH_CLOCK_FREQ 48000000 /* Peripheral clock frequency of 48 MHz */
    #define SPI_CLOCK_FREQ 1000000 /* SPI bus frequency of 1 MHz */
    
    SPI_HOST_INIT(SPI_PERIPHERAL_0, SPI_CLK_SOURCE_USE_DEFAULT, PERIPH_CLOCK_FREQ, SPI_CLOCK_FREQ, ...
    ```

??? info "extra_configuration_options"
    The `extra_configuration_options` parameter in the SPI initialization function allows you to specify additional, platform-specific settings for the SPI bus. These options can be used to fine-tune the SPI peripheral's behavior to suit your application's needs. Here's a breakdown of the available options:

    - **SPI_BUS_OPT_USE_DEFAULT**:
        - Utilizes the default SPI settings. This option is suitable when the default configuration of the SPI bus aligns with your application's requirements.
        !!! Info "Default SPI settings"
            - DOPO settings: 1 -> MOSI: SERCOM_PAD[2] SCK: SERCOM_PAD[3] SS: SERCOM_PAD[1]
            - DIPO settings: 0 -> MISO: SERCOM_PAD[0]
            - MSB first
            - 8-bit character size
            - Clock polarity low in idle state

    - **SPI_BUS_OPT_CLOCK_POLARITY_SCK_HIGH (0x01)**:
        - Sets the idle state of the clock line (SCK) to high. The clock polarity option is crucial for ensuring proper synchronization between the SPI master and slave devices.

    - **SPI_BUS_OPT_DATA_ORDER_LSB_FIRST (0x02)**:
        - Configures the SPI bus to transmit data starting with the least significant bit first. By default, the SPI protocol transmits the most significant bit first.

    - **SPI_BUS_OPT_CHAR_SIZE_10_BIT (0x04)**:
        - Adjusts the character size for SPI communication to 10 bits. This setting is useful for applications that require data transfers larger than the standard 8-bit size.

    - **SPI_BUS_OPT_DOPO_PAD_0 (0x08)** to **SPI_BUS_OPT_DOPO_PAD_3 (0x20)**:
        - These options define the Data Out Pinout (DOPO) mapping for the SPI bus. They determine which physical pin (pad) on the microcontroller is used for the MOSI (Master Out Slave In) signal.

    - **SPI_BUS_OPT_DIPO_PAD_0 (0x40)** to **SPI_BUS_OPT_DIPO_PAD_3 (0x100)**:
        - Similar to DOPO, these options set the Data In Pinout (DIPO) for the SPI bus. They specify the physical pin (pad) used for the MISO (Master In Slave Out) signal.

    **The configuration of DOPO and DIPO is crucial for correctly routing SPI signals to the appropriate pins on the microcontroller, especially in designs where multiple peripherals share the same physical pins.**
    
    Combining these options allows for comprehensive customization of the SPI bus. For instance, to set a high clock polarity, LSB-first data order, and specific pad settings for MOSI and MISO, you would bitwise OR the respective options:

    ```c
    spi_bus_opt_t my_spi_config = SPI_BUS_OPT_CLOCK_POLARITY_SCK_HIGH | SPI_BUS_OPT_DATA_ORDER_LSB_FIRST | SPI_BUS_OPT_DOPO_PAD_0 | SPI_BUS_OPT_DIPO_PAD_1;
    ```

    This parameter ensures that the SPI configuration can be tailored to a wide variety of devices and use cases, offering the flexibility needed for complex embedded systems.

## Example configuration

!!! example "Adafruit Feather M0 (SAMD21)"
    Configuration for a SAMD21G18A (Adafruit Feather M0) using the Arduino framework with default clock settings (48 MHz).
    
    ```c
    #include <hal_gpio.h>
    #include <hal_spi_host.h>

    #ifndef F_CPU
    #define F_CPU 48000000 /* Peripheral clock speed of 48 MHz */
    #endif

    /* Set PA22 as MOSI and PA23 as SCK */
    #define MOSI_PIN GPIO_PIN_PA22
    #define SCK_PIN GPIO_PIN_PA23

    #define SPI_CLOCK_SPEED 1000000 /* SPI bus speed of 1 MHz */

    /* Use SERCOM3 as peripheral */
    #define SPI_PERIPHERAL SPI_PERIPHERAL_3

    /* Use default clock sources, e.g., Arduino framework clock system */
    #define SPI_CLOCK_SOURCE SPI_CLK_SOURCE_USE_DEFAULT

    /* Use the default configuration options */
    #define SPI_EXTRA_CONFIG_OPTIONS SPI_BUS_OPT_USE_DEFAULT

    void setup() {
       gpio_set_pin_mode(MOSI_PIN, GPIO_MODE_D);
       gpio_set_pin_mode(SCK_PIN, GPIO_MODE_D);
       SPI_HOST_INIT(SPI_PERIPHERAL, SPI_CLOCK_SOURCE, F_CPU, SPI_CLOCK_SPEED, SPI_EXTRA_CONFIG_OPTIONS);
    }
    ...
    ```
This layout provides a comprehensive guide to setting up the SPI driver on Atmel SAMD21/51 microcontrollers, including necessary preliminary steps, clock and GPIO configurations, and usage examples.# Atmel SAMD21/51 SPI API usage

The general API for SPI communication on Atmel SAMD21/51 microcontrollers, while consistent across various models, involves platform-specific intricacies, particularly in clock system linkage. These specifics cannot be generalized through simple abstractions.

Consequently, the design approach was to decouple these details via a generic API, allowing for the configuration of essential parameters while accommodating platform-specific requirements.

## Platform specific settings

To correctly utilize the SPI peripheral, certain preparatory steps are essential. The following checklist should be completed:

- [ ] Set up the clocks for the SPI peripheral (this step can be skipped when using ASF or Arduino frameworks).
- [ ] Configure the pins for the SPI peripheral (refer to the GPIO section on this page).
- [ ] Utilize the spi_host_init function with appropriate configuration settings.
- [ ] Initialize the SPI hardware peripheral using the spi_host_init() function.

!!! note
    Click on the checkmarks above to track your progress.

### Clocks

This implementation does not configure clock generators but merely connects the SPI peripheral to pre-existing clock generators. These generators must be set up either by your integrating code or by the software framework you are using.

Frameworks like Arduino and ASF, which utilize the Atmel Start tool, typically handle most of the configuration. Both these frameworks default to using clock generator 0 set at 48 MHz (Arduino) or 8 MHz (ASF) and clock generator 1 set at 32.768 KHz.

!!! Warning
    The SPI peripheral requires two clocks for proper functioning: a slow one (< 100 KHz) primarily for internal timing and synchronization, and a fast one (≥ twice the frequency of SCK). The fast clock is used for operational purposes in host mode.

### GPIO pinmux settings

In addition to configuring the spi_host_init() function with the correct settings, the pins must be linked to the hardware peripheral using the SAMD's built-in pinmux. This is achieved with the gpio_set_pin_mode function:

```c
void gpio_set_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode);
```

??? question "How to set the right pin mode?"
    Refer to [Table 7-1 of the SAMD21 datasheet](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU32/ProductDocuments/DataSheets/SAM-D21-DA1-Family-Data-Sheet-DS40001882H.pdf) or [Table 6-1 of the SAMD51 datasheet](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU32/ProductDocuments/DataSheets/SAM-D5x-E5x-Family-Data-Sheet-DS60001507.pdf) to determine the correct `pin_mode`.

    For instance, configuring PA4 and PA5 of the SAMD21 for the SDA and SCL output of SERCOM 0 would require setting these pins to pin mode D. The function calls would be as follows:
    ```c
    const gpio_pin_t SDA_PIN = GPIO_PIN_PA4;
    const gpio_pin_t SCL_PIN = GPIO_PIN_PA5;

    gpio_set_pin_mode(SDA_PIN, GPIO_MODE_D);
    gpio_set_pin_mode(SCL_PIN, GPIO_MODE_D);
    ```

### spi_host_init function

The spi_host_init function is used to initialize the SPI hardware peripheral with the required settings. Compile-time validation of each setting is possible using the SPI_HOST_INIT() macro, which checks parameters before replacing the function call with spi_host_init.

```c
void spi_host_init(const spi_host_inst_t spi_peripheral_num, 
                   const spi_clock_sources_t clock_sources,
                   const uint32_t periph_clk_freq, 
                   const unsigned long spi_bus_frequency,
                   const spi_bus_opt_t extra_configuration_options);
```

The following parameters can be set within this function:

??? info "spi_peripheral_num"
    Specifies the SERCOM number to couple the module to.
    
    !!! Warning
        Be aware that these peripherals are shared with the UART, SPI, and I2C drivers. Ensure that this driver is the sole user of the selected peripheral to prevent overwriting any previous configurations. These peripherals can only be used for one communication interface type at a time.
    
    Example SAMD21 microcontroller configuration values:
    
    ```c
    typedef enum {
        SPI_PERIPHERAL_0,
        SPI_PERIPHERAL_1,
        ...
        SPI_PERIPHERAL_5
    } spi_host_inst_t;
    ```
    These values directly map to the SERCOM_x peripherals.
    
    Example usage:
    
    | Sercom     | spi_peripheral_num value |
    | ---------- | ---------------------- |
    | Sercom 0   | SPI_PERIPHERAL_0       |
    | Sercom ... | SPI_PERIPHERAL_...     |

??? info "clock_sources"
    Designates the clock sources for the SPI peripheral.
    
    Configuration options for the SAMD21 microcontrollers:
    ```c
    typedef enum {
        SPI_CLK_SOURCE_USE_DEFAULT = 0x00,
        SPI_CLK_SOURCE_FAST_CLKGEN0 = 0x01,
        ...
        SPI_CLK_SOURCE_SLOW_CLKGEN7 = 0x800,
    } spi_clock_sources_t;
    ```
    The SPI peripheral requires both a fast and a slow clock. The fast clock is used for operational purposes, while the slow one is for synchronization.

    To select both clocks, use an OR operation on the flags, such as:
    ```c
    #define SPI_PERIPHERAL_CLK_SOURCES (SPI_CLK_SOURCE_FAST_CLKGEN0 | SPI_CLK_SOURCE_SLOW_CLKGEN3)
    
    SPI_HOST_INIT(SPI_PERIPHERAL_0, SPI_PERIPHERAL_CLK_SOURCES, ...
    ```
    
    For default clock settings, use `SPI_CLK_SOURCE_USE_DEFAULT`, especially with ASF or Arduino frameworks.

??? info "periph_clk_freq"
    Specifies the output frequency of the selected fast frequency clock generator.
    
    Typical values:
    
    | Clock generator   | Platform | Framework | periph_clk_freq      |
    | ----------------- | -------- | --------- | -------------------- |
    | Clock generator 0 | SAMD51   | Arduino   | 120000000 (120 MHz)  |
    | Clock generator 0 | SAMD51   | ASF       | 8000000  (8 MHz)     |
    | Clock generator 0 | SAMD21   | Arduino   | 48000000 (48 MHz)    |
    | Clock generator 0 | SAMD21   | ASF       | 8000000 (8 MHz)      |
    
    With Arduino's default clock system, use `F_CPU` for this setting.

??? info "spi_bus_frequency"
    Sets the SPI bus frequency.
    
    The SAMD series supports SPI frequencies from 100 KHz to 24 MHz.
    
    !!! Warning
        The `periph_clk_freq` must be at least twice the desired SPI bus frequency.
    
    Example:
    ```c
    #define PERIPH_CLOCK_FREQ 48000000 /* Peripheral clock frequency of 48 MHz */
    #define SPI_CLOCK_FREQ 1000000 /* SPI bus frequency of 1 MHz */
    
    SPI_HOST_INIT(SPI_PERIPHERAL_0, SPI_CLK_SOURCE_USE_DEFAULT, PERIPH_CLOCK_FREQ, SPI_CLOCK_FREQ, ...
    ```
    
??? info "extra_configuration_options"
    The `extra_configuration_options` parameter in the SPI initialization function allows you to specify additional, platform-specific settings for the SPI bus. These options can be used to fine-tune the SPI peripheral's behavior to suit your application's needs. Here's a breakdown of the available options:

    - **SPI_BUS_OPT_USE_DEFAULT**:
        - Utilizes the default SPI settings. This option is suitable when the default configuration of the SPI bus aligns with your application's requirements.
	!!! info "Default SPI settings"
	    - DOPO settings: 1 -> MOSI: SERCOM_PAD[2] SCK: SERCOM_PAD[3] SS: SERCOM_PAD[1]
	    - DIPO settings: 0 -> MISO: SERCOM_PAD[0]
	    - MSB first
	    - 8-bit character size
	    - Clock polarity low in idle state

    - **SPI_BUS_OPT_CLOCK_POLARITY_SCK_HIGH (0x01)**:
        - Sets the idle state of the clock line (SCK) to high. The clock polarity option is crucial for ensuring proper synchronization between the SPI master and slave devices.

    - **SPI_BUS_OPT_DATA_ORDER_LSB_FIRST (0x02)**:
        - Configures the SPI bus to transmit data starting with the least significant bit first. By default, the SPI protocol transmits the most significant bit first.

    - **SPI_BUS_OPT_CHAR_SIZE_10_BIT (0x04)**:
        - Adjusts the character size for SPI communication to 10 bits. This setting is useful for applications that require data transfers larger than the standard 8-bit size.

    - **SPI_BUS_OPT_DOPO_PAD_0 (0x08)** to **SPI_BUS_OPT_DOPO_PAD_3 (0x20)**:
        - These options define the Data Out Pinout (DOPO) mapping for the SPI bus. They determine which physical pin (pad) on the microcontroller is used for the MOSI (Master Out Slave In) signal.

    - **SPI_BUS_OPT_DIPO_PAD_0 (0x40)** to **SPI_BUS_OPT_DIPO_PAD_3 (0x100)**:
        - Similar to DOPO, these options set the Data In Pinout (DIPO) for the SPI bus. They specify the physical pin (pad) used for the MISO (Master In Slave Out) signal.

    **The configuration of DOPO and DIPO is crucial for correctly routing SPI signals to the appropriate pins on the microcontroller, especially in designs where multiple peripherals share the same physical pins.**
    
    Combining these options allows for comprehensive customization of the SPI bus. For instance, to set a high clock polarity, LSB-first data order, and specific pad settings for MOSI and MISO, you would bitwise OR the respective options:

    ```c
    spi_bus_opt_t my_spi_config = SPI_BUS_OPT_CLOCK_POLARITY_SCK_HIGH | SPI_BUS_OPT_DATA_ORDER_LSB_FIRST | SPI_BUS_OPT_DOPO_PAD_0 | SPI_BUS_OPT_DIPO_PAD_1;
    ```

    This parameter ensures that the SPI configuration can be tailored to a wide variety of use cases.

## Example configuration

!!! example "Adafruit Feather M0 (SAMD21)"
    Configuration for a SAMD21G18A (Adafruit Feather M0) using the Arduino framework with default clock settings (48 MHz).
    
    ```c
    #include <hal_gpio.h>
    #include <hal_spi_host.h>

    #ifndef F_CPU
    #define F_CPU 48000000 /* Peripheral clock speed of 48 MHz */
    #endif

    /* Set PA22 as MISO, PA24 as MOSI, PA25 as SCK, and PA23 as SS */
    #define MISO_PIN GPIO_PIN_PA22
    #define MOSI_PIN GPIO_PIN_PA24
    #define SCK_PIN GPIO_PIN_PA25
    #define SS_PIN GPIO_PIN_PA23

    #define SPI_CLOCK_SPEED 1000000 /* SPI bus speed of 1 MHz */

    /* Use SERCOM3 as peripheral */
    #define SPI_PERIPHERAL SPI_PERIPHERAL_3

    /* Use default clock sources, e.g., Arduino framework clock system */
    #define SPI_CLOCK_SOURCE SPI_CLK_SOURCE_USE_DEFAULT

    /* Use the default configuration options */
    #define SPI_EXTRA_CONFIG_OPTIONS SPI_BUS_OPT_USE_DEFAULT

    void setup() {
       GPIO_SET_PIN_MODE(MISO_PIN, GPIO_MODE_C)
       GPIO_SET_PIN_MODE(MOSI_PIN, GPIO_MODE_C);
       GPIO_SET_PIN_MODE(SCK_PIN, GPIO_MODE_C);
       GPIO_SET_PIN_MODE(SS_PIN, GPIO_MODE_C);
       SPI_HOST_INIT(SPI_PERIPHERAL, SPI_CLOCK_SOURCE, F_CPU, SPI_CLOCK_SPEED, SPI_EXTRA_CONFIG_OPTIONS);
    }
    ...
    ```
