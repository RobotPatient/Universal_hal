#include <hal_gpio.h>
#include <gpio_platform_specific.h>
#include <gpio.h>

void SetGPIOPinDriveStrength(const GPIOPin pin, GPIODriveStrength driver_strength) {
    gpio_set_drive_strength(pin.pin_num, driver_strength);
}

GPIODriveStrength GetGPIODriveStrength(const GPIOPin pin) {
    return gpio_get_drive_strength(pin.pin_num);
}

void SetGPIOPinFunction(const GPIOPin pin, GPIOPinFunction pin_function) {
    gpio_set_function(pin.pin_num, pin_function);
}

void SetGPIOPull(const GPIOPin pin, GPIOPull PullMode) {
    bool up = (PullMode == (kGPIOPullBusKeep || kGPIOPullUp));
    bool down = (PullMode == (kGPIOPullBusKeep || kGPIOPullDown));
    gpio_set_pulls(pin.pin_num, up, down);
}

GPIOPinFunction GetGPIOPinFunction(const GPIOPin pin) {
return gpio_get_function(pin.pin_num);
}

void SetGPIOPinDirection(const GPIOPin pin, const unsigned char direction) {
gpio_set_dir(pin.pin_num, direction);
}

void SetGPIOPinLevel(const GPIOPin pin, const unsigned char level) {
gpio_put(pin.pin_num, level);
}

GPIOPinDirection GetGPIOPinDirection(const GPIOPin pin) {
return gpio_get_dir(pin.pin_num);
}