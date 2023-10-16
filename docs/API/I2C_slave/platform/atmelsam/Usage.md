# Atmel SAMD21/51 API usage

Although the general API is the same for each microcontroller. The underlying API specifics like the linking to a clock system is very platform specific. This can not be abstracted away using simple abstractions. 

Therefore the choice was made to decouple this using a generic api which provides a way to set the most critical parameters. 

## Platform specific settings

To be able to use the peripheral correctly, some pins, clocks and a struct have to be set beforehand. This is the checklist for this module:

- [ ] Setup the clocks for the peripheral (skip this step when using ASF or arduino).
- [ ] Setup the pins for the peripheral (see the gpio section on this page).
- [ ] Use the init function with the correct configuration settings
- [ ] Initialize the i2c hardware peripheral using the i2c_slave_init() function.
  
!!! note
    You can click on the checkmarks above to check them off your list ;)

### Clocks

Clockgenerators are not set by this implementation. It only hooks the peripheral up to existing clockgenerators. These generators have to be set-up by your own integrating code or software framework. 

Some frameworks like Arduino and ASF using the Atmel Start tool will configure most of the stuff under the hood. It is almost pure coincidence that both frameworks use the same default clockgenerators for the main clock and peripheral clocking.

Both frameworks have clockgenerator 0 set-up with a frequency of 48 (Arduino) or 8 MHz (ASF). And clockgenerator 3 set-up with a frequency of 32.768KHz. 

!!! Warning
    The Sercom needs two clocks to function, a slow one (< 100 KHz) and a fast one (>= $2 \cdot f_{SCL}$ ). The fast clock is used for operation in host-mode, the slow one is used for used for internal timing and synchronisation.

### GPIO pinmux settings

Besides setting the i2c_inst_t struct the pins have to be linked to the hardware peripheral using the SAMD's built-in pinmux. This can be done with the gpio_set_pin_mode function:

```c
void gpio_set_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode);
```

??? question "How to set the right pin mode?"
    For the exact pin_mode look at [Table 7-1 of the SAMD21 datasheet](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU32/ProductDocuments/DataSheets/SAM-D21-DA1-Family-Data-Sheet-DS40001882H.pdf) or [Table 6-1 of  the SAMD51 datasheet](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU32/ProductDocuments/DataSheets/SAM-D5x-E5x-Family-Data-Sheet-DS60001507.pdf).

     And **determine which column letter(s) corresponds to the right output(s)**.
    
     SERCOMx/PAD[0] corresponds to the SDA in/output of the SERCOM.
    
     SERCOMx/PAD[1] corresponds to the SCL output of the SERCOM. 
    
     For example configuring PA4 and PA5 of the SAMD21 to the SDA and SCL output of SERCOM 0: ![samd io table](../../../../assets/samd_io_table.png) Means that we have to set pin PA4 and PA5 to pin mode D (the column letter). The resulting function call will be:
     ```c
     {
     const gpio_pin_t SDA_PIN = GPIO_PIN_PA4;
     const gpio_pin_t SCL_PIN = GPIO_PIN_PA5;
    
     gpio_set_pin_mode(SDA_PIN, GPIO_MODE_D);
     gpio_set_pin_mode(SCL_PIN, GPIO_MODE_D);
     ...
     ```

### i2c_slave_init function

The i2c slave init function should be used to initialize the hardware peripheral with the right settings. Each setting can be statically validated using the I2C_SLAVE_INIT() macro. This macro will first validate in compile time whether valid settings have been entered. If no compile errors are generated the function call will be replaced with the i2c_host_init function.

```c
void i2c_host_init(const i2c_periph_inst_t i2c_peripheral_num, 
                   const uint16_t slave_addr,
                   const i2c_clock_sources_t clock_sources,
                   const i2c_extra_opt_t extra_configuration_options);
```

Within this function the following parameters can be set:
??? info  "i2c_peripheral_num"
    The sercom number to couple the module to. 
	!!! Warning
		When using these peripherals keep in mind that these peripherals are shared with the UART (master & slave), SPI (master & slave) and I2C host driver.
		Make sure that this driver is the only driver using this peripheral. If it is not the case, this driver will overwrite all previous peripheral configurations. 
		Since the peripheral can only be used for one communication interface type at a time.
	These are possible configuration values for the SAMD21 series of microcontrollers:

	```c
	typedef enum {
  		I2C_PERIPHERAL_0,
  		I2C_PERIPHERAL_1,
 		I2C_PERIPHERAL_2,
  		I2C_PERIPHERAL_3,
  		I2C_PERIPHERAL_4,
  		I2C_PERIPHERAL_5
	} i2c_periph_inst_t;
	```
	These values map directly to the SERCOM_x peripherals...
	
    Example: 
    
    | Sercom     | i2c_peripheral_num value |
    | ---------- | --------------------- |
    | Sercom 0   | I2C_PERIPHERAL_0      |
    | Sercom ... | I2C_PERIPHERAL_...    |

??? info "slave_addr"
    This is the slave address to use for this i2c slave driver instance.
    
    This can be set to any 8-bit value. But the values 0 and 0xFF are not recommended as they will be ignored and/or cause fault conditions to occur.

??? info "clock_sources"
    Indicates which clock sources are used for the hardware peripheral clock.
	
	These are possible configuration values for the SAMD21 series of microcontrollers:
	
	```c
	typedef enum {
		I2C_CLK_SOURCE_USE_DEFAULT = 0x00,
  		I2C_CLK_SOURCE_FAST_CLKGEN0 = 0x01,
  		I2C_CLK_SOURCE_FAST_CLKGEN1 = 0x02,
  		I2C_CLK_SOURCE_FAST_CLKGEN2 = 0x03,
  		I2C_CLK_SOURCE_FAST_CLKGEN3 = 0x04,
  		I2C_CLK_SOURCE_FAST_CLKGEN4 = 0x05,
  		I2C_CLK_SOURCE_FAST_CLKGEN5 = 0x06,
  		I2C_CLK_SOURCE_FAST_CLKGEN6 = 0x07,
  		I2C_CLK_SOURCE_FAST_CLKGEN7 = 0x08,
  		I2C_CLK_SOURCE_SLOW_CLKGEN0 = 0x100,
  		I2C_CLK_SOURCE_SLOW_CLKGEN1 = 0x200,
 		I2C_CLK_SOURCE_SLOW_CLKGEN2 = 0x300,
 		I2C_CLK_SOURCE_SLOW_CLKGEN3 = 0x400,
 		I2C_CLK_SOURCE_SLOW_CLKGEN4 = 0x500,
 		I2C_CLK_SOURCE_SLOW_CLKGEN5 = 0x600,
 		I2C_CLK_SOURCE_SLOW_CLKGEN6 = 0x700,
 		I2C_CLK_SOURCE_SLOW_CLKGEN7 = 0x800,
	} i2c_clock_sources_t;
	```
	The SERCOM peripherals in the SAMD series of microcontrollers need two clocks to function. 
	A fast and a slow clock. The fast one is used for baud-rate generation among things, and the slow one is primarly used for synchronisation.
	
	To select both the fast and slow clockgenerator, perform an OR operation on these flags like this:
	```c
    #define I2C_SLAVE_ADDR 0x28
	#define I2C_PERIPHERAL_CLK_SOURCES (I2C_CLK_SOURCE_FAST_CLKGEN0 | I2C_CLK_SOURCE_SLOW_CLKGEN3)
	
	/* Initialize peripheral with static parameter checking (during compile-time) */
	I2C_SLAVE_INIT(I2C_PERIPHERAL_0, I2C_SLAVE_ADDR, I2C_PERIPHERAL_CLK_SOURCES, ...
	
	or
	
	/* Initialize peripheral without parameter checking */ 
	i2c_slave_init(I2C_PERIPHERAL_0, I2C_SLAVE_ADDR, I2C_PERIPHERAL_CLK_SOURCES, ...
	```
	
	**If not sure which clockgenerator to use. Try the `I2C_CLK_SOURCE_USE_DEFAULT` flag.**
	
	This should work out of the box on ASF and Arduino.

??? info "extra_configuration_options"
	Platform dependent extra configuration options like: internal irq_priority, DMA use, 4-wire mode, etc. 
	
	The SAMD21 series supports the following extra configuration options:
	
	```c
	typedef enum {
  		I2C_EXTRA_OPT_NONE = 0,
  		I2C_EXTRA_OPT_4_WIRE_MODE = 1,
  		I2C_EXTRA_OPT_IRQ_PRIO_0 = 0x100,
  		I2C_EXTRA_OPT_IRQ_PRIO_1 = 0x200,
  		I2C_EXTRA_OPT_IRQ_PRIO_2 = 0x300,
  		I2C_EXTRA_OPT_IRQ_PRIO_3 = 0x400
	} i2c_extra_opt_t;
	```
	Setting one of these flags will do the following things:
	
	`I2C_EXTRA_OPT_NONE` flag will use default SERCOMx_handler irq priority of 2 and enable 2-WIRE mode (SCL+SDA line)

	`I2C_EXTRA_OPT_4_WIRE_MODE` flag will enable 4-wire mode
	
	`I2C_EXTRA_OPT_IRQ_PRIO_X` flag will overide the default SERCOMx_handler priority of 2 with priority of X. 
	
	Multiple flags can be selected in the same manner as with the clock_sources parameter (OR-ing them together).
	**However please note that only one IRQ_PRIO can be selected.**
	
	If not sure what to set.. Use the `I2C_EXTRA_OPT_NONE` flag. 
	
	This will use the most commonly used settings.
## Example configuration

!!! example "Adafruit Feather m0"
    This is an example configuration for a samd21g18a (adafruit feather m0)
    On the Arduino framework with the default clock settings (48 MHz).
    ```c
	#include <hal_gpio.h>
	#include <hal_i2c_host.h>

	/* Set PA22 as SDA and PA23 as SCL */
	#define SDA_PIN GPIO_PIN_PA22
	#define SCL_PIN GPIO_PIN_PA23

    /* Use slave address 0x28 */
    #define I2C_SLAVE_ADDR 0x28

    /* Use SERCOM3 as peripheral */
    #define I2C_PERIPHERAL I2C_PERIPHERAL_3
    
    /* Use default clock sources, e.g. arduino framework clock system */
    #define I2C_CLOCK_SOURCE I2C_CLK_SOURCE_USE_DEFAULT
    
    /* Use the default configuration options */
    #define I2C_EXTRA_CONFIG_OPTIONS I2C_EXTRA_OPT_NONE
    
    void setup(){
       GPIO_SET_PIN_MODE(SDA_PIN, GPIO_MODE_C);
       GPIO_SET_PIN_MODE(SCL_PIN, GPIO_MODE_C);
       I2C_SLAVE_INIT(I2C_PERIPHERAL, I2C_SLAVE_ADDR, I2C_CLK_SOURCE, I2C_EXTRA_CONFIG_OPTIONS);
    }
    ...
    ```
