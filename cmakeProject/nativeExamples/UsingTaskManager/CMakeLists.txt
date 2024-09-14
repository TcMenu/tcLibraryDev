cmake_minimum_required(VERSION 3.13)

project(UsingTaskManager C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(UsingTaskManager
        main.cpp
)

target_compile_definitions(UsingTaskManager PUBLIC BUILD_FOR_PICO_CMAKE)

pico_add_extra_outputs(UsingTaskManager)

target_link_libraries(UsingTaskManager pico_stdlib pico_sync TaskManagerIO)
