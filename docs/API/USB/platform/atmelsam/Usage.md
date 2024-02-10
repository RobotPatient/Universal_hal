# Atmel SAMD21/51 USB API Usage

The USB API for Atmel SAMD21/51 microcontrollers is designed with platform-specific nuances, particularly in clock system linkage, peripheral management and usb specific settings such as device descriptors. To address these intricacies, the API employs a generic approach, enabling the configuration of key parameters while accommodating specific platform requirements.

## Platform Specific Settings

For effective use of the USB peripheral, follow these essential steps:

- [ ] Include the device descriptors (`usb_descriptors.c`) and `tusb_config.h` in the build (link them to target `Universal_hal_usb_conf`)
- [ ] Set up the GPIO pins for the USB peripheral (details in the GPIO section, USB_DM and USB_DP pin).
- [ ] Use the `usb_serial_init` function with the right configuration settings.

!!! note
    You can click on the checkmarks above to mark your progress.

### Clocks

The USB implementation does not set up clock generators; it connects the USB peripheral to existing clock generators. This setup is the responsibility of the integrating code or software framework.

Frameworks like Arduino and ASF typically handle most configurations. Both set clock generator 0 at 48 MHz (Arduino) or 8 MHz (ASF) and clock generator 1 at 32.768 KHz.

!!! Warning
    The USB peripheral needs one clock: a fast one (≥ 8MHz) to be operational.


### Build system configuration
To use the USB serial module some build system settings need to be set. By default the Universal_hal exposes the `Universal_hal_usb_conf` build_target in CMake. This target has to be linked with the `device_descriptors.c` and usb configuration file `tusb_config.h`. Besides this the USB serial module has to be manually enabled, by setting the option `UHAL_ENABLE_USB_SERIAL_MODULE`, do this before importing the Universal hal! As the check inside the Universal_hal CMakeLists may miss this statement inside the check!

 This is easily done by these lines of CMake, this example assumes that the `usb_descriptors.c` and `tusb_config.h` are in the `src/` folder:
```cmake
...
set(UHAL_ENABLE_USB_SERIAL_MODULE ON)
...
# Include the Universal_hal here
...
target_sources(Universal_hal_usb_conf INTERFACE "src/usb_descriptors.c")
target_include_directories(Universal_hal_usb_conf INTERFACE src/)
```
If not sure you can always look at the CMakeLists of the usb_serial example.

### GPIO Pinmux Settings

Apart from configuring the `usb_serial_init()` function, you must link the GPIO pins to the hardware peripheral using the SAMD's pinmux with `gpio_set_pin_mode` and make sure the pin has no pull(-ups or -downs) enabled.

```c
void gpio_set_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode);
```

??? question "Setting the right pin mode"
    Consult the SAMD21 or SAMD51 datasheet for the correct `pin_mode`. For example, to configure PA4 and PA5 of the SAMD21 for SERCOM 0's RX and TX:
    ```c
    const gpio_pin_t USB_DM_PIN = GPIO_PIN_PA24;
    const gpio_pin_t USB_DP_PIN = GPIO_PIN_PA25;

    GPIO_SET_PIN_MODE(USB_DM_PIN, GPIO_MODE_OUTPUT);
    GPIO_SET_PIN_MODE(USB_DP_PIN, GPIO_MODE_OUTPUT);
    GPIO_SET_PIN_LVL(USB_DM_PIN, 0);
    GPIO_SET_PIN_LVL(USB_DP_PIN, 0);
    gpio_set_pin_mode(USB_DM_PIN, GPIO_MODE_G);
    gpio_set_pin_mode(USB_DP_PIN, GPIO_MODE_G);
    ```

### FreeRTOS compatibility
The library used (tinyusb) has FreeRTOS compatibility, enabling this offers better functionality and thread safety for use within a RTOS. It is highly suggested to enable this option when using a RTOS. 

Do this in `tusb_config.h`:
```c
#define CFG_TUSB_OS           OPT_OS_FREERTOS
```


### usb_serial_init Function

Use `usb_serial_init` to initialize the USB hardware peripheral with USB Serial CDC class functionality.

```c
uhal_status_t usb_serial_init(const usb_serial_inst_t serial_instance, 
                              const usb_clock_sources_t clock_source, 
                              const uint32_t clock_frequency);
```

Set the following parameters:

??? info "serial_instance"
    Specifies the USB serial bus to instantiate, the amount of busses available depends on the settings configured in `tusb_config.h`.
    The macro `CFG_TUD_CDC` specifies the amount of busses available.
    
    Configuration for SAMD21:
    
    ```c
    typedef enum {
        USB_SERIAL_0,
        ...
    } usb_serial_inst_t;
    ```
    If not sure what value to set? Set the default serial bus; USB_SERIAL0

??? info "clock_source"
    Identifies the clock source for the USB peripheral.

    SAMD21 options:
    ```c
    typedef enum {
        USB_CLK_SOURCE_USE_DEFAULT = 0x00,
        ...
    } usb_clock_sources_t;
    ```

??? info "clock_freq"
    The output frequency of the main cpu clock generator.

    Typical values:
    
    | Clock generator | Platform | Framework | Frequency |
    | --------------- | -------- | --------- | --------- |
    | Clock gen 0     | SAMD51   | Arduino   | 120 MHz   |
    | Clock gen 0     | SAMD21   | Arduino   | 48 MHz    |
    | Clock gen 0     | SAMD21   | ASF       | 8 MHz     |

    Use `F_CPU` with Arduino.

## Example Configuration

!!! example "Adafruit Feather M0 (SAMD21) USB Setup"
    ```c
    #include <hal_gpio.h>
    #include <hal_usb_serial.h>

    #ifndef F_CPU
    #define F_CPU 48000000
    #endif

    /* Define GPIO pins */
    #define USB_DM_PIN GPIO_PIN_PA24
    #define USB_DP_PIN GPIO_PIN_PA25

    /* USB settings */
    #define USB_SERIAL_PERIPHERAL USB_SERIAL_0
    #define USB_CLOCK_SOURCE UART_CLK_SOURCE_USE_DEFAULT

    void setup() {
       GPIO_SET_PIN_MODE(USB_DM_PIN, GPIO_MODE_OUTPUT);
       GPIO_SET_PIN_MODE(USB_DP_PIN, GPIO_MODE_OUTPUT);
       GPIO_SET_PIN_LVL(USB_DM_PIN, 0);
       GPIO_SET_PIN_LVL(USB_DP_PIN, 0);
       GPIO_SET_PIN_MODE(USB_DM_PIN, GPIO_MODE_G);
       GPIO_SET_PIN_MODE(USB_DP_PIN, GPIO_MODE_G);
       usb_serial_init(USB_SERIAL_PERIPHERAL, USB_CLOCK_SOURCE, F_CPU);
    }
    ...
    ```

For example usb_descriptors.c and tusb_config.h see the usb_serial example.