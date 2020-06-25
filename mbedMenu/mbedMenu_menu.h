/*
    The code in this file uses open source libraries provided by thecoderscorner

    DO NOT EDIT THIS FILE, IT WILL BE GENERATED EVERY TIME YOU USE THE UI DESIGNER
    INSTEAD EITHER PUT CODE IN YOUR SKETCH OR CREATE ANOTHER SOURCE FILE.

    All the variables you may need access to are marked extern in this file for easy
    use elsewhere.
*/

#ifndef MENU_GENERATED_CODE_H
#define MENU_GENERATED_CODE_H

#include <mbed.h>
#include <tcMenu.h>
#include <RuntimeMenuItem.h>
#include <LiquidCrystalIO.h>
#include <IoAbstractionWire.h>
#include "MBedEthernetTransport.h"
#include <RemoteConnector.h>
#include <EditableLargeNumberMenuItem.h>
#include "tcMenuLiquidCrystal.h"

void setupMenu();  // forward reference of the menu setup function.
extern const PROGMEM ConnectorLocalInfo applicationInfo;  // defines the app info to the linker.

// Global variables that need exporting

extern I2C i2c;
extern LiquidCrystal lcd;
extern LiquidCrystalRenderer renderer;

// Callback functions must always include CALLBACK_FUNCTION after the return type
#define CALLBACK_FUNCTION

// Global Menu Item exports

extern EnumMenuItem menuSubMenuFoods;
extern IpAddressMenuItem menuSubMenuEditIP;
extern IpAddressMenuItem menuSubMenuIPAddress;
extern EditableLargeNumberMenuItem menuSubMenuLargeNum;
extern SubMenuItem menuSubMenu;
extern FloatMenuItem menuA0Value;
void CALLBACK_FUNCTION onUserButton(int id);
extern BooleanMenuItem menuABoolean;
extern AnalogMenuItem menuAnalogValue;
extern DateFormattedMenuItem menuRTCDate;
extern TimeFormattedMenuItem menuRTCTime;

#endif // MENU_GENERATED_CODE_H
