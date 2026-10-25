cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED BUILD_FOR_ESP32_IDF)
    set(BUILD_FOR_ESP32_IDF ON CACHE BOOL "Build for ESP-IDF")
endif()

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

idf_build_set_property(IO_LOGGING_DEBUG 1)
idf_build_set_property(MINIMAL_BUILD ON)
idf_build_set_property(COMPILE_DEFINITIONS "-DIO_LOGGING_DEBUG=1" APPEND)
idf_build_set_property(COMPILE_DEFINITIONS "-DBUILD_FOR_ESP32_IDF=1" APPEND)

project(tcmenu_esp32_idf)