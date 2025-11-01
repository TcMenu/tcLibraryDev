/**
* This example presents some ESP32 specific information onto an OLED screen using Wire. The example assumes you're using
* an ESP32S2 with I2C OLED and an encoder on pins 5,6,7.
*
* This sketch was built using TcMenu Designer UI. You can use round trip adjustments using that UI.
* See https://tcmenu.github.io/documentation/arduino-libraries/tc-menu/
*/

#include "main_menu.h"
#include <Wire.h>
#include <EEPROM.h>
#include "esp_chip_info.h"
#include "esp_flash.h"

#define  ONE_MB_BYTES (1024.0F*1024.0F)

// Forward references
void acquireSystemInfo();

void setup() {
    // before proceeding, we must start wire and serial, then call setup menu.
    Serial.begin(115200);
    serdebugF("Starting ESP32-S2 example");
    Wire.begin();
    Wire.setClock(1000000);
    EEPROM.begin(512);
    serdebugF("Start menu init");

    // this will start up the menu, initialise input and display etc.
    setupMenu();

    serdebugF("Running app");

    // we capture when the title menu item is pressed present a standard version dialog.
    setTitlePressedCallback([](int titleCb) {
        showVersionDialog(&applicationInfo);
    });

    // schedule a task that will run twice a second
    taskManager.schedule(repeatMillis(500), []() {
        // set the currently available heap
        menuBoardInfoHeapAvl.setCurrentValue(esp_get_minimum_free_heap_size() / 1024);
        // update the up time
        menuUpTime.setFloatValue(menuUpTime.getFloatValue() + 0.5F);
    });

    // and fill in the system info menu
    acquireSystemInfo();
}

void loop() {
    taskManager.runLoop();
}

void acquireSystemInfo() {
    esp_chip_info_t chipInfo;
    uint32_t flashSize;
    esp_chip_info(&chipInfo);

    menuBoardInfoCores.setCurrentValue(chipInfo.cores);

    if(esp_flash_get_size(nullptr, &flashSize) == ESP_OK) {
        menuBoardInfoFlash.setFromFloatingPointValue(static_cast<float>(flashSize) / ONE_MB_BYTES);
    }

    char features[32] = {0};
    if(chipInfo.features & CHIP_FEATURE_WIFI_BGN) strcat(features, "WiFi/");
    if(chipInfo.features & CHIP_FEATURE_BT) strcat(features, "BT");
    if(chipInfo.features & CHIP_FEATURE_BLE) strcat(features, "BLE");
    menuBoardInfoFeatures.setTextValue(features);
}

// When the press me action item is selected/clicked in the menu then run this callback, which presents a menu dialog
void CALLBACK_FUNCTION onPressed(int id) {
    withMenuDialogIfAvailable([](MenuBasedDialog * dlg) {
        dlg->setButtons(BTNTYPE_CLOSE, BTNTYPE_NONE, 0);
        dlg->copyIntoBuffer("World");
        dlg->show("Hello", false);
    });
}
