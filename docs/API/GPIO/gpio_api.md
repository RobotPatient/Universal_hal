# GPIO API

## API functionality

The API for GPIO functionality has the following functions available:

```c
/* Pin toggle function (without static parameter checking) */
uhal_status_t gpio_toggle_pin_output(const gpio_pin_t pin);
/* Pin toggle function (with static parameter checking) */
uhal_status_t GPIO_TOGGLE_PIN_OUTPUT(const gpio_pin_t pin);

/* Function to set the pin mode (without static parameter checking) */
uhal_status_t gpio_set_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode);
/* Function to set the pin mode (with static parameter checking) */
uhal_status_t GPIO_SET_PIN_MODE(const gpio_pin_t pin, gpio_mode_t pin_mode);

/* Function to set the pin level (without static parameter checking) */
uhal_status_t gpio_set_pin_lvl(const gpio_pin_t pin, gpio_level_t level);
/* Function to set the pin level (with static parameter checking) */
uhal_status_t GPIO_SET_PIN_LVL(const gpio_pin_t pin, gpio_level_t level);

/* Function to get the currently set pin level (without compile-time parameter checking) */
gpio_level_t gpio_get_pin_lvl(const gpio_pin_t pin);
/* Function to get the currently set pin level (with compile-time parameter checking) */
gpio_level_t GPIO_GET_PIN_LVL(const gpio_pin_t pin);

/* Function to get the currently set pin mode (without compile-time parameter checking) */
gpio_mode_t gpio_get_pin_mode(const gpio_pin_t pin);
/* Function to get the currently set pin mode (with compile-time parameter checking) */
gpio_mode_t GPIO_GET_PIN_MODE(const gpio_pin_t pin);

/* Function to set pin options (without compile-time parameter checking) */
uhal_status_t gpio_set_pin_options(const gpio_pin_t pin, const gpio_opt_t opt);
/* Function to set set pin options (with compile-time parameter checking) */
uhal_status_t GPIO_SET_PIN_OPTIONS(const gpio_pin_t pin, const gpio_opt_t opt);

/* Function to get the currently set pin options (without compile-time parameter checking) */
gpio_opt_t gpio_get_pin_options(const gpio_pin_t pin);
/* Function to get the currently set pin options (with compile-time parameter checking) */
gpio_opt_t GPIO_GET_PIN_OPTIONS(const gpio_pin_t pin);

/* Function to set pin interrupts (without compile-time parameter checking) */
uhal_status_t gpio_set_interrupt_on_pin(const gpio_pin_t pin, gpio_irq_opt_t irq_opt);
/* Function to set pin interrupts (with compile-time parameter checking) */
uhal_status_t GPIO_SET_INTERRUPT_ON_PIN(const gpio_pin_t pin, gpio_irq_opt_t irq_opt);
```

### gpio_toggle_pin_output function
```c
/* Pin toggle function (without static parameter checking) */
uhal_status_t gpio_toggle_pin_output(const gpio_pin_t pin);
/* Pin toggle function (with static parameter checking) */
uhal_status_t GPIO_TOGGLE_PIN_OUTPUT(const gpio_pin_t pin);
```

#### Description
The `gpio_toggle_pin_output` and `GPIO_TOGGLE_PIN_OUTPUT` function allows you to toggle the output state of a specified GPIO pin.

####Error Checking
`GPIO_TOGGLE_PIN_OUTPUT`: This version of the function, written in uppercase letters, includes compile-time parameter checking to ensure that the pin number provided is an existing pin-number. This added layer of safety helps prevent incorrect pin numbers from being passed to the function, catching errors earlier in the development cycle.

- Usage Note: **Utilize this function only when the pin parameter is known at compile time**.

`gpio_toggle_pin_output`: This version of the function performs the same operations but without the compile-time parameter check.

- Usage Note: **Opt for this function when dealing with pin numbers that are determined at runtime.**

#### Parameters

1. `pin` (const gpio_pin_t):

	The specific GPIO pin whose output state is to be toggled.

#### Return:
uhal_status_t: The function returns a status of type uhal_status_t indicating the success or failure of the operation. The exact definitions of possible return values should be available in the "error_checking.h" header file.

#### Working

1. The function takes a specific pin as an argument.

2. It inverts the current output state of the pin (i.e., if the pin is currently in a HIGH state, it will be set to LOW and vice versa).

### gpio_set_pin_mode function
```c
/* Function to set the pin mode (without static parameter checking) */
uhal_status_t gpio_set_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode);
/* Function to set the pin mode (with static parameter checking) */
uhal_status_t GPIO_SET_PIN_MODE(const gpio_pin_t pin, gpio_mode_t pin_mode);
```

#### Description
The gpio_set_pin_mode function sets the mode of a specified GPIO (General Purpose Input/Output) pin. This function is essential for configuring the GPIO pin to operate in the desired mode, such as input, output, alternate function, or other specialized modes supported by the hardware.

#### Error Checking
`GPIO_SET_PIN_MODE`: This uppercase version includes compile-time parameter checking, ensuring both pin and pin_mode are compatible with the current microcontroller. This check provides an added layer of security by identifying invalid pin numbers or modes at an early stage.

- Usage Note: **Employ this function exclusively when both pin and pin_mode parameters are known at compile time.**

`gpio_set_pin_mode`: This version executes the same operations as GPIO_SET_PIN_MODE but without the compile-time parameter verification.

- Usage Note: **Utilize this function when dealing with pin or pin_mode values determined at runtime.**

#### Parameters
1. pin (const gpio_pin_t):

	The specific GPIO pin whose mode is to be set.

2. pin_mode (gpio_mode_t):

	The desired mode to set for the GPIO pin. The possible modes are defined by the gpio_mode_t enumeration, which should be documented in the "gpio_platform_specific.h" header file.

#### Return
**uhal_status_t**: The function returns a status of type uhal_status_t, indicating the success or failure of the operation. The exact definitions of possible return values should be available in the "error_checking.h" header file.


#### Working

1. The function takes a pin and the desired pin_mode as arguments.

2. It configures the pin to operate in the pin_mode specified.
	
	This setup enables the GPIO pin to function in the desired mode for various applications such as digital input, digital output, analog input, PWM output, and more.

### gpio_set_pin_lvl function
```c
/* Function to set the pin level (without static parameter checking) */
uhal_status_t gpio_set_pin_lvl(const gpio_pin_t pin, gpio_level_t level);
/* Function to set the pin level (with static parameter checking) */
uhal_status_t GPIO_SET_PIN_LVL(const gpio_pin_t pin, gpio_level_t level);
```

#### Description
The `gpio_set_pin_lvl` and `GPIO_SET_PIN_LVL` function allows you to set the output state of a specified GPIO pin.

#### Error Checking
`GPIO_SET_PIN_LVL`: This uppercase version includes compile-time parameter checking, ensuring both pin and pin_level are compatible with the current microcontroller. This check provides an added layer of security by identifying invalid pin numbers or modes at an early stage.

- Usage Note: **Employ this function exclusively when both pin and level parameters are known at compile time.**

`gpio_set_pin_lvl`: This version executes the same operations as `GPIO_SET_PIN_LVL` but without the compile-time parameter verification.

- Usage Note: **Utilize this function when dealing with pin or level values determined at runtime.**

#### Parameters:

1. pin (const gpio_pin_t):
        
   The specific GPIO pin whose level is to be set.

2. level (gpio_level_t):
        
   The desired level (HIGH/LOW or equivalent) for the GPIO pin. The possible levels should be defined by the gpio_level_t enumeration.

#### Return:

**uhal_status_t**: The function returns a status of type uhal_status_t, indicating the success or failure of the operation.

#### Working:

1. The function takes the specific pin and desired level as arguments.

2. It sets the output level of the pin to the specified level.


        
### gpio_get_pin_lvl function
```c
/* Function to get the currently set pin level (without compile-time parameter checking) */
gpio_level_t gpio_get_pin_lvl(const gpio_pin_t pin);
/* Function to get the currently set pin level (with compile-time parameter checking) */
gpio_level_t GPIO_GET_PIN_LVL(const gpio_pin_t pin);
```

#### Description

The `gpio_get_pin_lvl` function retrieves the current level (state) of a specified GPIO (General Purpose Input/Output) pin. This allows you to determine whether the pin is in a HIGH or LOW state, making it useful for reading the status of input devices, like buttons or sensors, connected to the pin.

#### Error checking

`GPIO_GET_PIN_LVL`: This uppercase version includes compile-time parameter checking, ensuring the given pin is compatible with the current microcontroller. This check provides an added layer of security by identifying invalid pin numbers or modes at an early stage.

- Usage Note: **Employ this function exclusively when the pin parameter is known at compile time.**

`gpio_get_pin_lvl`: This version executes the same operations as `GPIO_GET_PIN_LVL` but without the compile-time parameter verification.

- Usage Note: **Utilize this function when dealing with pin values determined at runtime.**

#### Parameters:

1. pin (const gpio_pin_t):

	The specific GPIO pin whose level is to be read.

#### Return:

**gpio_level_t**: The function returns the current level of the pin, either HIGH or LOW, as defined in the gpio_level_t enumeration.

#### Working:

1. The function reads the current level or state of the specified pin.

2. It then returns the level, which could be either HIGH, LOW or any other state defined by the gpio_level_t enumeration.
    

### gpio_get_pin_mode function
```c
/* Function to get the currently set pin mode (without compile-time parameter checking) */
gpio_mode_t gpio_get_pin_mode(const gpio_pin_t pin);
/* Function to get the currently set pin mode (with compile-time parameter checking) */
gpio_mode_t GPIO_GET_PIN_MODE(const gpio_pin_t pin);
```

#### Description
The `gpio_get_pin_mode` function retrieves the current mode of a specified GPIO (General Purpose Input/Output) pin. This enables you to determine the current configuration or functioning mode of the pin, such as whether it's set as an input, output, alternate function, or any other specialized mode supported by the hardware.


#### Error checking
`GPIO_GET_PIN_MODE`: This uppercase version includes compile-time parameter checking, ensuring the given pin is compatible with the current microcontroller. This check provides an added layer of security by identifying invalid pin numbers or modes at an early stage.

- Usage Note: **Employ this function exclusively when the pin parameter is known at compile time.**

`gpio_get_pin_mode`: This version executes the same operations as `GPIO_GET_PIN_MODE` but without the compile-time parameter verification.

- Usage Note: **Utilize this function when dealing with pin values determined at runtime.**


#### Parameters
1. pin (const gpio_pin_t):

	The specific GPIO pin whose mode is to be read.

#### Return
**gpio_mode_t**: The function returns the current mode of the pin as defined in the gpio_mode_t enumeration.

#### Working
1. The function reads the current mode or configuration of the specified pin.

2. It then returns the mode, which is defined by the gpio_mode_t enumeration.

### gpio_set_pin_options function
```c
/* Function to set pin options (without compile-time parameter checking) */
uhal_status_t gpio_set_pin_options(const gpio_pin_t pin, const gpio_opt_t opt);
/* Function to set set pin options (with compile-time parameter checking) */
uhal_status_t GPIO_SET_PIN_OPTIONS(const gpio_pin_t pin, const gpio_opt_t opt);
```

#### Description
The gpio_set_pin_options function sets specific options or configurations for a specified GPIO (General Purpose Input/Output) pin. Depending on the hardware or microcontroller in use, GPIO pins can have various options such as pull-up/pull-down resistors, slew rates, or drive strength. This function provides a means to adjust or modify these options.

#### Error checking
`GPIO_SET_PIN_OPTIONS`: This uppercase version includes compile-time parameter checking, ensuring both pin and options are compatible with the current microcontroller. This check provides an added layer of security by identifying invalid pin numbers or modes at an early stage.

- Usage Note: **Employ this function exclusively when both pin and opt parameters are known at compile time.**

`gpio_set_pin_options`: This version executes the same operations as `GPIO_SET_PIN_OPTIONS` but without the compile-time parameter verification.

- Usage Note: **Utilize this function when dealing with pin or opt values determined at runtime.**
#### Parameters
1. pin (const gpio_pin_t):

	The specific GPIO pin whose options are to be set.

2. opt (const gpio_opt_t):

	The desired options or configurations to set for the GPIO pin. The exact options available are defined by the gpio_opt_t enumeration or structure.
#### Return
**uhal_status_t**: The function returns a status of type uhal_status_t, indicating the success or failure of the operation.

#### Working
1. The function takes the pin and the desired opt as arguments.

2. It configures the pin based on the options provided in opt.

### gpio_get_pin_options function
```c
/* Function to get the currently set pin options (without compile-time parameter checking) */
gpio_opt_t gpio_get_pin_options(const gpio_pin_t pin);
/* Function to get the currently set pin options (with compile-time parameter checking) */
gpio_opt_t GPIO_GET_PIN_OPTIONS(const gpio_pin_t pin);
```
#### Description
The `gpio_get_pin_options` function retrieves the currently configured options or configurations of a specified GPIO (General Purpose Input/Output) pin. As GPIO pins can have a variety of configurable options such as pull-up/pull-down resistors, slew rates, or drive strength, this function provides a means to query and understand the current settings of these options.

#### Error checking
`GPIO_GET_PIN_OPTIONS`: This uppercase version includes compile-time parameter checking, ensuring the given pin is compatible with the current microcontroller. This check provides an added layer of security by identifying invalid pin numbers or modes at an early stage.

- Usage Note: **Employ this function exclusively when the pin parameter is known at compile time.**

`gpio_get_pin_options`: This version executes the same operations as `GPIO_GET_PIN_OPTIONS` but without the compile-time parameter verification.

- Usage Note: **Utilize this function when dealing with pin values determined at runtime.**


#### Parameters
1. pin (const gpio_pin_t):

	The specific GPIO pin whose options are to be retrieved.

#### Return
**gpio_opt_t**: The function returns the currently configured options of the pin as defined in the gpio_opt_t enumeration or structure.
        
#### Working
1. The function reads the currently configured options of the specified pin.

2. It then returns the options, as defined by the gpio_opt_t enumeration or structure.

### gpio_set_interrupt_on_pin function
```c
/* Function to set pin interrupts (without compile-time parameter checking) */
uhal_status_t gpio_set_interrupt_on_pin(const gpio_pin_t pin, gpio_irq_opt_t irq_opt);
/* Function to set pin interrupts (with compile-time parameter checking) */
uhal_status_t GPIO_SET_INTERRUPT_ON_PIN(const gpio_pin_t pin, gpio_irq_opt_t irq_opt);
```

#### Description
The `gpio_set_interrupt_on_pin` function configures a specified GPIO (General Purpose Input/Output) pin to generate an interrupt based on specific conditions or events. This enables the system or microcontroller to react immediately to changes on the GPIO pin, such as a button press or sensor reading change, without constantly polling the pin's state.

#### Error checking
`GPIO_SET_INTERRUPT_ON_PIN`: This uppercase version includes compile-time parameter checking, ensuring both pin and irq options are compatible with the current microcontroller. This check provides an added layer of security by identifying invalid pin numbers or modes at an early stage.

- Usage Note: **Employ this function exclusively when both pin and irq_opt parameters are known at compile time.**

`gpio_set_interrupt_on_pin`: This version executes the same operations as `GPIO_SET_INTERRUPT_ON_PIN` but without the compile-time parameter verification.

- Usage Note: **Utilize this function when dealing with pin or irq_opt values determined at runtime.**

#### Parameters
1. pin (const gpio_pin_t):

	The specific GPIO pin on which the interrupt will be configured.

2. irq_opt (gpio_irq_opt_t):

	The condition or event that will trigger the interrupt. The options could include events like a rising edge, falling edge, or level changes, as defined by the gpio_irq_opt_t enumeration or structure.
	
#### Return
**uhal_status_t**: The function returns a status of type uhal_status_t, indicating the success or failure of the operation.

#### Working
1. The function takes the pin and the desired irq_opt as arguments.

2. It configures the interrupt on the pin based on the specified condition or event in irq_opt.

