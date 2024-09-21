## Pico SDK build via CMake

Use this directory to build using CMake for PicoSDK. You simply point a cmake compatible IDE at this directory and it should be able to load the project structure. Disclaimer only tried with CLion.

To use you'll need to set the following variables.

## The Coders Corner libraries

TcMenu organisation made this library available for you to use. It takes significant effort to keep all our libraries current and working on a wide range of boards. Please consider making at least a one off donation via the sponsor button if you find it useful. In forks, please keep text to here intact.

## License

This build is Apache license. Consult each library for their license. 

## Using our libraries in Arduino and PlatformIO

Use the top level platformio.ini file for that purpose.

## Working with Native tool chains in production

_Commercial users: Before asking any questions in the tcMenu discussion board about this project, please make a donation commensurate with your companies funds_. This configuration will not be maintained unless we get enough donations.

We can support this toolchain on RP2040 PicoSDK, ESP-IDF, STM32Cube, Atmel AVR and SAMD. Please the the above link for more information.

Environment variables needed for PicoSDK:

    PICO_SDK_PATH=<your picosdk path>
    PICO_TOOLCHAIN_PATH=<path of arm toolchain>

## How to use these libraries

Here are the links to the libraries needed in the lib directory

* https://github.com/TcMenu/TaskManagerIO
* https://github.com/TcMenu/tcMenuLib
* https://github.com/TcMenu/IoAbstraction
* https://github.com/TcMenu/tcUnicodeHelper
* https://github.com/TcMenu/SimpleCollections
* https://github.com/TcMenu/LiquidCrystalIO
* https://github.com/TcMenu/TcMenuLog

Recommended libraries in cmakeProject/mbed_lib

* https://github.com/TcMenu/Adafruit-GFX-mbed-fork provides an Adafruit_GFX compatibile graphics library.

### Using in PicoSDK

As per all other PicoSDK applications, you need to set up the environment variables and ensure these libraries are available in the path. For the initial MVP we assume you have a local to project copy of these libraries similar to how the examples folder works, we'll sort this out properly after 4.2 and mobile app is released.

# Current state of play for direct pico-sdk / outside Arduino use

## Working

* IoAbstraction rotary encoders and switches without interrupts
* Wire I2C interface with IoAbstraction for PicoSDK
* SPI interface within IoAbstraction for PicoSDK
* TaskManagerIO fully working with PicoSDK
* SimpleCollections fully working with PicoSDK
* SCCircular buffer tested with interrupts on PicoSDK.
* Unicode helper library fully working with PicoSDK
* Liquid Crystal including I2C backpack is working
* SPI Wrapper is working
* Adafruit Fork I2C OLED working with PicoSDK
* SPI Adafruit fork test with PicoSDK
* PWM and Analog Input are working properly.

## To Test

* Check that AT24 EEPROMs are working properly.
* The copy script and setup of the libraries.

## Todo short term

* Test rotary encoders and switches with interrupts not working on PicoSDK
* Test rotary encoders/switches/LCD on other than 8574 IoExpanders and with interrupts.
* USB remote support for PicoSDK
* WiFi/LwIP remote support for PicoSDK
* Adafruit Fork I2C/SPI retest with mbed.
* Better way to create fonts
