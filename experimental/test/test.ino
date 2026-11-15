#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <tcMenu.h>
#include <IoAbstraction.h>

// Initialise the display object
TFT_eSPI tft = TFT_eSPI();

void setup() {
    // Start serial for easy release smoke-testing
    Serial.begin(115200);

    // Wait up to 2 seconds for serial port (helpful on Teensy cold boots)
    uint32_t startTime = millis();
    while (!Serial && (millis() - startTime < 2000));

    Serial.println("Teensy 4.0 Booting Up...");

    // Initialise the ILI9341 Screen via TFT_eSPI
    tft.init();
    tft.setRotation(1); // Landscape mode
    tft.fillScreen(TFT_BLACK);

    // Draw a quick hardware verification frame
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawRoundRect(10, 10, 300, 220, 8, TFT_BLUE);
    tft.drawCentreString("Teensy 4.0 Ready", 160, 100, 4);
    tft.drawCentreString("tcMenu / PlatformIO Environment OK", 160, 140, 2);

    Serial.println("Hardware test complete. Screen should now be active.");
}

void loop() {
    // tcMenu's task manager handles execution loops seamlessly
    taskManager.runLoop();
}
