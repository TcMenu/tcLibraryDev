#include "mbed.h"
#include "menuArd_menu.h"
#include <IoAbstractionWire.h>
#include <TaskManager.h>
#include <RemoteAuthentication.h>
#include <RemoteMenuItem.h>

// contains the graphical widget title components.
#include "stockIcons/wifiAndConnectionIconsLCD.h"

/**
 * This TcMenu example shows how to take over the display for your own purposes from a menu item.
 * It also shows how to use the dialog facilities to locally show an information dialog and also a
 * question dialog.
 * 
 * For more detail see the README.md file
 */

void myDisplayFunction(unsigned int encoderValue, RenderPressMode clicked);

I2C i2c(PF_0, PF_1);
IoAbstractionRef io8574 = ioFrom8574(0x40, 0xff, &i2c);

// a counter that we use in the display function when we take over the display.
int counter = 0;

void setup() {

    // When the renderer times out and is about to reset to main menu, you can get a callback.
    // For example if the menu should only be displayed during configuration.
    //
    // Call BEFORE setupMenu to ensure it takes effect immediately, call AFTER setupMenu if you
    // want to start in menu mode, but then apply the reset handler from that point onwards.
    renderer.setResetCallback([] {
        counter = 0;
        renderer.takeOverDisplay(myDisplayFunction);
    });

    serdebug("Added the reset callback");


    // here we use the EEPROM to load back the last set of values.
    menuMgr.setRootMenu(&menuTime);
    //menuMgr.load(eeprom);

    // this is put in by the menu designer and must be called (always ensure devices are setup first).
    setupMenu();
}

//
// standard setup for all taskManager based sketches. Always call runLoop in the loop.
// Never do anything long running in here.
//
void loop() {
    taskManager.runLoop();
}

//
// When the food choice option is changed on the menu, this function is called, it takes
// the value from menuFood and renders it as text in the menuText text item.
//
void CALLBACK_FUNCTION onFoodChoice(int /*id*/) {
    // copy the enum text for the current value
    char enumStr[20];
    int enumVal = menuFood.getCurrentValue();
    menuFood.copyEnumStrToBuffer(enumStr, sizeof(enumStr), enumVal);
    
    // and put it into a text menu item
    menuText.setTextValue(enumStr);
}

//
// this is the function called by the renderer every 1/5 second once the display is
// taken over, we pass this function to takeOverDisplay below.
//
void myDisplayFunction(unsigned int encoderValue, RenderPressMode clicked) {
    // we initialise the display on the first call.
    if(counter == 0) {
        switches.changeEncoderPrecision(999, 50);
        lcd.clear();
        lcd.print("Hello mbed tcMenu");
        lcd.setCursor(0, 1);
        lcd.print("OK button for menu..");
    }

    // We are told when the button is pressed in by the boolean parameter.
    // When the button is clicked, we give back to the menu..
    if(clicked) {
        renderer.giveBackDisplay();
        counter = 0;
    }
    else {
        char buffer[5];
        // otherwise update the counter.
        lcd.setCursor(0, 2);
        ltoaClrBuff(buffer, ++counter, 4, ' ', sizeof(buffer));
        lcd.print(buffer);
        lcd.setCursor(12, 2);
        ltoaClrBuff(buffer, encoderValue, 4, '0', sizeof(buffer));
        lcd.print(buffer);
    }
}

//
// We have an option on the menu to take over the display, this function is called when that
// option is chosen.
//
void CALLBACK_FUNCTION onTakeOverDisplay(int /*id*/) {
    // in order to take over rendering onto the display we just request the display
    // at which point tcMenu will stop rendering until the display is "given back".
    // Don't forget that LiquidCrystalIO uses task manager and things can be happening
    // in the background. Always ensure all operations with the LCD occur on the rendering
    // call back.

    counter = 0;
    renderer.takeOverDisplay(myDisplayFunction);
}

const char pgmInfoHeader[] PROGMEM = "Information dialog";
const char pgmQuestionHeader[] PROGMEM = "Order Food?";

void CALLBACK_FUNCTION onInfoDlg(int /*id*/) {
    // every renderer apart from NoRenderer has a dialog, that can be used to present
    // very basic info locally onto any display. Used in situations where something
    // needs to be confirmed / printed onto the local display.
    BaseDialog* dlg = renderer.getDialog();
    if(!dlg) return;

    // first we set the buttons how we want them. BTNTYPE_NONE means no button.
    dlg->setButtons(BTNTYPE_NONE, BTNTYPE_CLOSE);

    // then we show the dialog - 2nd boolean parameter is if dialog is local only
    dlg->show(pgmInfoHeader, true);

    // and then we set the second line (buffer) - must be after show.
    dlg->copyIntoBuffer("to be set..");

    // you can set the dialog buffer at some point later, it's safe, even if it's been dismissed.
    taskManager.scheduleOnce(1000, [] {
        BaseDialog* dlg = renderer.getDialog();
        dlg->copyIntoBuffer("now it's set..");
    });
}

//
// It's also possible to know when the dialog has finished, and what button was pressed.
// This is done by passing a function like below as second parameter to show.
//
void onFinished(ButtonType btn, void* /*userData*/) {
    if(btn == BTNTYPE_ACCEPT) {
        char sz[20];
        menuFood.copyEnumStrToBuffer(sz, sizeof(sz), menuFood.getCurrentValue());
    }
    else {
    }
}

void CALLBACK_FUNCTION onQuestionDlg(int /*id*/) {
    // yet another dialog, to ask a question this time.
    BaseDialog* dlg = renderer.getDialog();
    
    // this time we use two buttons and provide the selected index at the end (zero based)
    dlg->setButtons(BTNTYPE_ACCEPT, BTNTYPE_CANCEL, 1);
    
    // we can optionally set some data that will be given to us in the finished call back.
    dlg->setUserData(NULL); 
    
    // now we show the dialog (also giving the finished callback)
    dlg->show(pgmQuestionHeader, true, onFinished);

    // and lastly we set the text in the buffer area (2nd line)
    char sz[20];
    menuFood.copyEnumStrToBuffer(sz, sizeof(sz), menuFood.getCurrentValue());
    dlg->copyIntoBuffer(sz);
}

//
// We have a save option on the menu to save the settings. In a real system we could instead
// look at using a power down detection circuit to do this. For more info see below link.
// https://www.thecoderscorner.com/electronics/microcontrollers/psu-control/detecting-power-loss-in-powersupply/
//
void CALLBACK_FUNCTION onSaveSettings(int /*id*/) {
    //menuMgr.save(eeprom);
}

const char pgmPinTooShort[] PROGMEM = "Pin too short";
void CALLBACK_FUNCTION onChangePin(int id) {
	const char* sz = menuConnectivityChangePin.getTextValue();
	if (strlen(sz) < 4) {
		BaseDialog* dlg = renderer.getDialog();
		dlg->setButtons(BTNTYPE_NONE, BTNTYPE_CLOSE);
		dlg->copyIntoBuffer(sz);
		dlg->show(pgmPinTooShort, false);
	}
	else {
		//authManager.changePin(sz);
		serdebugF2("Pin changed to ", sz);
	}
}

























