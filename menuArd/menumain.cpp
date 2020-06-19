#include "mbed.h"
#include "menuArd.ino"

// Enable logging of debug statements to USB serial.
// This conincides with the enablement of the logging flag, see IoLogging.h
Serial console(USBTX, USBRX);
MBedLogger LoggingPort(console);

int main() {
    console.baud(115200);
    setup();
    while(1) {
        loop();
    }
    return 0;
}
