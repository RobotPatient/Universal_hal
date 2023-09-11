Import('env')
import os
import sys
from os.path import join, realpath

hal_var_found = False
#
# Private flags (only for the current "SomeLib" source files)
#
platform = env.get("PIOPLATFORM")
path = join(os.getcwd()+"/hal/platform/", platform)
if(os.path.exists(path)):
    hal_var_found = True
    env.Append(CPPPATH=[realpath("hal"), realpath(join("hal/platform/", platform))])
    env.Replace(SRC_FILTER=["+<hal/*>", "+<hal/platform/%s/*>" % platform])
else:
    print("ERROR! MCU: %s specified in pio config is not supported" % platform)
    sys.exit()


