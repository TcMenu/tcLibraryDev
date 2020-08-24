/*
    The code in this file uses open source libraries provided by thecoderscorner

    DO NOT EDIT THIS FILE, IT WILL BE GENERATED EVERY TIME YOU USE THE UI DESIGNER
    INSTEAD EITHER PUT CODE IN YOUR SKETCH OR CREATE ANOTHER SOURCE FILE.

    All the variables you may need access to are marked extern in this file for easy
    use elsewhere.
*/

#include <mbed.h>
#include <tcMenu.h>
#include "mbedMenu_menu.h"

// Global variable declarations

const PROGMEM ConnectorLocalInfo applicationInfo = { "MBed Test", "98ea360b-fe08-444a-996b-2e94dda7a2eb" };
U8g2GfxMenuConfig gfxConfig;
U8g2MenuRenderer renderer;

// Global Menu Item declarations

const char enumStrSubMenuFoods_0[]  = "Pizza";
const char enumStrSubMenuFoods_1[]  = "Pasta";
const char enumStrSubMenuFoods_2[]  = "Salad";
const char enumStrSubMenuFoods_3[]  = "Beef";
const char enumStrSubMenuFoods_4[]  = "Falafel";
const char* const enumStrSubMenuFoods[]  = { enumStrSubMenuFoods_0, enumStrSubMenuFoods_1, enumStrSubMenuFoods_2, enumStrSubMenuFoods_3, enumStrSubMenuFoods_4 };
const EnumMenuInfo minfoSubMenuFoods = { "Foods", 10, 0xFFFF, 4, NO_CALLBACK, enumStrSubMenuFoods };
EnumMenuItem menuSubMenuFoods(&minfoSubMenuFoods, 0, NULL);
RENDERING_CALLBACK_NAME_INVOKE(fnSubMenuEditIPRtCall, ipAddressRenderFn, "Edit IP", -1, NO_CALLBACK)
IpAddressMenuItem menuSubMenuEditIP(fnSubMenuEditIPRtCall, 9, &menuSubMenuFoods);
RENDERING_CALLBACK_NAME_INVOKE(fnSubMenuIPAddressRtCall, ipAddressRenderFn, "IP Address", -1, NO_CALLBACK)
IpAddressMenuItem menuSubMenuIPAddress(fnSubMenuIPAddressRtCall, 8, &menuSubMenuEditIP);
RENDERING_CALLBACK_NAME_INVOKE(fnSubMenuLargeNumRtCall, largeNumItemRenderFn, "Large Num", -1, NO_CALLBACK)
EditableLargeNumberMenuItem menuSubMenuLargeNum(fnSubMenuLargeNumRtCall, 7, 7, 3, &menuSubMenuIPAddress);
const SubMenuInfo minfoSubMenu = { "Sub Menu", 6, 0xFFFF, 0, NO_CALLBACK };
RENDERING_CALLBACK_NAME_INVOKE(fnSubMenuRtCall, backSubItemRenderFn, "Sub Menu", -1, NO_CALLBACK)
BackMenuItem menuBackSubMenu(fnSubMenuRtCall, &menuSubMenuLargeNum);
SubMenuItem menuSubMenu(&minfoSubMenu, &menuBackSubMenu, NULL);
const FloatMenuInfo minfoA0Value = { "A0 Value", 5, 0xFFFF, 3, NO_CALLBACK };
FloatMenuItem menuA0Value(&minfoA0Value, &menuSubMenu);
const BooleanMenuInfo minfoABoolean = { "A Boolean", 4, 0xFFFF, 1, onUserButton, NAMING_TRUE_FALSE };
BooleanMenuItem menuABoolean(&minfoABoolean, false, &menuA0Value);
const AnalogMenuInfo minfoAnalogValue = { "Analog Value", 2, 0xFFFF, 100, onAnalogChange, 0, 0, "" };
AnalogMenuItem menuAnalogValue(&minfoAnalogValue, 0, &menuABoolean);
RENDERING_CALLBACK_NAME_INVOKE(fnRTCDateRtCall, dateItemRenderFn, "RTC Date", -1, NO_CALLBACK)
DateFormattedMenuItem menuRTCDate(fnRTCDateRtCall, 3, &menuAnalogValue);
RENDERING_CALLBACK_NAME_INVOKE(fnRTCTimeRtCall, timeItemRenderFn, "RTC Time", -1, NO_CALLBACK)
TimeFormattedMenuItem menuRTCTime(fnRTCTimeRtCall, 1, (MultiEditWireType)EDITMODE_TIME_12H, &menuRTCDate);

// This is needed to provide the base Arduino compatibility layer
IoAbstractionRef globalAbstraction = internalDigitalIo();


// Set up code

void setupMenu() {
    menuRTCTime.setReadOnly(true);
    menuRTCDate.setReadOnly(true);
    menuSubMenuIPAddress.setReadOnly(true);

    prepareBasicU8x8Config(gfxConfig);
    renderer.setGraphicsDevice(&gfx, &gfxConfig);
    switches.initialise(internalDigitalIo(), true);
    menuMgr.initForEncoder(&renderer, &menuRTCTime, PA_6, PD_14, PA_5);
    remoteServer.begin(3333, &applicationInfo);
}
