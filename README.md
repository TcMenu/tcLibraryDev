[![Native C++ Tests](https://github.com/TcMenu/tcLibraryDev/actions/workflows/native-tests.yml/badge.svg)](https://github.com/TcMenu/tcLibraryDev/actions/workflows/native-tests.yml)

# TcMenu and IoAbstraction IDE/toolchain project

## Summary

## Summary

This project is the developer start point to work with TcMenu in a professional environment, and it has four main modes of operation:

1. **Local library development**, for those who prefer to have the library source local (often using PlatformIO).
2. **Pico SDK for CMake** using local libraries. [README.md](cmakeProject/README.md)
3. **Pico SDK for CMake** using auto-downloaded libraries. [README.md](cmakeProject/README.md)
4. **ESP32 CMake SDK** based setup. [cmakeEsp32](cmakeEsp32/README.md)
5. **STM32 Examples** require local libraries so do this first and then see https://github.com/TcMenu/stm32-examples for the rest.

You can choose to use this shell directly, or take inspiration from it and roll your own core project. To use with PlatformIO simply open the top-level directory in a suitable IDE, as it has a `platformio.ini`. To use with CMake, simply open the `cmakeProject` or `cmakeEsp32` directory with a suitable IDE.

Although this is mainly aimed at users and developers of tcMenu based applications, it also serves as a getting started point for you to develop source and build both libraries within an IDE using platformIO or PicoSDK Cmake.

## Library Setup

NOTE: For PicoSDK, you have two options, you can either install the libraries as shown below, or you can set build `USE_LOCAL_LIBRARIES` to `OFF` and CMake will automatically check out the libraries below. 

### Required libraries in `cmakeProject/lib`

* https://github.com/TcMenu/TaskManagerIO
* https://github.com/TcMenu/tcMenuLib
* https://github.com/TcMenu/IoAbstraction
* https://github.com/TcMenu/tcUnicodeHelper
* https://github.com/TcMenu/LiquidCrystalIO
* https://github.com/TcMenu/TcMenuLog

### Required libraries in `cmakeProject/mbed_lib`

* https://github.com/TcMenu/Adafruit-GFX-mbed-fork

### Commands to execute in order to prepare

From the `cmakeProject` directory

    mkdir lib
    cd lib
    git clone https://github.com/TcMenu/TaskManagerIO.git
    git clone https://github.com/TcMenu/IoAbstraction.git
    git clone https://github.com/TcMenu/tcMenuLib.git tcMenu
    git clone https://github.com/TcMenu/tcUnicodeHelper.git
    git clone https://github.com/TcMenu/LiquidCrystalIO.git
    git clone https://github.com/TcMenu/TcMenuLog.git

    cd ..
    mkdir mbed_lib
    git clone https://github.com/TcMenu/Adafruit-GFX-mbed-fork.git

## For use with platformIO

Just import the platformio.ini into your IDE and build.

## Unit testing

The unit testing runs natively on the host platform. We've made an IoAbstraction platform for it that minimally mocks GPIO, I2C and SPI.

To run the tests, in the top level `CMakeLists.txt` enable unit testing: `-DBUILD_NATIVE_TESTS=ON`. This the project to unit testing mode turning off other board specific code.

## PicoSDK Cmake specific notes

The [getting started guide is here](/cmakeProject/README.md).

When the local libraries flag is on `-DUSE_LOCAL_LIBRARIES=ON` it requires libraries be installed as above. 
When the local libraries flag is off `-DUSE_LOCAL_LIBRARIES=OFF` CMake will download the libraries.

## ESP32 IDF using CMake

The [getting started guide is here](/cmakeEsp32/README.md).
