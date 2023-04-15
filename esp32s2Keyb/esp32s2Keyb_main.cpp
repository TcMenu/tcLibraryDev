//
// ESP32 S2 example based on Saola board with a dashboard configuration onto an OLED display
// I2C on standard pin, 8 and 9 with an SH1106 display
// encoder on 5, 6 with button on 7
// Getting started: https://www.thecoderscorner.com/products/arduino-libraries/tc-menu/tcmenu-overview-quick-start/
//

#include "generated/esp32s2Keyb_menu.h"
#include <PlatformDetermination.h>

#include <TaskManagerIO.h>
#include <EEPROM.h>
#include <tcMenuVersion.h>
#include <stockIcons/wifiAndConnectionIcons16x12.h>
#include <tcUtil.h>
#include "stockIcons/directionalIcons.h"

#define MENU_WIFIMODE_STATION 0
const char pgmsListHeader[] PROGMEM = "List items";

// first we need to define both a left and right button, we use the ones from stockIcons/directionalIcons.h
DrawableIcon iconLeft(-1, Coord(11, 22), tcgfx::DrawableIcon::ICON_XBITMAP, ArrowHoriz11x22BitmapLeft, nullptr);
DrawableIcon iconRight(-1, Coord(11, 22), tcgfx::DrawableIcon::ICON_XBITMAP, ArrowHoriz11x22BitmapRight, nullptr);

void setup() {
    // before proceeding we must start wire and serial, then call setup menu.
    Serial.begin(115200);
    serdebugF("Starting ESP32-S2 keyboard");
    Wire.begin();
    Wire.setClock(1000000);
    EEPROM.begin(512);

    setupMenu();

    // always call load after setupMenu, as the EEPROM you chose in initialised only after this setupMenu()
    menuMgr.load();

    // set the number of rows in the list.
    menuExtrasMyList.setNumberOfRows(42);

    // lastly we capture when the root title is pressed present a standard version dialog.
    setTitlePressedCallback([](int titleCb) {
        showVersionDialog(&applicationInfo);
    });

    // for the speed menu enable card layout
    renderer.enableCardLayout(iconLeft, iconRight, nullptr, true);
    renderer.setCardLayoutStatusForSubMenu(nullptr, false);
    renderer.setCardLayoutStatusForSubMenu(&menuSpeed, true);
}

void loop() {
    taskManager.runLoop();
}


// The below link discusses list items in detail
// https://www.thecoderscorner.com/products/arduino-libraries/tc-menu/menu-item-types/list-menu-item/
int CALLBACK_FUNCTION fnExtrasMyListRtCall(RuntimeMenuItem *item, uint8_t row, RenderFnMode mode, char *buffer, int bufferSize) {
    switch (mode) {
        case RENDERFN_INVOKE:
        serdebugF2("Selection of ", row);
            return true;
        case RENDERFN_NAME:
            if (row == LIST_PARENT_ITEM_POS) {
                safeProgCpy(buffer, pgmsListHeader, bufferSize);
            } else {
                strcpy(buffer, "Row");
                fastltoa(buffer, row, 2, NOT_PADDED, bufferSize);
            }
            return true;
        case RENDERFN_VALUE:
            if (row == LIST_PARENT_ITEM_POS) {
                buffer[0] = 0;
            } else {
                strcpy(buffer, "Val");
                fastltoa(buffer, row, 2, NOT_PADDED, bufferSize);
            }
            return true;
        case RENDERFN_EEPROM_POS:
            return 0xffffU; // lists are generally not saved to EEPROM
        default:
            return false;
    }
}

void CALLBACK_FUNCTION pressMeActionRun(int id) {
    menuMgr.save();
    EEPROM.commit();
    auto dlg = renderer.getDialog();
    if (!dlg->isInUse()) {
        dlg->setButtons(BTNTYPE_NONE, BTNTYPE_CLOSE);
        dlg->showRam("Saved", false);
        dlg->copyIntoBuffer("to flash");
    }
}

void CALLBACK_FUNCTION onSpeed78(int id) {
    menuMgr.activateMenuItem(&menu33);
}

void CALLBACK_FUNCTION onSpeed33(int id) {
    menu45.setVisible(false);
    menuMgr.notifyStructureChanged();
}


void CALLBACK_FUNCTION onSpeed45(int id) {
    // TODO - your menu change code
}
