#ifndef GPIO_PLATFORM_SPECIFIC
#define GPIO_PLATFORM_SPECIFIC
#include <gpio.h>

#define SUPPORT_PIN_PULL 1
#define HAS_NO_GPIO_PORT_DESIGNATORS 1
#define SUPPORT_DRIVE_STRENGTH_SETTING 1
#define SUPPORT_PINMUX 1
#define SUPPORT_PIN_TOGGLE 0
#define SUPPORT_PIN_SAMPLING_MODE_SELECT 0

#define GPIODriveStrength enum gpio_drive_strength

#define GPIOPinFunction enum gpio_function

typedef enum {
  kGPIOPullBusKeep,
  kGPIOPullDown,
  kGPIOPullUp,
  kGPIONoPull
} GPIOPull;

typedef enum {
  kGPIOEventOut,
  kGPIOEventSet,
  kGPIOEventClr,
  kGPIOEventTgl
} GPIOInputEvent;


typedef struct {
  unsigned char pin_num;
} GPIOPin;
#endif
