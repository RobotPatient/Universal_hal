#[[
  Copyright 2023 (C) Victor Hogeweij <hogeweyv@gmail.com>

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
]]

get_directory_property(PLATFORM_DEFINED COMPILE_DEFINITIONS)
if(PLATFORM_DEFINED MATCHES "^__SAMD21")
    add_library(Universal_hall "hal/platform/atmelsam/default_irq_handlers.c"
                               "hal/platform/atmelsam/gpio_samd.c"
                               "hal/platform/atmelsam/i2c.c"
            hal/universal_hal.h
            hal/platform/atmelsam/spi_host.c
            hal/hal_spi_slave.h
            hal/platform/atmelsam/spi_slave.c
            hal/platform/atmelsam/spi_slave.c
            hal/platform/atmelsam/spi_platform_specific.h
            hal/platform/atmelsam/peripheral_clocking.h
            hal/platform/atmelsam/sercom_stuff.h
            hal/platform/atmelsam/i2c_slave.c)
    target_include_directories(Universal_hall PUBLIC "hal/" "utils/" "hal/platform/atmelsam/")
    target_link_libraries(Universal_hall INTERFACE board_sdk)
else()
    # You can define your OS here if desired
    MESSAGE(STATUS "PLATFORM NOT DETECTED")
endif()