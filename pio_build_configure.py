#
#  Copyright 2023 (C) Victor Hogeweij <hogeweyv@gmail.com>
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#

Import('env')
import os
import sys
from os.path import join, realpath

platform = env.get("PIOPLATFORM")
path = join(os.getcwd()+"/hal/platform/", platform)
if(os.path.exists(path)):
    env.Append(CPPPATH=[realpath("hal"), realpath(join("hal/platform/", platform))])
    env.Replace(SRC_FILTER=["+<hal/platform/%s/*>" % platform])
else:
    print("ERROR! MCU: %s specified in pio config is not supported" % platform)
    sys.exit()


