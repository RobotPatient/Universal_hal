# Atmel SAMD

The atmel SAMD series has full support for I2C Host and I2C slave functionality. The implementation is fully in hardware using the SERCOM peripherals. The implementation is purely based on interrupts, which are overridable (when using the GCC or Keil compiler). This interrupts based approach might be a concern when using very timing critical system as it is very generic and might mess with timing, unless thoroughly tested.

#### Internal implementation details
