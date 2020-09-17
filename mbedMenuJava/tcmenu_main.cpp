#include <mbed.h>
#include "mbedMenuJava_menu.h"

I2C i2c(PF_0, PF_1);

BufferedSerial console(USBTX, USBRX);
MBedLogger LoggingPort(console);

void setup() {
    console.set_baud(115200);
    setupMenu();

}

int main() {
    setup();
    while(1) {
        taskManager.runLoop();
    }
}






void CALLBACK_FUNCTION onUserButton(int id) {
    // TODO - your menu change code
}
















void CALLBACK_FUNCTION onTakeDisplay(int id) {
    // TODO - your menu change code
}


















