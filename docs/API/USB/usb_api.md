# USB API

## API Functionality

The API for USB functionality has the following functions available:

```c
/* USB Serial driver initialization function (without compile-time parameter checking) */
uhal_status_t usb_serial_init(const usb_serial_inst_t serial_instance, 
                            const usb_clock_sources_t clock_source, 
                            const uint32_t clock_frequency);

/* USB Serial data availability check function (without compile-time parameter checking) */
const int8_t usb_serial_available(const usb_serial_inst_t serial_instance);

/* USB Serial data transmission function (without compile-time parameter checking) */
uhal_status_t usb_serial_write_bytes(const usb_serial_inst_t serial_instance, 
                                    const uint8_t* buffer, 
                                    const size_t size);

/* USB Serial data reception function (without compile-time parameter checking) */
uhal_status_t usb_serial_read_bytes(const usb_serial_inst_t serial_instance, 
                                    uint8_t* buffer, 
                                    const size_t size);

/* USB Serial string transmission function (without compile-time parameter checking) */
uhal_status_t usb_serial_write_string(const usb_serial_inst_t serial_instance, 
                                      const char* buffer);

/* USB Serial string reception function (without compile-time parameter checking) */
uhal_status_t usb_serial_read_string(const usb_serial_inst_t serial_instance, 
                                    char* buffer, const size_t size, 
                                    const uint8_t read_until_newline);

/* USB Serial character reception function (without compile-time parameter checking) */
const uint8_t usb_serial_read_char(const usb_serial_inst_t serial_instance);

/* USB Serial deinitialization function (without compile-time parameter checking) */
uhal_status_t usb_serial_deinit(const usb_serial_inst_t serial_instance);
```
## usb_serial_init function

```c
/* USB Serial driver initialization function (without compile-time parameter checking) */
uhal_status_t usb_serial_init(const usb_serial_inst_t serial_instance, 
                              const usb_clock_sources_t clock_source, 
                              const uint32_t clock_frequency);

/* USB Serial driver initialization function (with compile-time parameter checking) */
uhal_status_t USB_SERIAL_INIT(const usb_serial_inst_t serial_instance, 
                              const usb_clock_sources_t clock_source, 
                              const uint32_t clock_frequency);
```

### Description:

The `usb_serial_init` and `USB_SERIAL_INIT` functions are designed to initialize a USB Serial interface with specific configurations.

### Error Checking:

- **USB_SERIAL_INIT**: Capitalized to denote its usage, this version of the function incorporates compile-time parameter checking to validate the correctness of the provided arguments. Parameters such as `serial_instance` and `clock_source` are verified against predefined valid options at compile time, facilitating early detection of configuration errors before runtime.
  
	- **Usage Note**: Use this function when parameters are predetermined and known at compile time.

- **usb_serial_init**: This variant offers the identical functionality of initializing a USB Serial interface but without the compile-time parameter verification. It is suitable for scenarios where parameters may not be determined until runtime.
  
	- **Usage Note**: Choose this function for more dynamic settings where parameters are decided during runtime.

### Parameters:

1. **serial_instance (const usb_serial_inst_t)**: Specifies the USB Serial instance to be initialized.
2. **clock_source (const usb_clock_sources_t)**: Defines the clock source for the USB Serial interface.
3. **clock_frequency (const uint32_t)**: Sets the frequency of the clock in Hertz used by the USB Serial interface.

### Return:

- **uhal_status_t**: Returns a status code that indicates the outcome of the initialization process. For a comprehensive explanation of possible return values, refer to the "error_checking.h" header file.

### Working:

1. The initialization begins with a verification process to check that the provided parameters are within acceptable limits.
2. Following parameter validation, the function configures the specified `clock_source` for the USB Serial interface.
3. The `clock_frequency` is then adjusted as specified, ensuring the interface operates at the desired clock rate.
4. Upon successful configuration, the USB Serial interface is activated, ready for communication tasks.

This setup process ensures that the USB Serial interface is properly configured and ready for subsequent communication operations, such as data transmission and reception.

## usb_serial_available function

```c
/* USB Serial data availability check function (without compile-time parameter checking) */
const uint8_t usb_serial_available(const usb_serial_inst_t serial_instance);

/* USB Serial data availability check function (with compile-time parameter checking) */
const uint8_t USB_SERIAL_AVAILABLE(const usb_serial_inst_t serial_instance);
```

### Description:

The `usb_serial_available` and `USB_SERIAL_AVAILABLE` functions are designed to check the amount of data available to read from a specified USB Serial interface.

### Error Checking:

- **USB_SERIAL_AVAILABLE**: Capitalized for emphasis, this variant includes compile-time parameter checking to validate the `serial_instance` against predefined valid instances. This compile-time validation ensures that the parameter provided is correct and can help catch configuration or usage errors early in the development process.
  
	- **Usage Note**: This function is intended for use when the `serial_instance` parameter is known at compile time and can be validated against existing configurations.

- **usb_serial_available**: This version provides the same functionality as its uppercase counterpart but without compile-time parameter checking. It is suitable for scenarios where the serial instance may be determined dynamically at runtime.
  
	- **Usage Note**: Opt for this function when the `serial_instance` parameter may vary during the execution of the application.

### Parameters:

1. **serial_instance (const usb_serial_inst_t)**: The USB Serial instance for which the data availability is being queried.

### Return:

- **const uint8_t**: Returns the number of bytes available for reading from the specified USB Serial interface. This can be used to determine if data is ready to be read without blocking the calling thread.

### Working:

The function checks the internal buffer of the specified USB Serial instance to determine how much data is available. This allows applications to efficiently manage data reading operations by ensuring that read operations are attempted only when data is actually available, thereby avoiding unnecessary blocking or polling.

This method is particularly useful in event-driven or non-blocking I/O scenarios where the application can perform other tasks or enter a low-power state while waiting for data to become available on the USB Serial interface.

## usb_serial_write_bytes function

```c
/* USB Serial data transmission function (without compile-time parameter checking) */
uhal_status_t usb_serial_write_bytes(const usb_serial_inst_t serial_instance, 
                                     const uint8_t* buffer, 
                                     const size_t size);

/* USB Serial data transmission function (with compile-time parameter checking) */
uhal_status_t USB_SERIAL_WRITE_BYTES(const usb_serial_inst_t serial_instance, 
                                     const uint8_t* buffer, 
                                     const size_t size);
```

### Description:

The `usb_serial_write_bytes` and `USB_SERIAL_WRITE_BYTES` functions are designed to transmit a sequence of bytes over a USB Serial interface, using a specified USB Serial instance.

### Error Checking:

- **USB_SERIAL_WRITE_BYTES**: Presented in uppercase, this variant includes compile-time parameter checking to validate the `serial_instance` against pre-defined valid instances and ensures that the `buffer` and `size` parameters are correctly provided. This level of validation at compile time assists in identifying misconfigurations or potential errors before the code is executed, contributing to a more robust and error-resistant implementation.
  
	- **Usage Note**: Use this function when the parameters, especially the `serial_instance`, are known at compile time and can be statically checked for validity.

- **usb_serial_write_bytes**: This version offers identical functionality for sending data over a USB Serial interface but does not include compile-time checks for the parameters. It is intended for situations where the serial instance or data to be sent may not be determined until runtime.
  
	- **Usage Note**: Opt for this function when dealing with dynamic data or when the `serial_instance` may vary during application execution.

### Parameters:

1. **serial_instance (const usb_serial_inst_t)**: The USB Serial instance through which the data will be transmitted.
2. **buffer (const uint8_t*)**: A pointer to the array of bytes that are to be sent.
3. **size (const size_t)**: The number of bytes to transmit from the `buffer`.

### Return:

- **uhal_status_t**: Returns a status code indicating the success or failure of the data transmission process. For detailed explanations of possible return values, refer to the "error_checking.h" header file.

### Working:

1. The function first verifies that the `serial_instance` is valid and that the `buffer` and `size` parameters are correctly specified.
2. It then queues the bytes from the `buffer` for transmission over the specified USB Serial interface.
3. The function manages the actual data transmission, ensuring that all bytes in the `buffer` are sent according to the specified `size`.
4. Upon completion, the function returns a status indicating the outcome of the transmission operation, such as success, partial transmission, or failure due to errors.

## usb_serial_read_bytes function

```c
/* USB Serial data reception function (without compile-time parameter checking) */
uhal_status_t usb_serial_read_bytes(const usb_serial_inst_t serial_instance, 
                                    uint8_t* buffer, 
                                    const size_t size);

/* USB Serial data reception function (with compile-time parameter checking) */
uhal_status_t USB_SERIAL_READ_BYTES(const usb_serial_inst_t serial_instance, 
                                    uint8_t* buffer, 
                                    const size_t size);
```

### Description:

The `usb_serial_read_bytes` and `USB_SERIAL_READ_BYTES` functions are designed for receiving a specified number of bytes from a USB Serial interface into a provided buffer.

### Error Checking:

- **USB_SERIAL_READ_BYTES**: Highlighted in uppercase, this variant incorporates compile-time parameter checking to ensure the correctness of the `serial_instance`. This compile-time validation confirms that the `serial_instance` provided is among predefined valid instances and that the `buffer` and `size` parameters are appropriately set up. This preemptive check aids in catching misconfigurations or potential errors early in the development cycle, contributing to a more reliable implementation.
  
	- **Usage Note**: Employ this function when the `serial_instance` and the size of data to be read are known at compile time, allowing for static verification of the parameters' validity.

- **usb_serial_read_bytes**: This version offers the same functionality for data reception via a USB Serial interface but without the compile-time checks for `serial_instance`, `buffer`, and `size`. It is tailored for situations where the serial instance to be used or the amount of data to be read may not be fixed until runtime.
  
	- **Usage Note**: Opt for this function in more dynamic scenarios where the `serial_instance` or the amount of data to be received can vary during the application's operation.

### Parameters:

1. **serial_instance (const usb_serial_inst_t)**: Identifies the USB Serial instance from which data is to be received.
2. **buffer (uint8_t *)**: Points to the memory location where the received data will be stored.
3. **size (const size_t)**: Specifies the number of bytes to read into the `buffer`.

### Return:

- **uhal_status_t**: Returns a status indicating the outcome of the read operation. Success, partial data reception, or failure due to errors such as buffer overflow or communication issues are among the possible return values. Refer to the "error_checking.h" header file for a detailed explanation of return codes.

### Working:

1. The function initiates by validating that the `serial_instance` is configured correctly and that the `buffer` and `size` are suitable for receiving the specified amount of data.
2. It then proceeds to read bytes from the USB Serial interface, storing them into the `buffer` until the `size` number of bytes has been received.
3. The function handles any communication errors or buffer management issues that may occur during the data reception process.
4. Upon completion, a status is returned to indicate the success level of the data reception, providing insight into whether the operation was completed as expected or if issues were encountered.

## usb_serial_write_string function

```c
/* USB Serial string transmission function (without compile-time parameter checking) */
uhal_status_t usb_serial_write_string(const usb_serial_inst_t serial_instance, 
                                      const char* buffer);

/* USB Serial string transmission function (with compile-time parameter checking) */
uhal_status_t USB_SERIAL_WRITE_STRING(const usb_serial_inst_t serial_instance, 
                                      const char* buffer);
```

### Description:

The `usb_serial_write_string` and `USB_SERIAL_WRITE_STRING` functions are tailored for sending a null-terminated string over a specified USB Serial interface. These functions simplify the process of transmitting text data, making it straightforward to send messages or commands via USB Serial communication.

### Error Checking:

- **USB_SERIAL_WRITE_STRING**: This uppercase version denotes the function that incorporates compile-time parameter checking, ensuring that the `serial_instance` provided corresponds to a valid, predefined USB Serial instance. Additionally, it verifies that the `buffer` parameter is a valid pointer. This level of scrutiny at compile time assists in preemptively identifying misconfigurations or errors, enhancing the reliability and stability of the implementation.
  
	- **Usage Note**: Employ this function when the `serial_instance` is a known constant at compile time, allowing for static validation of parameters for added security and robustness.

- **usb_serial_write_string**: Offers identical functionality for string transmission over USB Serial without compile-time checks on the `serial_instance` and `buffer`. This version is more flexible, accommodating scenarios where the serial instance or the string to be sent may be determined dynamically during runtime.
  
	- **Usage Note**: Choose this function when dealing with dynamic conditions where the `serial_instance` or the message content might change over time or are not known until the point of execution.

### Parameters:

1. **serial_instance (const usb_serial_inst_t)**: Specifies the USB Serial instance through which the string will be transmitted.
2. **buffer (const char *)**: A pointer to a null-terminated string that contains the data to be sent.

### Return:

- **uhal_status_t**: Returns a status indicating the success or failure of the string transmission. The function can return values denoting complete success, partial transmission if the buffer could not be fully sent, or failure due to communication errors. Detailed definitions of possible return values can be found in the "error_checking.h" header file.

### Working:

1. Initially, the function checks that the `serial_instance` is valid and that the `buffer` points to a null-terminated string.
2. It then proceeds to transmit the characters of the string sequentially over the USB Serial interface until it reaches the null terminator, marking the end of the string.
3. The function manages the data transmission, ensuring the string is sent in its entirety or identifying any issues that might prevent successful completion.
4. Upon finishing the transmission, it returns a status to indicate whether the string was successfully sent, partially sent, or if the operation failed.


## usb_serial_read_string function

```c
/* USB Serial string reception function (without compile-time parameter checking) */
uhal_status_t usb_serial_read_string(const usb_serial_inst_t serial_instance, 
                                     char* buffer, 
                                     const size_t size, 
                                     const uint8_t read_until_newline);

/* USB Serial string reception function (with compile-time parameter checking) */
uhal_status_t USB_SERIAL_READ_STRING(const usb_serial_inst_t serial_instance, 
                                     char* buffer, 
                                     const size_t size, 
                                     const uint8_t read_until_newline);
```

### Description:

The `usb_serial_read_string` and `USB_SERIAL_READ_STRING` functions are tailored to receive a string from a USB Serial interface, storing it into a provided buffer. These functions are designed with flexibility to either read a specified number of characters or to stop reading upon encountering a newline character, depending on the `read_until_newline` parameter.

### Error Checking:

- **USB_SERIAL_READ_STRING**: Presented in uppercase, this variant integrates compile-time parameter checking to verify the `serial_instance` against predefined valid instances. It also ensures that the `buffer` and `size` parameters are suitably provided, and that the `read_until_newline` flag is a valid boolean value. This preemptive validation at compile time aids in identifying potential misconfigurations or errors early, enhancing the implementation's reliability.
  
	- **Usage Note**: Utilize this function when the parameters, particularly the `serial_instance`, are known constants at compile time, allowing for static validation and ensuring a robust setup.

- **usb_serial_read_string**: This version provides the same functionality for string reception via USB Serial without compile-time checks on the `serial_instance`, `buffer`, `size`, and `read_until_newline`. It accommodates scenarios where the serial instance or data reading criteria may be decided dynamically during runtime.
  
	- **Usage Note**: Choose this function in scenarios where flexibility is required, such as when the `serial_instance` or reading conditions may vary over time or are determined based on runtime conditions.

### Parameters:

1. **serial_instance (const usb_serial_inst_t)**: Identifies the USB Serial instance from which the string is to be received.
2. **buffer (char *)**: Points to the memory location where the received string will be stored.
3. **size (const size_t)**: Specifies the maximum number of characters to read into the `buffer`, including the null terminator.
4. **read_until_newline (const uint8_t)**: A flag indicating whether to stop reading after a newline character is received (`1`) or to read until the `size` limit is reached (`0`).

### Return:

- **uhal_status_t**: Returns a status indicating the success or failure of the string reception. Possible values include success, partial reception if the buffer becomes full before reaching a newline, or failure due to communication errors or buffer issues. Refer to the "error_checking.h" header file for detailed explanations of return codes.

### Working:

1. The function begins by ensuring that the `serial_instance` is correctly configured and that the `buffer` and `size` are suitable for the intended operation.
2. It reads characters from the USB Serial interface into the `buffer`, adhering to the specified `size` limit and optionally stopping at a newline character if `read_until_newline` is set to `1`.
3. The function manages the reception process, handling any issues that might arise, such as buffer overflows or communication errors.
4. Upon completion, it returns a status to indicate the outcome of the reception, informing the caller of successful data acquisition, incomplete reception, or failure.


## usb_serial_read_char function

```c
/* USB Serial character reception function (without compile-time parameter checking) */
const int8_t usb_serial_read_char(const usb_serial_inst_t serial_instance);

/* USB Serial character reception function (with compile-time parameter checking) */
const int8_t USB_SERIAL_READ_CHAR(const usb_serial_inst_t serial_instance);
```

### Description:

The `usb_serial_read_char` and `USB_SERIAL_READ_CHAR` functions are designed to read a single character from a specified USB Serial interface. These functions streamline the process of receiving data by providing a simple method for retrieving one byte of information at a time from the serial buffer.

### Error Checking:

- **USB_SERIAL_READ_CHAR**: Indicated in uppercase, this version includes compile-time parameter checking to confirm the `serial_instance` against predefined valid instances. This ensures that the `serial_instance` provided is correct and helps in identifying configuration or usage errors before runtime, thus enhancing the robustness of the implementation.
  
	- **Usage Note**: Use this function when the `serial_instance` is a known constant at compile time, enabling static verification of the parameter's validity for increased safety and reliability.

- **usb_serial_read_char**: Offers the same functionality for reading a character from a USB Serial interface but without compile-time checks for the `serial_instance`. It is intended for scenarios where the serial instance may be determined dynamically, providing flexibility in applications where runtime conditions dictate serial communication dynamics.
  
	- **Usage Note**: Opt for this function in dynamic scenarios where the `serial_instance` may change or is determined at runtime, requiring a more flexible approach to serial communication.

### Parameters:

1. **serial_instance (const usb_serial_inst_t)**: Specifies the USB Serial instance from which the character is to be read.

### Return:

- **const uint8_t**: Returns the character read from the specified USB Serial interface. If no data is available, the returned value is equal to -1

### Working:

1. The function starts by ensuring that the `serial_instance` specified is valid and ready for data reception.
2. It then attempts to read a single character from the USB Serial buffer associated with the `serial_instance`.
3. The process involves checking the serial buffer for available data and retrieving one byte (character) if available.
4. Upon successful reading, the character is returned to the caller. If no data is available or an error occurs, the function's response will depend on its defined error handling or return value conventions.

## usb_serial_deinit function

```c
/* USB Serial deinitialization function (without compile-time parameter checking) */
uhal_status_t usb_serial_deinit(const usb_serial_inst_t serial_instance);

/* USB Serial deinitialization function (with compile-time parameter checking) */
uhal_status_t USB_SERIAL_DEINIT(const usb_serial_inst_t serial_instance);
```

### Description:

The `usb_serial_deinit` and `USB_SERIAL_DEINIT` functions are designed to properly deinitialize a previously initialized USB Serial interface. These functions ensure that any resources allocated during the operation of the USB Serial instance are correctly released and that the instance is returned to a default state, ready for re-initialization if necessary.

### Error Checking:

- **USB_SERIAL_DEINIT**: Presented in uppercase, this version includes compile-time parameter checking to confirm the `serial_instance` against predefined valid instances. This compile-time validation ensures the correctness and appropriateness of the `serial_instance`, aiding in the prevention of errors related to invalid or uninitialized instances prior to runtime.
  
	- **Usage Note**: Utilize this function when the `serial_instance` is a known constant at compile time, allowing for static verification of the parameter's validity. This is particularly useful in statically configured environments where the serial configuration is known beforehand and can be validated for correctness.

- **usb_serial_deinit**: Provides the same functionality for deinitializing a USB Serial interface but without compile-time checks for the `serial_instance`. This version offers flexibility for dynamically configured systems where the serial instance may vary over time or be determined based on runtime conditions.
  
	- **Usage Note**: Choose this function in scenarios requiring dynamic handling of USB Serial instances, such as applications that might reconfigure serial connections based on user input or other runtime conditions.

### Parameters:

1. **serial_instance (const usb_serial_inst_t)**: Specifies the USB Serial instance to be deinitialized.

### Return:

- **uhal_status_t**: Returns a status indicating the success or failure of the deinitialization process. The function can return values indicating successful deinitialization, failure due to errors such as invalid instance specifications, or other issues encountered during the release of resources. Detailed explanations of possible return values should be consulted in the "error_checking.h" header file.

### Working:

1. The function begins by validating the provided `serial_instance` to ensure it is a currently active and valid USB Serial interface.
2. It then proceeds to disable the USB Serial interface, carefully releasing any resources, such as memory or hardware locks, that were allocated during its operation.
3. The function also ensures any ongoing or pending serial communications are properly concluded or aborted to avoid leaving the system in an inconsistent state.
4. Following the successful release of resources and deactivation of the interface, the function returns a status indicating the outcome of the deinitialization process.
