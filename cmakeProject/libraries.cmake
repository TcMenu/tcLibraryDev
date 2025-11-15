cmake_minimum_required(VERSION 3.13)

# Add option to choose library source, ON means you pull libraries into lib/mbed_libs, OFF - auto fetch
option(USE_LOCAL_LIBRARIES "Use local libraries from lib/ directory instead of FetchContent" ON)

function (includeLibraries)
    if(USE_LOCAL_LIBRARIES)
        message(STATUS "Using local library sources from lib/ directory")

        # Use local libraries
        add_subdirectory(${CMAKE_SOURCE_DIR}/lib/TaskManagerIO/cmake TaskManagerIO)
        add_subdirectory(${CMAKE_SOURCE_DIR}/lib/IoAbstraction/cmake IoAbstraction)
        add_subdirectory(${CMAKE_SOURCE_DIR}/lib/SimpleCollections/cmake SimpleCollections)
        add_subdirectory(${CMAKE_SOURCE_DIR}/lib/tcMenu/cmake tcMenuLib)
        add_subdirectory(${CMAKE_SOURCE_DIR}/lib/TcMenuLog/cmake TcMenuLog)
        add_subdirectory(${CMAKE_SOURCE_DIR}/lib/tcUnicodeHelper/cmake TcUnicodeHelper)
        add_subdirectory(${CMAKE_SOURCE_DIR}/lib/LiquidCrystalIO/cmake LiquidCrystalIO)

        add_subdirectory(${CMAKE_SOURCE_DIR}/mbed_libs/Adafruit-GFX-mbed-fork/cmake AdafruitGFX)
    else()
        message(STATUS "Auto fetch TcLibs into _deps folder, to use local libs set: -DUSE_LOCAL_LIBRARIES=ON ")

        include(FetchContent)

        #  Include these libraries in CMakeLists.txt
        # tcMenu
        # IoAbstraction
        # TaskManagerIO
        # tcUnicodeHelper
        # TcMenuLog
        # AdafruitGFXNativePort
        # SimpleCollections
        # LiquidCrystalIO


        SET(TCMENU_BASE_GIT_REPO "https://github.com/TcMenu")

        # Fetch the TcMenu git repo
        FetchContent_Declare(
            tcMenuLib
            GIT_REPOSITORY ${TCMENU_BASE_GIT_REPO}/tcMenuLib.git
            GIT_TAG        main # Use a stable branch, tag, or commit hash
            SOURCE_SUBDIR  cmake
        )

        # Fetch the IoAbstraction git repo
        FetchContent_Declare(
            IoAbstraction
            GIT_REPOSITORY ${TCMENU_BASE_GIT_REPO}/IoAbstraction.git
            GIT_TAG        main # Use a stable branch, tag, or commit hash
            SOURCE_SUBDIR  cmake
        )

        # Fetch the TaskManagerIO git repo
        FetchContent_Declare(
            TaskManagerIO
            GIT_REPOSITORY ${TCMENU_BASE_GIT_REPO}/TaskManagerIO.git
            GIT_TAG        main # Use a stable branch, tag, or commit hash
            SOURCE_SUBDIR  cmake
        )

        # Fetch the TcUnicodeHelper git repo
        FetchContent_Declare(
            TcUnicodeHelper
            GIT_REPOSITORY ${TCMENU_BASE_GIT_REPO}/TcUnicodeHelper.git
            GIT_TAG        main # Use a stable branch, tag, or commit hash
            SOURCE_SUBDIR  cmake
        )

        # Fetch the TcMenuLog git repo
        FetchContent_Declare(
            TcMenuLog
            GIT_REPOSITORY ${TCMENU_BASE_GIT_REPO}/TcMenuLog.git
            GIT_TAG        main # Use a stable branch, tag, or commit hash
            SOURCE_SUBDIR  cmake
        )

        # Fetch the Adafruit-GFX-mbed-fork git repo
        FetchContent_Declare(
            AdafruitGFX
            GIT_REPOSITORY ${TCMENU_BASE_GIT_REPO}/Adafruit-GFX-mbed-fork.git
            GIT_TAG        main # Use a stable branch, tag, or commit hash
            SOURCE_SUBDIR  cmake
        )

        # Fetch the LiquidCrystalIO git repo
        FetchContent_Declare(
            LiquidCrystalIO
            GIT_REPOSITORY ${TCMENU_BASE_GIT_REPO}/LiquidCrystalIO.git
            GIT_TAG        main # Use a stable branch, tag, or commit hash
            SOURCE_SUBDIR  cmake
        )

        # Fetch the SimpleCollections git repo
        FetchContent_Declare(
            SimpleCollections
            GIT_REPOSITORY ${TCMENU_BASE_GIT_REPO}/SimpleCollections.git
            GIT_TAG        main # Use a stable branch, tag, or commit hash
            SOURCE_SUBDIR  cmake
        )

        FetchContent_MakeAvailable(tcMenuLib IoAbstraction TaskManagerIO TcUnicodeHelper TcMenuLog AdafruitGFX SimpleCollections LiquidCrystalIO)
    endif ()
endfunction()