#include "xioAlarmPanel_menu.h"
#include "tcMenuVersion.h"

void onTitlePressed() {
    withMenuDialogIfAvailable([] (MenuBasedDialog* dlg) {
        dlg->setButtons(BTNTYPE_CLOSE, BTNTYPE_NONE);
        dlg->show("Home Alarm", false);
        char sz[10];
        tccore::copyTcMenuVersion(sz, sizeof sz);
        dlg->copyIntoBuffer(sz);
    });
}

void setup() {
    setupMenu();

    setTitlePressedCallback([](int id) {
        onTitlePressed();
    });

    menuZoneHistory.setNumberOfRows(5);
    menuZones.setTextValue("123FE");
}

void loop() {
    taskManager.runLoop();
}


void CALLBACK_FUNCTION onZoneEnablement(int id) {
    // TODO - your menu change code
}

void CALLBACK_FUNCTION onEmergency(int id) {
    // TODO - your menu change code
}

// see tcMenu list documentation on thecoderscorner.com
int CALLBACK_FUNCTION fnZoneHistoryRtCall(RuntimeMenuItem* item, uint8_t row, RenderFnMode mode, char* buffer, int bufferSize) {
   switch(mode) {
    case RENDERFN_INVOKE:
        // TODO - your code to invoke goes here - row is the index of the item
        return true;
    case RENDERFN_NAME:
        if(row == LIST_PARENT_ITEM_POS) strncpy(buffer, "Zone History", bufferSize);
        else {
            ltoaClrBuff(buffer, row, 2, NOT_PADDED, bufferSize);
        }
        return true;
    case RENDERFN_VALUE:
        if(row == LIST_PARENT_ITEM_POS) strncpy(buffer, ">>", bufferSize);
        else {
            buffer[0] = 'H'; buffer[1]=0;
            fastltoa(buffer, row, 3, NOT_PADDED, bufferSize);
        }
        return true;
    case RENDERFN_EEPROM_POS: return 0xffff; // lists are generally not saved to EEPROM
    default: return false;
    }
}

void CALLBACK_FUNCTION onArmed(int id) {
    // TODO - your menu change code
}
