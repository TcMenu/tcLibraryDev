//
// Created by David Cherry on 27/06/2020.
//

#include <MenuItems.h>
#include <MenuIterator.h>
#include <RuntimeMenuItem.h>
#include "SimhubConnector.h"

void SimhubConnector::exec() {
    while(serialPort->available()) {
        if(linePosition >= MAX_LINE_WIDTH) {
            lineBuffer[MAX_LINE_WIDTH - 1] = 0;
            serdebugF2("Error occurred during Rx ", lineBuffer);
            linePosition = 0;
        }

        lineBuffer[linePosition] = serialPort->read();

        if(lineBuffer[linePosition] == '\n') {
            lineBuffer[linePosition] = 0;
            Serial.print("RX "); Serial.println(lineBuffer);
            // we have received a command from simhub
            processCommandFromSimhub();
            linePosition=0;
        }
        else linePosition++;
    }

}

void SimhubConnector::processCommandFromSimhub() {
    if(isDigit(lineBuffer[0])) {
        processTcMenuCommand();
    }
    else if(strcmp(lineBuffer, "simhubStart") == 0) {
        changeStatus(true);

    }
    else if(strcmp(lineBuffer, "simhubEnd") == 0) {
        changeStatus(false);
    }
}

void SimhubConnector::changeStatus(bool connected) {
    if(statusMenuId != -1) {
        BooleanMenuItem* item = reinterpret_cast<BooleanMenuItem*>(getMenuItemById(statusMenuId));
        if(item) item->setBoolean(connected);
    }
}

void SimhubConnector::processTcMenuCommand() {
    int i = 0;
    char sz[6];
    while(isDigit(lineBuffer[i]) && i < 5) {
        sz[i] = lineBuffer[i];
        i++;
    };
    sz[i] = 0;
    int menuId = atoi(sz);
    while(lineBuffer[i] != '=') i++;
    char* value = &lineBuffer[i + 1];

    serdebugF4("updch, item, value", sz, menuId, value);

    MenuItem* menuItem = getMenuItemById(menuId);
    if(menuItem->getMenuType() == MENUTYPE_INT_VALUE || menuItem->getMenuType() == MENUTYPE_ENUM_VALUE || menuItem->getMenuType() == MENUTYPE_BOOLEAN_VALUE) {
        ValueMenuItem* valItem = reinterpret_cast<ValueMenuItem*>(menuItem);
        valItem->setCurrentValue(atoi(value));
    }
    else if(menuItem->getMenuType() == MENUTYPE_TEXT_VALUE) {
        TextMenuItem* txtItem = reinterpret_cast<TextMenuItem*>(menuItem);
        txtItem->setTextValue(value);
    }
}
