## ESP32 IDF support

In this folder you can find the ESP32 IDF support. It is based on the ESP-IDF 5 with Cmake support.

There are two examples:

* `tcmenu_idf_esp32` which is a freshly started project using the ESP-IDF 5 framework directly. It demonstrates how to use tcMenu with the ESP-IDF without Arduino.
* `tcmenu_arduino_esp32` which is based on the IDF Arduino component `hello_world` example with a menu added to it.

## Setting up the components so ESP32-IDF can see them (all cases)

Within an ESP32 project there is a directory called `components`. For TcMenu to be available within the ESP-IDF project, the TcMenu library components should be made available within the `components` folder. You do this using the python script in this folder as follows: 

`generateCmake.py -l libDirectory -o componentsDirectory -m arduino|idf`

* `libDirectory` is the directory containing the actual TcMenu libraries.
* `componentsDirectory` is the directory where the components should be created. Normally `components` within the project folder.
* `arduino|idf` is the framework to use. `idf` for ESP-IDF direct. `arduino` for Arduino as an ESP-IDF component, 

I typically run the above script in both the `cmakeProject/lib` and `cmakeProject/mbed_libs` folders so I get the core
libraries and the extra libraries intended for outside Arduino projects..

## For ESP-IDF without Arduino.

In the CMake configuration ensure the build flag `BUILD_FOR_ESP32_IDF` is enabled. I normally do this as follows in the top level `CMakeLists.txt` near the top:

    if(NOT DEFINED BUILD_FOR_ESP32_IDF)
        set(BUILD_FOR_ESP32_IDF ON CACHE BOOL "Build for ESP-IDF")
    endif()

This tells our frameworks that you're not using Arduino at all, and we'll fill in the gaps and provide the things needed for TcMenu directly.
