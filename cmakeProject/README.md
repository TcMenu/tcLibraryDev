[![Native C++ Tests](https://github.com/TcMenu/tcLibraryDev/actions/workflows/native-tests.yml/badge.svg)](https://github.com/TcMenu/tcLibraryDev/actions/workflows/native-tests.yml)

## Pico SDK build via CMake

Use this directory to build using CMake for PicoSDK. You simply point a cmake compatible IDE at this directory and it should be able to load the project structure. Disclaimer only tried with CLion and VSCode with the
pico plugin and the cmake plugins respectivly.

To use you'll need to set the following variables.

## The Coders Corner libraries

TcMenu organisation made this library available for you to use. It takes significant effort to keep all our libraries current and working on a wide range of boards. Please consider making at least a one off donation via the sponsor button if you find it useful. In forks, please keep text to here intact.

## License

This build is Apache license. Consult each library for their license. 

## Using our libraries in Arduino and PlatformIO

Use the top level `platformio.ini` file for that purpose. 

## Working with Native tool chains in production

_Commercial users: Before asking any questions in the tcMenu discussion board about this project, please make a donation commensurate with your companies funds_. This configuration will not be maintained unless we get enough donations.

We can support this toolchain on RP2040 PicoSDK, ESP-IDF, STM32Cube, Atmel AVR and SAMD. Please the the above link for more information.

Environment variables needed for PicoSDK:
    ```
    PICO_SDK_PATH=<your picosdk path>
    PICO_TOOLCHAIN_PATH=<path of arm toolchain>
    ```

## How to use these libraries

CMake Variable `USE_LOCAL_LIBRARIES` defines if you want to check out libraries manually into the `lib` directory (I.E. you want to manage the library repos yourself. If this is not set, they will be pulled automatically.

Here are the links to the libraries needed in the lib directory

* https://github.com/TcMenu/TaskManagerIO
* https://github.com/TcMenu/tcMenuLib
* https://github.com/TcMenu/IoAbstraction
* https://github.com/TcMenu/tcUnicodeHelper
* https://github.com/TcMenu/LiquidCrystalIO
* https://github.com/TcMenu/TcMenuLog
* https://github.com/TcMenu/Adafruit-GFX-mbed-fork provides an Adafruit_GFX compatibile graphics library.

### Examples that are included

There are quite a few examples included in the `nativeExamples` directory. These are included in the CMake project by default unless you specify `TC_CMAKE_EXCLUDE_EXAMPLES` to exclude them.

* We also support [STM32Cube directly with CMake](https://github.com/tcmenu/stm32-examples) too
* Also see the [ESP32-IDF examples area](https://github.com/TcMenu/tcLibraryDev/blob/main/cmakeEsp32/)

### Using in PicoSDK

As per all other PicoSDK applications, you need to set up the environment variables and ensure these libraries are available in the path.

* Copy libraries.cmake to your project
* Add the following to your `CMakeLists.txt` after `pico_sdk_init()`
    ```
    include(libraries.cmake)
    includeLibraries()
    ```
* Add the following to your `CMakeLists.txt` in the `target_link_libraries`
    ```
    tcMenu
    IoAbstraction
    TaskManagerIO
    tcUnicodeHelper
    TcMenuLog
    AdafruitGFXNativePort
    ```
* Build.
* The libraries will be in `build/_deps`.

Here are my CMake options that I tend to use:

```
Environment variables:
PICO_SDK_PATH=$HOME\pico\pico-sdk;PICO_TOOLCHAIN_PATH=$HOME\pico\arm-gnu-toolchain-12.3.rel1-mingw-w64-i686-arm-none-eabi

Command options:
-DPICOTOOL_FORCE_FETCH_FROM_GIT=ON
```

## Running the unit tests

Add this to your build flags: `-DBUILD_NATIVE_TESTS=ON`, it turns off embedded toolchains and enables native tests.