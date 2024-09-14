/*
 * This example shows how to use a matrix keyboard using our libraries, it is the simplest example possible using
 * device pins to the keyboard matrix. However, it is possible to use matrix keyboards  with certain IO expanders
 * as well.
 * See https://www.thecoderscorner.com/products/arduino-libraries/io-abstraction/matrix-keyboard-keypad-manager/
 */

#include <TaskManagerIO.h>
#include <SwitchInput.h>
#include <extras/SPIHelper.h>
#include <hardware/i2c.h>
#include "PlatformDeterminationWire.h"
#include "EepromAbstractionWire.h"
#include "KeyboardManager.h"

//
// We need to make a keyboard layout that the manager can use. choose one of the below.
// The parameter in brackets is the variable name.
//
MAKE_KEYBOARD_LAYOUT_3X4(keyLayout)
//MAKE_KEYBOARD_LAYOUT_4X4(keyLayout)

//
// We need a keyboard manager class too
//
MatrixKeyboardManager keyboard;

//
// We need a class that extends from KeyboardListener. this gets notified when
// there are changes in the keyboard state.
//
class MyKeyboardListener : public KeyboardListener {
public:
    void keyPressed(char key, bool held) override {
        serlogF3(SER_DEBUG, "Key press ", key, held);
    }

    void keyReleased(char key) override {
        serlogF2(SER_DEBUG, "Key release ", key);
    }
} myListener;


int main() {
    // here we configure the key matrix.
    keyLayout.setRowPin(0, 22);
    keyLayout.setRowPin(1, 21);
    keyLayout.setRowPin(2, 20);
    keyLayout.setRowPin(3, 19);
    keyLayout.setColPin(0, 18);
    keyLayout.setColPin(1, 17);
    keyLayout.setColPin(2, 16);
    IOLOG_START_SERIAL
    serlogF(SER_DEBUG, "Starting IoAbstraction Matrix Keyboard demo");

    // create the keyboard mapped to arduino pins and with the layout chosen above.
    // it will callback our listener when keys are pressed and released.
    keyboard.initialise(asIoRef(internalDigitalDevice()), &keyLayout, &myListener);


    while(1) {
        taskManager.runLoop();
    }
}
