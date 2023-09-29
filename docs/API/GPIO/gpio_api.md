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
#### Error Checking
`GPIO_SET_PIN_MODE`: This uppercase version includes compile-time parameter checking, ensuring both pin and pin_mode are compatible with the current microcontroller. This check provides an added layer of security by identifying invalid pin numbers or modes at an early stage.

- Usage Note: **Employ this function exclusively when both pin and pin_mode parameters are known at compile time.**

`gpio_set_pin_mode`: This version executes the same operations as GPIO_SET_PIN_MODE but without the compile-time parameter verification.

- Usage Note: **Utilize this function when dealing with pin or pin_mode values determined at runtime.**

#### Description
The gpio_set_pin_mode function sets the mode of a specified GPIO (General Purpose Input/Output) pin. This function is essential for configuring the GPIO pin to operate in the desired mode, such as input, output, alternate function, or other specialized modes supported by the hardware.

#### Parameters
1. pin (const gpio_pin_t):

	The specific GPIO pin whose mode is to be set.

2. pin_mode (gpio_mode_t):

	The desired mode to set for the GPIO pin. The possible modes are defined by the gpio_mode_t enumeration, which should be documented in the "gpio_platform_specific.h" header file.

#### Return
uhal_status_t: The function returns a status of type uhal_status_t, indicating the success or failure of the operation. The exact definitions of possible return values should be available in the "error_checking.h" header file.


#### Working

1. The function takes a pin and the desired pin_mode as arguments.

2. It configures the pin to operate in the pin_mode specified.
	
	This setup enables the GPIO pin to function in the desired mode for various applications such as digital input, digital output, analog input, PWM output, and more.



