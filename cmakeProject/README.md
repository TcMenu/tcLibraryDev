# tcMenu CMake Embedded & Native Build Framework

[![Native C++ Tests](https://github.com/TcMenu/tcLibraryDev/actions/workflows/native-tests.yml/badge.svg)](https://github.com/TcMenu/tcLibraryDev/actions/workflows/native-tests.yml)

This directory provides CMake integration for building **tcMenu** and **The Coders Corner** libraries across embedded targets (**Raspberry Pi Pico / RP2040**, **ESP32 / ESP-IDF**, **STM32Cube**) and native host platforms.

---

### Supported Platforms & Frameworks

* **Raspberry Pi Pico (PicoSDK)**: Native CMake build with examples located in `nativeExamples/`.
* **ESP32 (ESP-IDF)**: See the [ESP32 CMake & ESP-IDF directory](https://github.com/TcMenu/tcLibraryDev/blob/main/cmakeEsp32/).
* **STM32Cube**: See our [STM32 CMake Examples](https://github.com/tcmenu/stm32-examples).
* **Arduino / PlatformIO**: Use the top-level `platformio.ini` file in the repository root.
* **Native Host (x86_64 / ARM64)**: Unit testing with Unity (`-DBUILD_NATIVE_TESTS=ON`).

---

### Prerequisites (PicoSDK)

Configure the required environment variables for your toolchain before running CMake:

**Linux / macOS:**
```bash
export PICO_SDK_PATH=/path/to/pico-sdk
export PICO_TOOLCHAIN_PATH=/path/to/arm-none-eabi-toolchain
```

**Windows (PowerShell):**
```powershell
$env:PICO_SDK_PATH = "C:\pico\pico-sdk"
$env:PICO_TOOLCHAIN_PATH = "C:\pico\arm-gnu-toolchain"
```

---

### Quick Start: Building for Raspberry Pi Pico

#### 1. Automatic Dependency Fetching Mode
To automatically download all library dependencies via CMake `FetchContent`, set `USE_LOCAL_LIBRARIES=OFF`:

```bash
# Configure and build
cmake -B build -DUSE_LOCAL_LIBRARIES=OFF
cmake --build build
```

Optional: To force fetching `picotool` from git during build, add `-DPICOTOOL_FORCE_FETCH_FROM_GIT=ON`.

#### 2. Local Libraries Mode
If you prefer managing local repository clones in `lib/` and `mbed_lib/` (the default mode `USE_LOCAL_LIBRARIES=ON`):

```bash
cmake -B build -DUSE_LOCAL_LIBRARIES=ON
cmake --build build
```

Required repositories for local mode:
* [TaskManagerIO](https://github.com/TcMenu/TaskManagerIO) (clone into `lib/TaskManagerIO`)
* [tcMenuLib](https://github.com/TcMenu/tcMenuLib) (clone into `lib/tcMenu`)
* [IoAbstraction](https://github.com/TcMenu/IoAbstraction) (clone into `lib/IoAbstraction`)
* [tcUnicodeHelper](https://github.com/TcMenu/tcUnicodeHelper) (clone into `lib/tcUnicodeHelper`)
* [LiquidCrystalIO](https://github.com/TcMenu/LiquidCrystalIO) (clone into `lib/LiquidCrystalIO`)
* [TcMenuLog](https://github.com/TcMenu/TcMenuLog) (clone into `lib/TcMenuLog`)
* [Adafruit-GFX-mbed-fork](https://github.com/TcMenu/Adafruit-GFX-mbed-fork) (clone into `mbed_lib/Adafruit-GFX-mbed-fork`)

---

### Included Examples

Examples in `nativeExamples/` are built automatically unless `-DTC_CMAKE_EXCLUDE_EXAMPLES=ON` is specified.

| Example | Target | Description / Peripherals |
| :--- | :--- | :--- |
| `UsingTaskManager` | Pico / RP2040 | TaskManagerIO scheduling and coroutines via Serial |
| `UsingIoAbstraction` | Pico / RP2040 | Multi-IO abstraction and digital pin management |
| `Using1602I2C` | Pico / RP2040 | LiquidCrystal 16x2 display over I2C |
| `UsingSSD1306Oled` | Pico / RP2040 | SSD1306 OLED display using Adafruit-GFX |
| `UsingMatrixKeyboard` | Pico / RP2040 | Matrix keypad input using IoAbstraction |
| `UnicodeHandlerTest` | Pico / RP2040 | tcUnicodeHelper UTF-8 rendering verification |
| `ControlAppOledEncoderPico` | Pico / RP2040 | Complete tcMenu application: SSD1306 OLED + Rotary Encoder |
| `ControlAppOledKeyboardWifi` | Pico W / RP2040 | Complete tcMenu application: SSD1306 OLED + Keypad + Wi-Fi |

---

### Integrating tcMenu into Your Own PicoSDK Project

1. Copy `libraries.cmake` into your project directory.
2. In your `CMakeLists.txt`, include and invoke `includeLibraries()` after `pico_sdk_init()`:

```cmake
include(libraries.cmake)
includeLibraries()

target_link_libraries(your_target_name
    tcMenu
    IoAbstraction
    TaskManagerIO
    tcUnicodeHelper
    TcMenuLog
    AdafruitGFXNativePort
    LiquidCrystalIO
)
```

---

### Running Native Unit Tests

You can build and run unit tests on your host machine without embedded hardware:

```bash
cmake -B build-tests -DBUILD_NATIVE_TESTS=ON
cmake --build build-tests
ctest --test-dir build-tests --output-on-failure
```

---

### Sponsorship & Support

TcMenu and its accompanying libraries are open-source under the Apache-2.0 license. Developing and maintaining these libraries across diverse hardware targets requires substantial effort. 

If you use these libraries in commercial products or need dedicated support, please consider sponsoring development via GitHub Sponsors or contacting [The Coders Corner](https://www.thecoderscorner.com).
