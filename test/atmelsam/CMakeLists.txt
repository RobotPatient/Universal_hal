if(UHAL_BUILD_TESTS)
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.x
)

# CMake 3.14+
FetchContent_MakeAvailable(googletest)

add_library(board_sdk INTERFACE "mocks/sam.h"
                                     "mocks/port_mock.h")

target_include_directories(board_sdk INTERFACE mocks/)


add_library(Universal_hal ${UHAL_FILEPATH}/hal/platform/atmelsam/gpio/gpio_samd.c
                          ${UHAL_FILEPATH}/hal/platform/atmelsam/irq/irq_bindings.c)
target_compile_definitions(Universal_hal PUBLIC DISABLE_I2C_HOST_MODULE
                                         DISABLE_I2C_SLAVE_MODULE
                                         DISABLE_SPI_HOST_MODULE
                                         DISABLE_SPI_SLAVE_MODULE
                                         DISABLE_DMA_MODULE)
target_include_directories(Universal_hal PUBLIC ${UHAL_FILEPATH}/hal/platform/atmelsam/
                                                ${UHAL_FILEPATH}/hal/)

target_link_libraries(Universal_hal board_sdk)

add_executable(SAMD_GPIO_TEST gpio/gpio.cpp)
target_link_libraries(SAMD_GPIO_TEST  gtest_main gmock_main board_sdk)

endif()