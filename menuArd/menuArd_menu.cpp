/*
    The code in this file uses open source libraries provided by thecoderscorner

    DO NOT EDIT THIS FILE, IT WILL BE GENERATED EVERY TIME YOU USE THE UI DESIGNER
    INSTEAD EITHER PUT CODE IN YOUR SKETCH OR CREATE ANOTHER SOURCE FILE.

    All the variables you may need access to are marked extern in this file for easy
    use elsewhere.
*/

#include <Arduino.h>
#include <tcMenu.h>
#include "menuArd_menu.h"

// Global variable declarations

const PROGMEM ConnectorLocalInfo applicationInfo = { "Car Dashboard", "7c4dea07-625c-44f2-9b28-9f387239a2f4" };
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
LiquidCrystalRenderer renderer(lcd, 20, 4);

// Global Menu Item declarations

const BooleanMenuInfo PROGMEM minfoSimhubConnected = { "Simhub Connected", 4, 0xFFFF, 1, NO_CALLBACK, NAMING_YES_NO };
BooleanMenuItem menuSimhubConnected(&minfoSimhubConnected, false, NULL);
const AnalogMenuInfo PROGMEM minfoGear = { "Gear", 3, 0xFFFF, 15, NO_CALLBACK, 0, 0, "" };
AnalogMenuItem menuGear(&minfoGear, 0, &menuSimhubConnected);
const AnalogMenuInfo PROGMEM minfoSpeed = { "Speed", 2, 0xFFFF, 1000, NO_CALLBACK, 0, 0, "MPH" };
AnalogMenuItem menuSpeed(&minfoSpeed, 0, &menuGear);
const AnalogMenuInfo PROGMEM minfoRPM = { "RPM", 1, 0xFFFF, 25000, NO_CALLBACK, 0, 0, "" };
AnalogMenuItem menuRPM(&minfoRPM, 0, &menuSpeed);


// Set up code

void setupMenu() {


    lcd.setIoAbstraction(io23017);
    lcd.begin(20, 4);
    switches.initialise(io23017, true);
    menuMgr.initForEncoder(&renderer, &menuRPM, 6, 7, 5);
}