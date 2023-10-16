# I2C Slave driver compatibility

## Platform compatibility:

The standard I2C API for this framework has basic I2C Slave functionality. However not every platform has I2C peripherals or support I2C Slave functionality.

| Platform   | I2C Slave support  | HW peripheral/Bitbanged |
| ---------- | ------------------- | ----------------------- |
| Atmel SAMD | ✔                   | HW peripheral           |
| RPI RP2040 | Not yet implemented | HW peripheral           |