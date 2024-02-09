cmake_minimum_required(VERSION 3.16)

include(FetchContent)
FetchContent_Declare(
  SAMD_Toolchain
  GIT_REPOSITORY https://github.com/Hoog-V/SAMD_Toolchain.git
  GIT_TAG        main
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../SDK
)

FetchContent_MakeAvailable(SAMD_Toolchain)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/../SDK/samd21g18_toolchain_armgcc.cmake)

project(atmelsam_gpio_blinky)

add_subdirectory(./../../../ Universal_hal)


add_executable(blinky.elf "src/main.c" ${STARTUP_SCRIPT_SOURCES})
target_link_libraries(blinky.elf PUBLIC Universal_hal)
