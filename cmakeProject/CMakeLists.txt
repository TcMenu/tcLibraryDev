cmake_minimum_required(VERSION 3.26)

include(pico_sdk_import.cmake)

project(tccLibSdk)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(lib/TcMenuLog/cmake)
add_subdirectory(lib/SimpleCollections/cmake)
add_subdirectory(lib/TaskManagerIO/cmake)
add_subdirectory(lib/IoAbstraction/cmake)
add_subdirectory(lib/tcMenu/cmake)
add_subdirectory(lib/tcUnicodeHelper/cmake)
add_subdirectory(lib/LiquidCrystalIO/cmake)
add_subdirectory(mbed_lib/Adafruit-GFX-mbed-fork/cmake)

IF(NOT DEFINED "ENV{TC_CMAKE_EXCLUDE_EXAMPLES}")
    add_subdirectory(nativeExamples/UsingCollections)
    add_subdirectory(nativeExamples/UsingTaskManager)
    add_subdirectory(nativeExamples/UsingIoAbstraction)
    add_subdirectory(nativeExamples/Using1602I2C)
    add_subdirectory(nativeExamples/UsingSSD1306Oled)
    add_subdirectory(nativeExamples/ControlAppOledEncoderPico)
    add_subdirectory(nativeExamples/ControlAppOledKeyboardWifi)
    add_subdirectory(nativeExamples/UsingMatrixKeyboard)
    add_subdirectory(nativeExamples/UnicodeHandlerTest)
ENDIF ()