
# TcMenu > Arduino > ESP32 IDF

This is an example of how to use TcMenu with ESP-IDF and the Arduino component. It is based on the [hello_world](https://github.com/espressif/arduino-esp32/tree/master/examples/hello_world) 
example. You can find more information about [Arduino-esp32 and the build system](https://github.com/espressif/arduino-esp32).

This example is part of the [TcMenu Arduino Framework](https://github.com/TcMenu). You can download the
[TcMenu Designer UI](https://github.com/TcMenu/tcMenu/releases) to create your own menu, the designer can non-destructively 
update this example or your own project.

## What was this built for?

This example was built with the following hardware:

* SH1106 128x64 OLED screen
* Regular rotary encoder
* ESP32-S2 but any ESP32 should work if you run `menuconfig` and change the board.

## How to build

Assuming that you have a project already, or you are building this one..

Before any other step, you need to run the `generateCmake.py` from the root cmakeEsp32 folder. This will generate the 
`CMakeLists.txt` file for each library of the project into the components directory, setting up our libraries.

Example where `TC_LIBRARY_DEV_PATH` is the path to the tcMenu library dev project:

    cd myProjectDir 
    generateCmake.py -l ${TC_LIBRARY_DEV_PATH}/cmakeProject/lib -o ./components
    generateCmake.py -l ${TC_LIBRARY_DEV_PATH}/cmakeProject/mbed_libs -o ./components

## Preparing the project to build

Now setup the project to build with the ESP-IDF build system. Follow the regular ESP-IDF instructions for building
and flashing.

## The rest of this README is copied from the hello_world example

## Example folder contents

The project **hello_world** contains one source file in C++ language [main.cpp](main/main.cpp). The file is located in folder [main](main).

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both).

Below is short explanation of remaining files in the project folder.

```
├── CMakeLists.txt
├── main
│   ├── CMakeLists.txt
│   ├── idf_component.yml
│   └── main.cpp
└── README.md                  This is the file you are currently reading
```

## How to add Arduino libraries

In the project create folder `components/` and clone the library there.
In the library folder create new CMakeLists.txt file, add lines shown below to the file and edit the SRCS to match the library source files.

```
idf_component_register(SRCS "user_library.cpp" "another_source.c"
                      INCLUDE_DIRS "."
                      REQUIRES arduino-esp32
                      )
```

Below is structure of the project folder with the Arduino libraries.

```
├── CMakeLists.txt
├── components
│   ├── user_library
│   │   ├── CMakeLists.txt     This needs to be added
│   │   ├── ...
├── main
│   ├── CMakeLists.txt
│   ├── idf_component.yml
│   └── main.cpp
└── README.md                  This is the file you are currently reading
```
