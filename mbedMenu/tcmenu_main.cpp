#include <mbed.h>
#include "mbedMenu_menu.h"

BufferedSerial console(USBTX, USBRX);
MBedLogger LoggingPort(console);

void setup() {
    setupMenu();

    taskManager.scheduleFixedRate(100, [] {
        menuAnalogValue.setCurrentValue(rand() % 90);
        menuABoolean.setBoolean(rand()%2);
        menuA0Value.setFloatValue(rand()%100 / 100.F);
    });
}

int main() {
    console.set_baud(115200);
    setup();
    while(1) {
        taskManager.runLoop();
    }
}

void CALLBACK_FUNCTION onAnalogChange(int id) {
    // TODO - your menu change code
}

void CALLBACK_FUNCTION onUserButton(int id) {
    // TODO - your menu change code
}
