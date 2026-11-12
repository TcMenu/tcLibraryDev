idf_component_register(
    SRCS
        "main_main.cpp" "ESP32NativeProject_main.cpp" "ESP32NativeProject_menu.cpp"
    INCLUDE_DIRS
        "."
    REQUIRES
        freertos
        esp_timer
        log
        driver
        TaskManagerIO IoAbstraction tcUnicodeHelper tcMenu TcMenuLog Adafruit-GFX-mbed-fork
)