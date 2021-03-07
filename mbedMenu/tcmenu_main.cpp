#include <mbed.h>
#include "mbedMenu_menu.h"

void setup() {
    setupMenu();
}

int main() {
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
