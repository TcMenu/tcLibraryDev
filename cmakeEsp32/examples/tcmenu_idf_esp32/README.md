# tcMenu ESP32 ESP-IDF Example

This is a pure ESP-IDF example for tcMenu on ESP32.

It does not use the Arduino ESP32 component.

### Manual debuggging

There's a lot to be said for STM32/ST-Link, it is much easier to use. I cannot reliably debug ESP32 with OpenOCD within an IDE directly. I.E. it works one week and works again the next.

My workaround (should work in CLion and Code): In an ESP32 command window, run the following command:

    openocd.exe -f board/esp32c3-builtin.cfg -c "adapter speed 10000" -c "gdb_memory_map disable" -c "gdb_flash_program disable" -c "gdb_port 3333" -c "tcl_port disabled" -c "telnet_port 4444"

Then once the command is running, in CLion or Code, connect to the GDB server at localhost:3333.
