#ifndef GPIO_PLATFORM_SPECIFIC
#define GPIO_PLATFORM_SPECIFIC

#define HAS_NO_GPIO_PORT_DESIGNATORS 0
#define SUPPORT_DRIVE_STRENGTH_SETTING 1
#define SUPPORT_PINMUX 1
#define SUPPORT_PIN_TOGGLE 1
#define SUPPORT_PIN_SAMPLING_MODE_SELECT 1

typedef enum {
   kGPIOPortA,
   kGPIOPortB,
   kGPIOPortC
} GPIOPort;

typedef enum {
    kGPIONormalDriveStrength,
    kGPIOStrongDriveStrength
} GPIODriveStrength;

typedef enum {
  kGPIOFunctionA = 0x0,
  kGPIOFunctionB = 0x1,
  kGPIOFunctionC = 0x2,
  kGPIOFunctionD = 0x3,
  kGPIOFunctionE = 0x4,
  kGPIOFunctionF = 0x5,
  kGPIOFunctionG = 0x6,
  kGPIOFunctionH = 0x7,
  kGPIOFunctionI = 0x8,
  kGPIOFunctionJ = 0x9,
  kGPIOFunctionK = 0xA,
  kGPIOFunctionL = 0xB,
  kGPIOFunctionM = 0xC,
  kGPIOFunctionN = 0xD
} GPIOPinFunction;

typedef enum {
  kGPIOPullDown,
  kGPIOPullUp,
  kGPIONoPull
} GPIOPull;

typedef enum {
  kGPIOSampleOnDemand,
  kGPIOSampleContinuously
} GPIOSamplingMode;

typedef enum {
  kGPIOEventOut,
  kGPIOEventSet,
  kGPIOEventClr,
  kGPIOEventTgl
} GPIOInputEvent;


typedef struct {
  GPIOPort port_num;
  unsigned char pin_num;
} GPIOPin;

#endif
