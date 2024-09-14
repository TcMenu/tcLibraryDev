
#include <IoAbstraction.h>
#include <PlatformDeterminationWire.h>

uint8_t toWrite = 0;

void checkAllI2CAddresses() {
    Serial.println("Starting scan");
    for(int i=0; i<128; i++) {
        if(ioaWireReady(defaultWireTypePtr, i)) {
            Serial.print("Device found on 0x");
            Serial.println(i, HEX);
        }
        else {
            Serial.print("Nothing at 0x");
            Serial.println(i, HEX);
        }
    }
    Serial.println("Finished scan");
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.println("Wire test starting");

    ioaWireBegin();

    Serial.println("Wire begin completed");

    taskManager.scheduleOnce(100, checkAllI2CAddresses);
    taskManager.scheduleFixedRate(1000, [] {
        toWrite = toWrite!=0 ? 0 : 0xff;
        ioaWireWriteWithRetry(defaultWireTypePtr, 0x20, &toWrite, 1);
    });
}

void loop() {
    taskManager.runLoop();
}