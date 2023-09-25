# I2C compatibility

## Platform compatibility:

The standard I2C API for this framework has basic I2C Master and I2C Slave functionality. However not every platform has I2C peripherals or supports both I2C Master and I2C Slave functionality.

| Platform   | I2C Master support  | I2C Slave support   | HW peripheral/Bitbanged |
| ---------- | ------------------- | ------------------- | ----------------------- |
| Atmel SAMD | ✔                   | ✔                   | HW peripheral           |
| RPI RP2040 | Not yet implemented | Not yet implemented | HW peripheral           |

## How does the I2C bus work?

The Inter-Integrated Circuit (I2C) protocol is a popular communication protocol that microcontrollers and microprocessors use to communicate with various external devices, like sensors, memory devices, and other microcontrollers. It’s a simple, two-wire serial interface that’s ideal for interconnecting low-speed peripherals to a motherboard, embedded system, or cellphone.

### Basic Concepts

1. **Master and Slave Devices:**
	- **Master Device:** Initiates and controls the data transfer.
	- **Slave Device:** Responds to the master device.
  
2. **Two-Wire Communication:**
	- **SDA (Serial Data Line):** Carries data.
	- **SCL (Serial Clock Line):** Synchronizes data transfer.

### Working Principle

1. The master device initiates communication.
2. The master device sends the address of the specific slave device it wants to communicate with, and whether it wants to write or read data from the slave device. 
3. The master sends or requests data to/from the slave.
4. The slave sends an acknowledgment bit for each byte received.
5. The master ends the communication.
### Atmel SAMD

The atmel SAMD series has full support for I2C Host and I2C slave functionality. The implementation is fully in hardware using the SERCOM peripherals. The implementation is purely based on interrupts, which are overridable (when using the GCC or Keil compiler). This interrupts based approach might be a concern when using very timing critical system as it is very generic and might mess with timing, unless thoroughly tested.

#### Internal implementation details

```mermaid
  graph TD;
      A-->B;
      A-->C;
      B-->D;
      C-->D;
```
