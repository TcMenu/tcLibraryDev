#include "ESP32NativeProject_menu.h"
#include <PlatformDetermination.h>
#include <TaskManagerIO.h>





// Declaring any arrays used by enum/list items
const char* strChoicesEnumEntries[] = { "Spanner", "Hammer", "Router" };

void buildMenu(TcMenuBuilder& builder) {
    builder.usingDynamicEEPROMStorage()
        .analogBuilder(MENU_COUNT_ID, "Count", DONT_SAVE, MenuFlags().readOnly(), 0, nullptr)
            .offset(0).divisor(1).step(1).maxValue(255).unit("tms").endItem()
        .actionItem(MENU_ADD_ID, "Add", NoMenuFlags, tcAdded_onAddToCount)
        .subMenu(MENU_VALUE_ITEMS_ID, "Value Items", NoMenuFlags, nullptr)
            .analogBuilder(MENU_TENTHS_ID, "Tenths", ROM_SAVE, NoMenuFlags, 0, onTenthsChanged)
                .offset(0).divisor(10).step(1).maxValue(1000).unit("").endItem()
            .enumItem(MENU_CHOICES_ID, "Choices", ROM_SAVE, strChoicesEnumEntries, 3, NoMenuFlags, 0, onChoicesChanged)
            .boolItem(MENU_YES_OR_NO_ID, "Yes Or No", ROM_SAVE, NAMING_YES_NO, NoMenuFlags, false, nullptr)
            .endSub()
        .subMenu(MENU_RUNTIME_ITEMS_ID, "Runtime Items", NoMenuFlags, nullptr)
            .largeNumberItem(MENU_LARGE_VE_ID, "Large +ve", ROM_SAVE, LargeFixedNumber(6, 3, 0U, 0U, false), false, NoMenuFlags, nullptr)
            .textItem(MENU_TEXT_ID, "Text", ROM_SAVE, 10, NoMenuFlags, "", nullptr)
            .timeItem(MENU_TIME_ITEM_ID, "Time Item", DONT_SAVE, NoMenuFlags, EDITMODE_TIME_24H, TimeStorage(0, 0, 0, 0), nullptr)
            .dateItem(MENU_DATE_ITEM_ID, "Date Item", DONT_SAVE, NoMenuFlags, DateStorage(1, 1, 2020), nullptr)
            .endSub();
}



void setup() {
    setupMenu();

}

/*int main() {
    setup();
    while(1) {
        taskManager.runLoop();
    }
}*/


void CALLBACK_FUNCTION tcAdded_onAddToCount(int id) {
    getMenuCount().setCurrentValue(getMenuCount().getCurrentValue() + 1); // Added by generator for the count example
}


void CALLBACK_FUNCTION onTenthsChanged(int id) {
    // TODO - your menu change code
}


void CALLBACK_FUNCTION onChoicesChanged(int id) {
    // TODO - your menu change code
}
