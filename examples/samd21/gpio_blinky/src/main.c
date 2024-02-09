#include "clock_init.h"
#include <hal_gpio.h>

/*
* The GPIO Pin attached to the LED
*/
#define LED_PIN GPIO_PIN_PA2


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

	while (1) {
    /*
    * Toggle the GPIO output state
    */
	GPIO_TOGGLE_PIN_OUTPUT(LED_PIN);
	for(uint32_t i =0; i< 1000000; i++);
	}
}


