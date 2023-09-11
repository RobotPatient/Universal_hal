get_directory_property(PLATFORM_DEFINED COMPILE_DEFINITIONS)
if(PLATFORM_DEFINED MATCHES "^__SAMD21")
    add_library(Universal_hall "hal/platform/atmelsam/atmel_default_irq_handlers.c"
                               "hal/platform/atmelsam/gpio_samd.c"
                               "hal/platform/atmelsam/i2c.c")
    target_include_directories(Universal_hall PUBLIC "hal/" "utils/" "hal/platform/atmelsam/")
    target_link_libraries(Universal_hall INTERFACE board_sdk)
else()
    # You can define your OS here if desired
    MESSAGE(STATUS "PLATFORM NOT DETECTED")
endif()