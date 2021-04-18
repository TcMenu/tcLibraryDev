
#include <IoAbstraction.h>
#include <PlatformDeterminationWire.h>

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.println("Wire test starting");

    ioaWireBegin();

    Serial.println("Wire begin completed");

    bool ready = ioaWireReady(defaultWireTypePtr, 0x20);

    Serial.print("wire ready "); Serial.println(ready);
}

void loop() {
    taskManager.runLoop();
}