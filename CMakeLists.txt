cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(test_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_executable(dr-robo
  dr-robo.c
)
pico_enable_stdio_usb(dr-robo 1)
pico_enable_stdio_uart(dr-robo 1)
pico_add_extra_outputs(dr-robo)
target_link_libraries(dr-robo pico_stdlib hardware_pwm pico_time hardware_irq)
