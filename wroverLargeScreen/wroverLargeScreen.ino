#include "wroverLargeScreen_menu.h"

#define TFT_BACKLIGHT 5

void setup() {
    SPI.begin(19, 25, 23);
    SPI.setFrequency(20000000);
    Serial.begin(115200);
    pinMode(TFT_BACKLIGHT, OUTPUT);
    digitalWrite(TFT_BACKLIGHT, LOW);
    setupMenu();
}

void loop() {
    taskManager.runLoop();

}


void CALLBACK_FUNCTION onVolumeChanged(int id) {
    // TODO - your menu change code
}

void CALLBACK_FUNCTION onChannelChanged(int id) {
    // TODO - your menu change code
}

void CALLBACK_FUNCTION onTrimChange(int id) {
    // TODO - your menu change code
}

void CALLBACK_FUNCTION onSave(int id) {
    // TODO - your menu change code
}
