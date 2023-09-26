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


#### SERCOMX_Handler ISR
```mermaid
flowchart TD
    subgraph SERCOMX_Handler
    EXIT[Exit ISR]
    START(Start ISR)
    TRANSACTION_TYPE_I2C_HOST_R{Transaction is I2C host read transaction?}
    TRANSACTION_TYPE_I2C_HOST_W{Transaction is I2C host write transaction?}
    
    START --> TRANSACTION_TYPE_I2C_HOST_W{Transaction is I2C host write transaction?}

    TRANSACTIONTYPEW--> |No| TRANSACTIONTYPER
    TRANSACTIONTYPEW--> |Yes| i2c_host_x_data_send_irq(Run the i2c_host_x_data_send_irq routine)
    TRANSACTIONTYPER --> |No| EXIT
    TRANSACTIONTYPER --> |Yes| i2c_host_x_data_recv_irq(Run the i2c_host_x_data_recv_irq_routine)
    i2c_host_x_data_send_irq --> EXIT
    i2c_host_x_data_recv_irq --> EXIT
    end
```
#### i2c_host_x_data_send_irq standard implementation
```mermaid
flowchart TD
    subgraph i2c_master_x_data_send_irq
    START[Start Routine] --> BYTETOWRITE
    EXIT[Exit Routine]
    BYTETOWRITE{Is there a byte left to write?}
    BYTETOWRITE --> |Yes| WRITEBYTE(Write byte from transaction buffer to the bus)
    WRITEBYTE --> LASTBYTEW{Was this the last byte?}
    LASTBYTEW --> |Yes| GENERATEI2CSTOPW
    LASTBYTEW --> |No| RESET_INTFLAGW(Request next i2c host write interrupt)
    BYTETOWRITE --> |No| GENERATEI2CSTOPW(Generate stop condition)
    RESET_INTFLAGW --> EXIT
    GENERATEI2CSTOPW --> EXIT
    end
```
#### i2c_host_x_data_send_irq standard implementation
 
```mermaid
 flowchart TD
    subgraph  i2c_master_x_data_recv_irq
    START[Start Routine] --> BYTETOREAD
    EXIT[Exit Routine]
    BYTETOREAD{Is there a byte left to read?}
    BYTETOREAD --> |Yes| READBYTE(Read byte from bus and write it to the transaction buffer)
    READBYTE --> LASTBYTER{Was this the last byte?}
    LASTBYTER --> |No| GENERATEACK(Generate ACK action on bus and request more bytes)
    GENERATEACK --> EXIT
    LASTBYTER --> |Yes| GENERATENAK
    GENERATENAK --> GENERATEI2CSTOP(Generate stop condition on bus)
    GENERATEI2CSTOP--> EXIT
    BYTETOREAD --> |No| GENERATENAK(Generate NAK action on bus)    
    end
```
