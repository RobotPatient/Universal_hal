get_directory_property(PLATFORM_DEFINED COMPILE_DEFINITIONS)
if(PLATFORM_DEFINED MATCHES "^__SAMD21")
    add_library(Universal_hall "platform_dep/atmelsam/atmel_default_irq_handlers.c" "hal_gpio/platform/atmelsam/gpio_samd.c" "hal_i2c/platform/atmelsam/i2c.c"
            platform_dep/atmelsam/atmel_default_irq_handlers.h)
    target_include_directories(Universal_hall PUBLIC "hal_gpio/" "hal_gpio/platform/atmelsam/" "hal_i2c/platform/atmelsam" "hal_i2c/" "platform_dep/atmelsam/")
    target_link_libraries(Universal_hall PRIVATE board_sdk)
else()
    # You can define your OS here if desired
    MESSAGE(STATUS "PLATFORM NOT DETECTED")
endif()