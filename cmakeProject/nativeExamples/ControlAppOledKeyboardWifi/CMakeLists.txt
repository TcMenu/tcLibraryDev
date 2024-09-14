cmake_minimum_required(VERSION 3.13)

project(ControlAppOledKeyboardWifi C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(ControlAppOledKeyboardWifi
        generated/tcMenuAdaFruitGfxMono.cpp
        generated/ControlAppOledKeyboardWifi_menu.cpp
        tcmenu_main.cpp
)
target_compile_definitions(ControlAppOledKeyboardWifi
        PUBLIC MBEDTLS_CIPHER_MODE_CBC=1
)

target_include_directories(ControlAppOledKeyboardWifi PUBLIC ${CMAKE_CURRENT_LIST_DIR})

pico_add_extra_outputs(ControlAppOledKeyboardWifi)

target_link_libraries(
        ControlAppOledKeyboardWifi
        pico_stdlib pico_sync hardware_i2c hardware_spi hardware_adc hardware_pwm
        pico_cyw43_arch_lwip_poll pico_mbedtls pico_lwip_mbedtls pico_unique_id
        SimpleCollections TaskManagerIO IoAbstraction tcMenu tcUnicodeHelper AdafruitGFXNativePort
)
