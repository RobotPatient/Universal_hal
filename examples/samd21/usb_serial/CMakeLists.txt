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

project(atmelsam_usb_echo)

set(UHAL_ENABLE_USB_SERIAL_MODULE ON)
set(UHAL_USB_NO_OS ON)

add_subdirectory(./../../../ Universal_hal)

add_executable(usb_serial.elf "src/main.c" ${STARTUP_SCRIPT_SOURCES})
target_link_libraries(usb_serial.elf PUBLIC Universal_hal Universal_hal_usb)


target_sources(Universal_hal_usb_conf INTERFACE "src/usb_descriptors.c")
target_include_directories(Universal_hal_usb_conf INTERFACE src/)

