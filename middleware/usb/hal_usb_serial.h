/**
* \file            hal_usb_serial.h
* \brief           USB serial module include file
*/
/*
*  Copyright 2023 (C) Victor Hogeweij <hogeweyv@gmail.com>
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
* This file is part of the Universal Hal Framework.
*
* Author:          Victor Hogeweij <hogeweyv@gmail.com>
*/
#ifndef HAL_USB_SERIAL_H
#define HAL_USB_SERIAL_H
#include <error_handling.h>
#include <usb_serial_platform_specific.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * \brief Initialize usb peripheral with serial cdc class functionality
 * 
 * \param serial_instance The serial cdc class instance number
 * \param clock_source The clock source to instantiate the usb peripheral with, if not instantiated already
 * \param clock_frequency The clock frequency of the microcontroller
 * \return uhal_status_t Indicates if initialization was succesfull
 */
uhal_status_t usb_serial_init(const usb_serial_inst_t serial_instance, const usb_clock_sources_t clock_source, const uint32_t clock_frequency);


/**
 * \brief Polls the USB CDC peripheral for the connection status of the serial cdc bus
 * 
 * \param serial_instance The serial cdc class instance number
 * \return const uint8_t 1 or 0, 1 if host device opened the serial port, 0 if host device has not opened serial port
 */
const uint8_t usb_serial_available(const usb_serial_inst_t serial_instance);

/**
 * \brief Write bytes to the usb serial bus
 * 
 * \param serial_instance The serial cdc class instance number
 * \param buffer The buffer to write from
 * \param size The number of bytes to write
 * \return uhal_status_t 0 if succesfull, negative if unsuccesfull
 */
uhal_status_t usb_serial_write_bytes(const usb_serial_inst_t serial_instance, const uint8_t* buffer, const size_t size);

/**
 * @brief Read bytes from the usb serial bus
 * 
 * @param serial_instance The serial cdc class instance number
 * @param buffer The buffer to read to
 * @param size The number of bytes to read
 * @return uhal_status_t 0 if succesfull, negative if unsuccesfull
 */
uhal_status_t usb_serial_read_bytes(const usb_serial_inst_t serial_instance, uint8_t* buffer, const size_t size);

/**
 * \brief Write null_terminated string to the usb serial bus
 * 
 * \param serial_instance The serial cdc class instance number
 * \param buffer The buffer to write from
 * \param size The number of characters to write
 * \return uhal_status_t 0 if succesfull, negative if unsuccesfull
 */
uhal_status_t usb_serial_write_string(const usb_serial_inst_t serial_instance, const char* buffer);


/**
 * \brief Read string from the usb serial bus (reads until '\n' is entered or buffer size is reached, settable option! )
 * 
 * \param serial_instance The serial cdc class instance number
 * \param buffer The buffer to write from
 * \param size The buffer size
 * \return uhal_status_t 0 if succesfull, negative if unsuccesfull
 */
uhal_status_t usb_serial_read_string(const usb_serial_inst_t serial_instance, char* buffer, const size_t size, const uint8_t read_until_newline);

/**
 * \brief Read a single character from the USB serial bus
 * 
 * \param serial_instance The serial cdc class instance number
 * \return uint8_t 
 */
const int8_t usb_serial_read_char(const usb_serial_inst_t serial_instance);

/**
 * \brief De-initializes serial cdc bus on USB peripheral
 * 
 * \param serial_instance The serial cdc class instance number
 * \return uhal_status_t 0 if successfull, negative if unsuccesfull
 */
uhal_status_t usb_serial_deinit(const usb_serial_inst_t serial_instance);





/**
 * \brief This task needs to periodically called to process all the usb transactions
 *        when using no RTOS put it in the while loop! When using a RTOS put it into a task.
 */
void usb_serial_poll_task();


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // HAL_USB_SERIAL_H
