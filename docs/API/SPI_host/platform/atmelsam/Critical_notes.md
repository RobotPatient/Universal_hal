# SAMD I2C Host Driver: Key Considerations

## On Reliability
The SAMD I2C Host driver, while functional, has room for improvement. It primarily relies on interrupts for data transactions. This approach can momentarily halt the CPU during data transmission or reception from the bus. While there are plans for expanding driver types, for instance through DMA or polling, these are yet to be implemented under the `extra_configuration_options` parameter. Generally, the driver is reliable for many applications. However, to enhance performance and reduce CPU halting, it's advised to limit the frequency of short requests.

## Limitations to Consider
For systems that demand strict timing or hard real-time requirements, this driver may not be the best fit. Here's why:

- **Timeout Constraints**: While the hardware I2C peripheral has timeout provisions for slave responses, the driver lacks timeouts for bus synchronization and other potential fault conditions.
  
- **Error Handling**: Absence of comprehensive error-handling for failed I2C transaction requests compromises maximum reliability.
  
- **Interrupt Timing**: Since the exact moment the CPU gets interrupted is unpredictable, it could potentially disrupt RTOS tasks running on the target device.

## Roadmap for Enhancement
To address the mentioned concerns, the following improvements are recommended:

- Implement error handling for unsuccessful I2C transactions.
- Incorporate additional timeout conditions.
- Introduce DMA support.
- Provide polling support.
