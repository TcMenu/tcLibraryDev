cmake_minimum_required(VERSION 3.13)

project(ControlAppOledEncoderPico C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(ControlAppOledEncoderPico
        generated/ControlAppOledEncoderPico_menu.cpp
        generated/tcMenuAdaFruitGfxMono.cpp
        tcmenu_main.cpp
        appTheme.cpp
        generated/StdioTransport.h
        generated/StdioTransport.cpp
)

pico_enable_stdio_usb(ControlAppOledEncoderPico 1)
pico_enable_stdio_uart(ControlAppOledEncoderPico 0)

target_include_directories(ControlAppOledEncoderPico PUBLIC ${CMAKE_CURRENT_LIST_DIR})

pico_add_extra_outputs(ControlAppOledEncoderPico)

target_link_libraries(
        ControlAppOledEncoderPico
        pico_stdlib pico_sync hardware_i2c hardware_spi hardware_adc hardware_pwm
        pico_unique_id
        SimpleCollections TaskManagerIO IoAbstraction tcMenu tcUnicodeHelper AdafruitGFXNativePort
)
