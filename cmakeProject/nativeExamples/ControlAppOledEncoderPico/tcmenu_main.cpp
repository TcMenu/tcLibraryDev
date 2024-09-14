#include "generated/ControlAppOledEncoderPico_menu.h"
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
        menuA0Level.setFloatValue(internalAnalogDevice().getCurrentFloat(ADC_PICO_FIRST_OFFSET));
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
    int cv = menuVolume.getCurrentValue();
    int mv = menuVolume.getMaximumValue();
    float fltVal = float(cv) / float(mv);
    internalAnalogDevice().setCurrentFloat(12, fltVal);
}

void onDialogEnding(ButtonType buttonPressed, void* data) {
    serlogF2(SER_DEBUG, "Dialog ended ", buttonPressed);
}

void CALLBACK_FUNCTION onShowDialog(int id) {
    withMenuDialogIfAvailable([](MenuBasedDialog *dlg) {
        dlg->setButtons(BTNTYPE_ACCEPT, BTNTYPE_CANCEL, menuDialogActive.getCurrentValue());
        dlg->show("Test Dialog", true, onDialogEnding);
        dlg->copyIntoBuffer("Buffer data");
    });
}

void CALLBACK_FUNCTION onEnableChange(int id) {
    bool set1Visible = menuEnable.getCurrentValue() == 0;

    // enable and disable all the items in the menu based on the enum value
    menuSet1A.setVisible(set1Visible);
    menuSet1B.setVisible(set1Visible);
    menuSet1C.setVisible(set1Visible);
    menuSet1D.setVisible(set1Visible);
    menuSet2A.setVisible(!set1Visible);
    menuSet2B.setVisible(!set1Visible);

    // as we've structurally changed the layout of the active menu, we have to tell menu mgr
    menuMgr.notifyStructureChanged();
}

// This callback needs to be implemented by you, see the below docs:
//  1. List Docs - https://www.thecoderscorner.com/products/arduino-libraries/tc-menu/menu-item-types/list-menu-item/
//  2. ScrollChoice Docs - https://www.thecoderscorner.com/products/arduino-libraries/tc-menu/menu-item-types/scrollchoice-menu-item/
int CALLBACK_FUNCTION fnListCustomRtCall(RuntimeMenuItem* item, uint8_t row, RenderFnMode mode, char* buffer, int bufferSize) {
    switch(mode) {
    default:
        return defaultRtListCallback(item, row, mode, buffer, bufferSize);
    }
}

void CALLBACK_FUNCTION StatusAppeared(int id) {
    serlogF(SER_DEBUG, "Status menu has appeared");
}
