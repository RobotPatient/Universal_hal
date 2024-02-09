#include "clock_init.h"
#include <hal_gpio.h>
#include <hal_uart.h>

/*
* The GPIO Pin attached to the LED
*/
#define LED_PIN GPIO_PIN_PA2

/*
* UART Pins
* PA13 Sercom4/PAD[1]
* PA12 Sercom4/PAD[0]
*/
#define RX_PIN GPIO_PIN_PA13
#define TX_PIN GPIO_PIN_PA12

/*
* Sercom 2 will be used for the UART connection
*/
#define UART_INST UART_PERIPHERAL_4
#define UART_BAUDRATE 115200

int main(void)
{
    /*
     * Set the main clock to 48MHz
     */
	Clock_Init();

    /*
     * Set the LED Pin to output mode
     */
    GPIO_SET_PIN_MODE(LED_PIN, GPIO_MODE_OUTPUT);
    
    /*
    * Set the UART Pin Mux modes
    */
    GPIO_SET_PIN_MODE(RX_PIN, GPIO_MODE_D);
    GPIO_SET_PIN_MODE(TX_PIN, GPIO_MODE_D);

    /*
    * Initialize the UART connection with:
    * 48 MHz main clock, 115200 baud, no parity bit
    */
    uart_init(UART_INST, UART_BAUDRATE, UART_CLK_SOURCE_USE_DEFAULT, CONF_CPU_FREQUENCY, UART_EXTRA_OPT_RX_PAD_1);

    uint8_t msg_buffer[10];
	while (1) {
        uart_receive(UART_INST, msg_buffer, 1);
        uart_transmit(UART_INST, msg_buffer, 1);   
	}
}


