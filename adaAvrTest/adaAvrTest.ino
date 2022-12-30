#include "adaAvrTest_menu.h"

void setup() {
    Wire.begin();
    setupMenu();

}

void loop() {
    taskManager.runLoop();

}
