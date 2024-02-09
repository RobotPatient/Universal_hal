#include <hal_gpio.h>
#include <hal_uart.h>
#include <hal_usb_serial.h>
#include "clock_init.h"

/*
* USB Pins
*/
#define USBDM                       GPIO_PIN_PA24
#define USBDP                       GPIO_PIN_PA25

/**
 * USB Serial Instance,
 * Multiple VCOM instances can be initialized by increasing the CFG_TUD_CDC define
*/
#define USB_SERIAL_INST             0

/**
 * Example settings, 
 * ECHO_PUT_CH_NUM is the number of characters being buffered
 * ECHO_PUT_READ_UNTIL_NEWLINE stops reading when newline character comes in
*/
#define ECHO_PUT_CH_NUM             1
#define ECHO_PUT_READ_UNTIL_NEWLINE 0

int main(void) {
    /*
     * Set the main clock to 48MHz
     */
    Clock_Init();

    /*
     * Set the MUX for the USB pins
     */
    GPIO_SET_PIN_MODE(USBDM, GPIO_MODE_OUTPUT);
    GPIO_SET_PIN_MODE(USBDP, GPIO_MODE_OUTPUT);
    GPIO_SET_PIN_LVL(USBDM, 0);
    GPIO_SET_PIN_LVL(USBDP, 0);
    GPIO_SET_PIN_MODE(USBDM, GPIO_MODE_G);
    GPIO_SET_PIN_MODE(USBDP, GPIO_MODE_G);

    /*
	* Initialize the USB serial peripheral with default settings and the CPU_FREQUENCY of 48MHz
	*/
    usb_serial_init(USB_SERIAL_INST, USB_CLK_SOURCE_USE_DEFAULT, CONF_CPU_FREQUENCY);

    char msg[ECHO_PUT_CH_NUM+1];
    while (1) {
		/* Fetch new characters / background task */
        usb_serial_poll_task();
		/* Run if the serial bus is opened */
        if (usb_serial_available(USB_SERIAL_INST)) {
			/* Read a character */
            usb_serial_read_string(USB_SERIAL_INST, msg, ECHO_PUT_CH_NUM, ECHO_PUT_READ_UNTIL_NEWLINE);
			/* Echo the character */
            usb_serial_write_string(USB_SERIAL_INST, msg);
        }
    }
}
