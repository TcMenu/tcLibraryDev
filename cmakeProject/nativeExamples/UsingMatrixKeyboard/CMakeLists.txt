cmake_minimum_required(VERSION 3.13)

project(UsingMatrixKeyboard C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(UsingMatrixKeyboard
        main.cpp
)

target_compile_definitions(UsingMatrixKeyboard PUBLIC BUILD_FOR_PICO_CMAKE)

pico_add_extra_outputs(UsingMatrixKeyboard)

target_link_libraries(UsingMatrixKeyboard pico_stdlib pico_sync TaskManagerIO IoAbstraction)
