# TcMenu and IoAbstraction development and test project

## Summary

This project is a shell that can be used to work with and build our libraries for either platformIo or PicoSDK with CMake. You can choose to use this, or take inspiration from it and roll your own core project.

Although mainly aimed at developers of tcMenu organisation and people building with our libraries, it also serves as a getting started point for you to develop source and build both libraries within an IDE using platformIO or PicoSDK Cmake.

## To set up:

1. Clone this repo
2. Create a `lib` and `mbed_lib` directory at the top level
3. Clone `IoAbstraction`, `LiquidCrystalIO`, `TaskManagerIO`, `tcMenu`, `SimpleCollections`, `TcMenuLog` and `tcUnicodeHelper` into the `lib` directory
4. Clone `AdafruitGFXNativePort` into the `mbed_lib` directory 
5. Set up either `platformio.ini` with the right parameters for your board, or configure Cmake and import the `cmakeProject` directory into an IDE.

## PicoSDK Cmake specific notes

See the readme file in the cmakeProject directory.