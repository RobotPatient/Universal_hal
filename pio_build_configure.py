Import('env')
import os
import sys
from os.path import join, realpath

hal_var_found = False
#
# Private flags (only for the current "SomeLib" source files)
#
platform = env.get("PIOPLATFORM")
path = join(os.getcwd()+"/hal_gpio/platform/", platform)
if(os.path.exists(path)):
    hal_var_found = True
    env.Append(CPPPATH=[realpath("hal_gpio/"), realpath(join("hal_gpio/platform/", platform))])
    env.Append(CPPPATH=[realpath("hal_i2c"), realpath(join("hal_i2c/platform/", platform))])
    env.Replace(SRC_FILTER=["+<hal_gpio/gpio.h>","+<hal_gpio/gpio_error_handling.h>", "+<*/platform/%s/*>" % platform])
else:
    print("ERROR! MCU: %s specified in pio config is not supported" % platform)
    sys.exit()


