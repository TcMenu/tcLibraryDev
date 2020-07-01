#include "menuArd_menu.h"
#include "SimhubConnector.h"
#include <IoAbstractionWire.h>

IoAbstractionRef io23017 = ioFrom23017(0x20, ACTIVE_LOW_OPEN, 2);
SimhubConnector connector(&Serial, 4);

void setup() {
    Serial.begin(115200);
    Serial.println("Starting...");
    Wire.begin();
    setupMenu();

    taskManager.scheduleFixedRate(1, &connector);

}

void loop() {
    taskManager.runLoop();

}

