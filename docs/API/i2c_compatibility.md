# I2C compatibility

## Platform compatibility:

The standard I2C API for this framework has basic I2C Master and I2C Slave functionality. However not every platform has I2C peripherals or supports both I2C Master and I2C Slave functionality.

| Platform   | I2C Host support    | I2C Slave support   | HW peripheral/Bitbanged |
| ---------- | ------------------- | ------------------- | ----------------------- |
| Atmel SAMD | ✔                   | ✔                   | HW peripheral           |
| RPI RP2040 | Not yet implemented | Not yet implemented | HW peripheral           |

### Atmel SAMD

The atmel SAMD series has full support for I2C Host and I2C slave functionality. The implementation is fully in hardware using the SERCOM peripherals. The implementation is purely based on interrupts, which are overridable (when using the GCC or Keil compiler). This interrupts based approach might be a concern when using very timing critical system as it is very generic and might mess with timing, unless thoroughly tested.

#### Internal implementation details

```mermaid
flowchart TD
    START(Start ISR) --> TRANSACTIONTYPEW{Transaction is I2C host write transaction?}
   
    TRANSACTIONTYPEW--> |No| TRANSACTIONTYPER{Transaction is I2C host read transaction?}
    TRANSACTIONTYPEW--> |Yes| BYTETOWRITE{Is there a byte left to write?}
    BYTETOWRITE --> |Yes| WRITEBYTE(Write byte from transaction buffer to the bus)
    WRITEBYTE --> LASTBYTEW{Was this the last byte?}
    LASTBYTEW --> |Yes| GENERATEI2CSTOP
    LASTBYTEW --> |No| RESET_INTFLAG(Reset interrupt flags)
    BYTETOWRITE --> |No| GENERATEI2CSTOP(Generate stop condition)
    TRANSACTIONTYPER--> |No| RESET_INTFLAG
    TRANSACTIONTYPER --> |Yes| BYTETOREAD{Is there a byte left to read?}
    BYTETOREAD --> |Yes| READBYTE(Read byte from bus and write it to the transaction buffer)

    READBYTE --> LASTBYTER{Was this the last byte?}
    LASTBYTER --> |No| GENERATEACK(Generate ACK action on bus and request more bytes)
    GENERATEACK --> RESET_INTFLAG
    LASTBYTER --> |Yes| GENERATENAK
    GENERATENAK --> GENERATEI2CSTOP
    BYTETOREAD --> |No| GENERATENAK(Generate NAK action on bus)    
    GENERATEI2CSTOP --> RESET_INTFLAG
    OTHERISR --> RESET_INTFLAG
    RESET_INTFLAG --> OTHERISR(Run other communication interface ISR)  
    OTHERISR --> EXIT[Exit ISR]

```
