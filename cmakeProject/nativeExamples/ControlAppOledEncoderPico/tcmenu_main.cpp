#include "PicoSDKOled_menu.h"
#include "PlatformDeterminationWire.h"
#include <PlatformDetermination.h>
#include <TaskManagerIO.h>
#include <hardware/i2c.h>
#include "tcMenuVersion.h"

#define OLED_SPI_TX_PIN 3
#define OLED_SPI_RX_PIN 4
#define OLED_SPI_CLK_PIN 2
#define I2C_SDA 10
#define I2C_CLK 11



// Declaring any arrays used by enum/list items
const char* strChannelEnumEntries[] = { "Phono", "CD", "Aux" };
const char* strStateEnumEntries[] = { "Standby", "Warm Up", "Active", "Protect", "Overheat" };
const char* strDialogActiveEnumEntries[] = { "Item 0", "Item 1" };
const char* strEnableEnumEntries[] = { "Set 1", "Set 2" };
const char* strFlashListListItems[] = {  "Items1", "Items2" };

void buildMenu(TcMenuBuilder& builder) {
    builder.usingDynamicEEPROMStorage()
        .analogBuilder(MENU_VOLUME_ID, "Volume", DONT_SAVE, NoMenuFlags, 0, onVolumeChanged)
            .offset(-180).divisor(2).step(1).maxValue(255).unit("dB").endItem()
        .enumItem(MENU_CHANNEL_ID, "Channel", DONT_SAVE, strChannelEnumEntries, 3, NoMenuFlags, 0, nullptr)
        .boolItem(MENU_MUTE_ID, "Mute", DONT_SAVE, NAMING_ON_OFF, NoMenuFlags, false, nullptr)
        .boolItem(MENU_DIRECT_ID, "Direct", DONT_SAVE, NAMING_ON_OFF, NoMenuFlags, false, nullptr)
        .subMenu(MENU_STATUS_ID, "Status", NoMenuFlags, StatusAppeared)
            .analogBuilder(MENU_LEFT_VU_ID, "Left VU", DONT_SAVE, MenuFlags().readOnly(), 0, nullptr)
                .offset(-200).divisor(10).step(1).maxValue(250).unit("dB").endItem()
            .analogBuilder(MENU_RIGHT_VU_ID, "RightVU", DONT_SAVE, MenuFlags().readOnly(), 0, nullptr)
                .offset(-200).divisor(10).step(1).maxValue(250).unit("dB").endItem()
            .enumItem(MENU_STATE_ID, "State", DONT_SAVE, strStateEnumEntries, 5, MenuFlags().readOnly(), 0, nullptr)
            .floatItem(MENU_A0_LEVEL_ID, "A0 Level", DONT_SAVE, 3, MenuFlags().readOnly(), 0.0, nullptr)
            .endSub()
        .subMenu(MENU_RUNTIMES_ID, "Runtimes", NoMenuFlags, nullptr)
            .timeItem(MENU_TIME24_ID, "Time 24", DONT_SAVE, NoMenuFlags, EDITMODE_TIME_24H, TimeStorage(0, 0, 0, 0), nullptr)
            .timeItem(MENU_TIME12_ID, "Time 12", DONT_SAVE, NoMenuFlags, EDITMODE_TIME_12H, TimeStorage(0, 0, 0, 0), nullptr)
            .dateItem(MENU_DATE_GR_ID, "Date Gr", DONT_SAVE, NoMenuFlags, DateStorage(1, 1, 2020), nullptr)
            .actionItem(MENU_SHOW_DIALOG_ID, "Show Dialog", NoMenuFlags, onShowDialog)
            .enumItem(MENU_DIALOG_ACTIVE_ID, "Dialog Active", DONT_SAVE, strDialogActiveEnumEntries, 2, NoMenuFlags, 0, nullptr)
            .endSub()
        .subMenu(MENU_DYNAMIC_ID, "Dynamic", NoMenuFlags, nullptr)
            .enumItem(MENU_ENABLE_ID, "Enable", DONT_SAVE, strEnableEnumEntries, 2, NoMenuFlags, 0, onEnableChange)
            .analogBuilder(MENU_SET1_A_ID, "Set 1A", DONT_SAVE, NoMenuFlags, 0, nullptr)
                .offset(0).divisor(1).step(1).maxValue(255).unit("Unit").endItem()
            .floatItem(MENU_SET1_B_ID, "Set 1B", DONT_SAVE, 3, NoMenuFlags, 0.0, nullptr)
            .boolItem(MENU_SET1_C_ID, "Set 1C", DONT_SAVE, NAMING_CHECKBOX, NoMenuFlags, false, nullptr)
            .rgb32Item(MENU_SET1_D_ID, "Set 1D", DONT_SAVE, false, NoMenuFlags, RgbColor32(0, 0, 0), nullptr)
            .actionItem(MENU_SET2_A_ID, "Set 2A", NoMenuFlags, nullptr)
            .boolItem(MENU_SET2_B_ID, "Set 2B", DONT_SAVE, NAMING_TRUE_FALSE, NoMenuFlags, false, nullptr)
            .endSub()
        .listItemRtCustom(MENU_LIST_CUSTOM_ID, "List Custom", 10, fnListCustomRtCall, NoMenuFlags, nullptr)
        .listItemFlash(MENU_FLASH_LIST_ID, "Flash List", 2, strFlashListListItems, NoMenuFlags, nullptr);
}


void setupTheme();

void setup() {
    // prepare the I2C bus before setting up the menu
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_CLK, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_CLK);
    ioaWireBegin(i2c_default);

    // prepare the SPI bus before setting up the menu
    spi_init(spi_default, SPI_TEN_MHZ);
    gpio_set_function(OLED_SPI_CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(OLED_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(OLED_SPI_RX_PIN, GPIO_FUNC_SPI);

    // our example has an item tracking the first analog ADC, and also using the volume item for PWM to an LED.
    internalAnalogDevice().initPin(ADC_PICO_FIRST_OFFSET, DIR_IN);
    internalAnalogDevice().initPin(12, DIR_PWM);

    // now prepare logging
    IOLOG_START_SERIAL
    stdio_init_all();

    setupMenu();

    setupTheme();

    setTitlePressedCallback([](int) {
        showVersionDialog(&applicationInfo);
    });

    taskManager.schedule(repeatMillis(250), [] {
        getMenuA0Level().setFloatValue(internalAnalogDevice().getCurrentFloat(ADC_PICO_FIRST_OFFSET));
    });

    renderer.setResetIntervalTimeSeconds(30);
}

int main() {
    setup();
    while(1) {
        taskManager.runLoop();
    }
}

void CALLBACK_FUNCTION onVolumeChanged(int id) {
    int cv = getMenuVolume().getCurrentValue();
    int mv = getMenuVolume().getMaximumValue();
    float fltVal = static_cast<float>(cv) / static_cast<float>(mv);
    internalAnalogDevice().setCurrentFloat(12, fltVal);
}

void onDialogEnding(ButtonType buttonPressed, void* data) {
    serlogF2(SER_DEBUG, "Dialog ended ", buttonPressed);
}

void CALLBACK_FUNCTION onShowDialog(int id) {
    withMenuDialogIfAvailable([](MenuBasedDialog *dlg) {
        dlg->setButtons(BTNTYPE_ACCEPT, BTNTYPE_CANCEL, getMenuDialogActive().getCurrentValue());
        dlg->show("Test Dialog", true, onDialogEnding);
        dlg->copyIntoBuffer("Buffer data");
    });
}

void CALLBACK_FUNCTION onEnableChange(int id) {
    bool set1Visible = getMenuEnable().getCurrentValue() == 0;

    // enable and disable all the items in the menu based on the enum value
    getMenuSet1A().setVisible(set1Visible);
    getMenuSet1B().setVisible(set1Visible);
    getMenuSet1C().setVisible(set1Visible);
    getMenuSet1D().setVisible(set1Visible);
    getMenuSet2A().setVisible(!set1Visible);
    getMenuSet2B().setVisible(!set1Visible);

    // as we've structurally changed the layout of the active menu, we have to tell menu mgr
    menuMgr.notifyStructureChanged();
}

// This callback needs to be implemented by you, see the below docs:
//  1. List Docs - https://www.thecoderscorner.com/products/arduino-libraries/tc-menu/menu-item-types/list-menu-item/
//  2. ScrollChoice Docs - https://www.thecoderscorner.com/products/arduino-libraries/tc-menu/menu-item-types/scrollchoice-menu-item/
int CALLBACK_FUNCTION fnListCustomRtCall(RuntimeMenuItem* item, uint8_t row, RenderFnMode mode, char* buffer, int bufferSize) {
    if (row < LIST_PARENT_ITEM_POS && (mode == RENDERFN_NAME || mode == RENDERFN_VALUE)) {
        buffer[0]=0;
        fastltoa(buffer, row, 5, NOT_PADDED, bufferSize);
        return true;
    }
    return defaultRtListCallback(item, row, mode, buffer, bufferSize);
}

void CALLBACK_FUNCTION StatusAppeared(int id) {
    serlogF(SER_DEBUG, "Status menu has appeared");
}
