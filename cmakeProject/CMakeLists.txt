set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.26)

option(BUILD_NATIVE_TESTS "Build native unit tests using Unity" OFF)

IF (BUILD_NATIVE_TESTS)
    message("Unit tests using native build")
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_STANDARD 20)
    enable_testing()

    function(get_unity_if_needed)
        if (NOT TARGET unity)
            message(STATUS "fetching unity dependency")
            include(FetchContent)
            FetchContent_Declare(
                    unity
                    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
                    GIT_TAG        v2.5.2
            )
            FetchContent_MakeAvailable(unity)
        endif ()
    endfunction()
ELSE ()
    message("Building for PicoSDK")
    include(pico_sdk_import.cmake)
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_STANDARD 17)
    pico_sdk_init()
ENDIF ()

project(tccLibSdk)


include(libraries.cmake)
includeLibraries()

#IF (NOT DEFINED "ENV{TC_CMAKE_EXCLUDE_EXAMPLES}" AND NOT BUILD_NATIVE_TESTS)
#    add_subdirectory(nativeExamples/UsingTaskManager)
#    add_subdirectory(nativeExamples/UsingIoAbstraction)
#    add_subdirectory(nativeExamples/Using1602I2C)
#    add_subdirectory(nativeExamples/UsingSSD1306Oled)
#    add_subdirectory(nativeExamples/ControlAppOledEncoderPico)
#    add_subdirectory(nativeExamples/ControlAppOledKeyboardWifi)
#    add_subdirectory(nativeExamples/UsingMatrixKeyboard)
#    add_subdirectory(nativeExamples/UnicodeHandlerTest)
#ENDIF ()
#
