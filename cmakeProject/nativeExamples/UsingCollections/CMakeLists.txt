cmake_minimum_required(VERSION 3.13)

project(UsingCollections C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(UsingCollections
        main.cpp
)

target_compile_definitions(UsingCollections PUBLIC BUILD_FOR_PICO_CMAKE)

pico_add_extra_outputs(UsingCollections)

target_link_libraries(UsingCollections pico_stdlib pico_sync SimpleCollections)