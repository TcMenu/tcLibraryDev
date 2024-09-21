# TcMenu and IoAbstraction IDE/toolchain project

## Summary

This project is a shell that can be used to work with and build our libraries for either platformIo or PicoSDK with CMake. You can choose to use this, or take inspiration from it and roll your own core project. To use with platformIO simply open in a suitable IDE in the top level directory, it has a `platformio.ini`. To use with CMake, simply open the `cmakeProject` directory with a suitable IDE.

Although this is mainly aimed at users and developers of tcMenu based applications, it also serves as a getting started point for you to develop source and build both libraries within an IDE using platformIO or PicoSDK Cmake.

## To set up:

1. Clone this repo
2. Create a `lib` and `mbed_lib` directory under the `cmakeProject` directory (this makes it easy to host both options in one solution)
3. Clone `IoAbstraction`, `LiquidCrystalIO`, `TaskManagerIO`, `tcMenu`, `SimpleCollections`, `TcMenuLog` and `tcUnicodeHelper` into the `lib` directory
4. Clone `AdafruitGFXNativePort` into the `mbed_lib` directory 
5. Set up either `platformio.ini` with the right parameters for your board, or configure Cmake and import the `cmakeProject` directory into an IDE.

## Required libraries in `cmakeProject/lib`

* https://github.com/TcMenu/TaskManagerIO
* https://github.com/TcMenu/tcMenuLib
* https://github.com/TcMenu/IoAbstraction
* https://github.com/TcMenu/tcUnicodeHelper
* https://github.com/TcMenu/SimpleCollections
* https://github.com/TcMenu/LiquidCrystalIO
* https://github.com/TcMenu/TcMenuLog

## Required libraries in `cmakeProject/mbed_lib`

* https://github.com/TcMenu/Adafruit-GFX-mbed-fork

## PicoSDK Cmake specific notes

The [getting started guide is here](/cmakeProject/README.md).
