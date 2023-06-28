#ifndef GPIO_HPP
#define GPIO_HPP
#include <gpio_platform_specific.h>
#include <gpio_error_handling.h>

typedef enum {
    kGPIODirInput,
    kGPIODirOutput
}GPIOPinDirection;

#if SUPPORT_PIN_PULL
void SetGPIOPull(const GPIOPin pin, GPIOPull PullMode);
#endif

#if SUPPORT_DRIVE_STRENGTH_SETTING
void SetGPIOPinDriveStrength(const GPIOPin pin, GPIODriveStrength driver_strength);
GPIODriveStrength GetGPIODriveStrength(const GPIOPin pin);
#endif


#if SUPPORT_PINMUX
void SetGPIOPinFunction(const GPIOPin pin, GPIOPinFunction pin_function);
GPIOPinFunction GetGPIOPinFunction(const GPIOPin pin);
#endif

#if SUPPORT_PIN_SAMPLING_MODE_SELECT
void SetGPIOPinSamplingMode(const GPIOPin pin, GPIOSamplingMode sampling_mode);
#endif

#if SUPPORT_PIN_TOGGLE
void ToggleGPIOPin(const GPIOPin pin);
#endif

void SetGPIOPinDirection(const GPIOPin pin, const unsigned char direction);

void SetGPIOPinLevel(const GPIOPin pin, const unsigned char level);

GPIOPinDirection GetGPIOPinDirection(const GPIOPin pin);

#endif
