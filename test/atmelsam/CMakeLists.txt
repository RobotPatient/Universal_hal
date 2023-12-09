if(UHAL_BUILD_TESTS)

add_library(board_sdk INTERFACE "mocks/sam.h"
                                     "mocks/port_mock.h"
                                     "mocks/nvic_mock.h"
                                     "mocks/gclk_mock.h"
                                     "mocks/eic_mock.h")

target_include_directories(board_sdk INTERFACE mocks/)


add_library(Universal_hal ${UHAL_FILEPATH}/hal/platform/atmelsam/gpio/gpio_samd.c
                          ${UHAL_FILEPATH}/hal/platform/atmelsam/irq/irq_bindings.c)

target_compile_definitions(Universal_hal PUBLIC "DISABLE_I2C_HOST_MODULE"
                                         "DISABLE_I2C_SLAVE_MODULE"
                                         "DISABLE_SPI_HOST_MODULE"
                                         "DISABLE_SPI_SLAVE_MODULE"
                                         "DISABLE_DMA_MODULE"
                                         "DISABLE_DMA_HANDLER")

target_include_directories(Universal_hal PUBLIC ${UHAL_FILEPATH}/hal/platform/atmelsam/
                                                ${UHAL_FILEPATH}/hal/)

target_link_libraries(Universal_hal board_sdk)

add_executable(SAMD_GPIO_TEST gpio/gpio.c)
target_link_libraries(SAMD_GPIO_TEST board_sdk)

endif()