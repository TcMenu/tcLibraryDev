cmake_minimum_required(VERSION 3.13)

project(Using1602I2C C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(Using1602I2C
        main.cpp
)

target_compile_definitions(Using1602I2C PUBLIC BUILD_FOR_PICO_CMAKE)

pico_add_extra_outputs(Using1602I2C)

target_link_libraries(Using1602I2C
        pico_stdlib pico_sync hardware_i2c hardware_spi hardware_adc hardware_pwm
        IoAbstraction LiquidCrystalIO TaskManagerIO
)