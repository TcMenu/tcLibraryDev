cmake_minimum_required(VERSION 3.13)

function (includeLibraries)
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

endfunction()