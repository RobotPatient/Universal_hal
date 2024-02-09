if(UHAL_BUILD_TESTS)
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.x
)

# CMake 3.14+
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
    fff
    GIT_REPOSITORY https://github.com/meekrosoft/fff.git
    GIT_TAG master
)

FetchContent_MakeAvailable(fff)

add_library(board_sdk INTERFACE "mocks/sam.h"
                                     "mocks/port_mock.h"
                                     "mocks/nvic_mock.h"
                                     "mocks/gclk_mock.h"
                                     "mocks/eic_mock.h"
                                     "mocks/sercom_mock.h")

target_include_directories(board_sdk INTERFACE mocks/)


add_library(Universal_hal_gpio ${UHAL_FILEPATH}/hal/platform/atmelsam/gpio/gpio_samd.c
                          ${UHAL_FILEPATH}/hal/platform/atmelsam/irq/irq_bindings.c)

target_compile_definitions(Universal_hal_gpio PUBLIC "DISABLE_I2C_HOST_MODULE"
                                         "DISABLE_I2C_SLAVE_MODULE"
                                         "DISABLE_SPI_HOST_MODULE"
                                         "DISABLE_SPI_SLAVE_MODULE"
                                         "DISABLE_DMA_MODULE"
                                         "DISABLE_DMA_HANDLER"
                                         "DISABLE_UART_MODULE")

target_include_directories(Universal_hal_gpio PUBLIC ${UHAL_FILEPATH}/hal/platform/atmelsam/
                                                ${UHAL_FILEPATH}/hal/)

target_link_libraries(Universal_hal_gpio board_sdk)


add_library(Universal_hal_uart ${UHAL_FILEPATH}/hal/platform/atmelsam/uart/uart.c)

target_compile_definitions(Universal_hal_uart PUBLIC "DISABLE_I2C_HOST_MODULE"
                                         "DISABLE_I2C_SLAVE_MODULE"
                                         "DISABLE_SPI_HOST_MODULE"
                                         "DISABLE_SPI_SLAVE_MODULE"
                                         "DISABLE_DMA_MODULE"
                                         "DISABLE_DMA_HANDLER")

target_include_directories(Universal_hal_uart PUBLIC ${UHAL_FILEPATH}/hal/platform/atmelsam/
                                                     ${UHAL_FILEPATH}/hal/)

target_link_libraries(Universal_hal_uart board_sdk)



add_executable(SAMD_GPIO_TEST gpio/gpio.cpp)
target_link_libraries(SAMD_GPIO_TEST Universal_hal_gpio board_sdk gtest_main gmock_main fff)
target_compile_definitions(SAMD_GPIO_TEST PUBLIC "FFF_GENERATE")


add_executable(SAMD_UART_TEST uart/uart.cpp)
target_link_libraries(SAMD_UART_TEST Universal_hal_uart board_sdk gtest_main gmock_main fff)
target_compile_definitions(SAMD_UART_TEST PUBLIC "FFF_GENERATE")

add_test(
        NAME SAMD_GPIO_TEST
        COMMAND SAMD_GPIO_TEST
)

add_test(
  NAME SAMD_UART_TEST
  COMMAND SAMD_UART_TEST
)
endif()