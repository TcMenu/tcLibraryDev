/**
 * ESP8266 / ESP32 WiFi and OLED example. Works out of the box with Heltek Kits 8 & 32.
 *
 * This example shows a very basic home automation system, in this case we simulate a greenhouse
 * growing tomatoes and cucumbers. We can check the temperature of the areas, check if windows
 * are opened, open the window, configure the heater and see if it's on.
 *
 * It is not a full working automation project, but rather designed to show how easily such a thing
 * could be achieved with TcMenu.
 *
 * The circuit uses a PCF8574 with both the input via rotary encoder and the outputs for the heater
 * and window opener driven by it too.
 */

#include <Wire.h>
#include "menuArd_menu.h"
#include <IoAbstractionWire.h>
#include <ArduinoEEPROMAbstraction.h>
#include <RemoteAuthentication.h>
#include <RemoteMenuItem.h>
#include <MockIoAbstraction.h>
#include <ESP8266WiFi.h>
#include <EepromItemStorage.h>

// contains the graphical widget title components.
#include "stockIcons/wifiAndConnectionIcons16x12.h"

char fileChoicesArray[255]{};

// here we define the heater and window pins on the PCF8574
// 0, 1 are A and B from encoder, 2 is OK button.
#define WINDOW_PIN 3
#define HEATER_PIN 4

// this is the interrupt pin connection from the PCF8574 back to the ESP8266 board.
#define IO_INTERRUPT_PIN 12


//
// ESP boards tend to have few pins available for general purpose use, to make things easier
// this sketch assumes the rotary encoder and switch are attached to a PCF8574 IO Expander.
// Change in menu designer's code generator to remove this if you prefer to use device pins.
//
//IoAbstractionRef io8574 = new LoggingIoAbstraction(ioFrom8574(0x20, IO_INTERRUPT_PIN), 8);
IoAbstractionRef io8574 = ioFrom8574(0x20, IO_INTERRUPT_PIN);

// eeprom wrapper, initialised in setup.
ArduinoEEPROMAbstraction *eeprom = NULL;

// we want to authenticate connections, the easiest and quickest way is to use the EEPROM
// authenticator where pairing requests add a new item into the EEPROM. Any authentication
// requests are then handled by looking in the EEPROM.
EepromAuthenticatorManager authManager;

// we add two widgets that show both the connection status and wifi signal strength
// these are added to the renderer and rendered upon any change.
TitleWidget connectedWidget(iconsConnection, 2, 16, 12);
TitleWidget wifiWidget(iconsWifi, 5, 16, 12, &connectedWidget);

// Here we create two additional menus, that will be added manually to handle the connectivity
// status and authentication keys. In a future version these will be added to th desinger.
RemoteMenuItem menuRemoteMonitor(1001, 2);
EepromAuthenicationInfoMenuItem menuAuthKeyMgr(1002, &authManager, &menuRemoteMonitor);


// state used by the sketch

bool connectedToWifi = false;

// when there's a change in communication status (client connects for example) this gets called.
void onCommsChange(CommunicationInfo info) {
    if(info.remoteNo == 0) {
        connectedWidget.setCurrentState(info.connected ? 1 : 0);
    }
    // this relies on logging in IoAbstraction's ioLogging.h, to turn it on visit the file for instructions.
    serdebugF4("Comms notify (rNo, con, enum)", info.remoteNo, info.connected, info.errorMode);
}

//
// here we just start serial for debugging and try to initialise the display and menu
//
void setup() {
    Serial.begin(115200);
    Serial.println("Starting NodeMCU example");

    // set up the inbuilt ESP rom to use for load and store.
    EEPROM.begin(512);
    eeprom = new ArduinoEEPROMAbstraction(&EEPROM);

    // now we enable authentication using EEPROM authentication. Where the EEPROM is
    // queried for authentication requests, and any additional pairs are stored there too.
    // first we initialise the authManager, then pass it to the class.
    // Always call BEFORE setupMenu()
    authManager.initialise(eeprom, 100);
    remoteServer.setAuthenticator(&authManager);

    // Here we add two additional menus for managing the connectivity and authentication keys.
    // In the future, there will be an option to autogenerate these from the designer.
    menuIpAddress.setNext(&menuAuthKeyMgr);
    menuRemoteMonitor.addConnector(remoteServer.getRemoteConnector(0));
    menuRemoteMonitor.registerCommsNotification(onCommsChange);
    menuAuthKeyMgr.setLocalOnly(true);

    // because we are initialising wifi from the menu entries, we need to load the WiFi
    // specific eeprom values very early, so we just load the two items we need.
    loadMenuItem(eeprom, &menuSSID);
    loadMenuItem(eeprom, &menuPwd);

    // this sketch assumes you've successfully connected to the Wifi before, does not
    // call begin.. You can initialise the wifi whichever way you wish here.
    if(strlen(menuSSID.getTextValue())==0) {
        // no SSID come up as an access point
        WiFi.mode(WIFI_AP);
        WiFi.softAP("tcmenu", "secret");
        serdebugF("Started up in AP mode, connect with 'tcmenu' and 'secret'");
    }
    else {
        WiFi.begin(menuSSID.getTextValue(), menuPwd.getTextValue());
        WiFi.mode(WIFI_STA);
        serdebugF("Connecting to Wifi using settings from connectivity menu");
    }

    renderer.setFirstWidget(&wifiWidget);

    // initialise the menu.
    setupMenu();

    // load back the core menu items
    menuMgr.load(*eeprom);

    // now monitor the wifi level every second and report it in a widget.
    taskManager.scheduleFixedRate(1000, [] {
        if(WiFi.status() == WL_CONNECTED) {
            if(!connectedToWifi) {
                IPAddress localIp = WiFi.localIP();
                Serial.print("Now connected to WiFi");
                Serial.println(localIp);
                menuIpAddress.setIpAddress(localIp[0], localIp[1], localIp[2], localIp[3]);
                connectedToWifi = true;
            }
            wifiWidget.setCurrentState(fromWiFiRSSITo4StateIndicator(WiFi.RSSI()));
        }
        else {
            connectedToWifi = false;
            wifiWidget.setCurrentState(0);
        }
    });

    //
    // here we simulate the temperature changing.
    // temperature doesn't change that often, even 5 seconds is probably too short.
    // in a normal system you'd probably do something other than call random..
    //
    taskManager.scheduleFixedRate(5000, [] {
        menuTomatoTemp.setCurrentValue(random(255));
        menuCucumberTemp.setCurrentValue(random(255));
    });

    //
    // Now we configure our simulated heater and window.. We use inverse
    // logic on the 8574 because it's better at pulling down to GND than up.
    //
    ioDevicePinMode(io8574, WINDOW_PIN, OUTPUT);
    ioDevicePinMode(io8574, HEATER_PIN, OUTPUT);
    ioDeviceDigitalWrite(io8574, WINDOW_PIN, HIGH);
    ioDeviceDigitalWriteS(io8574, HEATER_PIN, HIGH);
}

//
// as with all apps based off IoAbstraction's taskManager we must call loop
// very frequently. On ESP8266 it's absolutely essential that you don't do
// anything here that takes a long time as the board will get reset by it's
// internal watchdog.
//
void loop() {
    taskManager.runLoop();
}


// used by the below function to store state.
bool windowOpen = false;

//
// here we have the function that is called initially when the window is open
// and then repeatedly called by rescheduling itself until the window is closed
//
void windowOpenFn() {
    if(menuWindowOpen.getBoolean()) {
        windowOpen = !windowOpen;
        // 0 is narrow opening, 1 is wide. We simulate this by adjusting the speed of the call.
        int windowDelay = menuWinOpening.getCurrentValue() == 0 ? 500 : 250;
        serdebugF2("Setting window delay to ", windowDelay);
        taskManager.scheduleOnce(windowDelay, windowOpenFn);
    }
    else windowOpen = false;

    ioDeviceDigitalWriteS(io8574, WINDOW_PIN, windowOpen);
}

// we are using a simulated low speed PWM to control the heater
// store it in a global variable
bool heaterOn;

void heaterOnFn() {
    if(menuElectricHeater.getBoolean()) {
        heaterOn = !heaterOn;
        // the power is low medium or high. We simulate by changing the delay
        int heaterDelay = min(500, (menuHeaterPower.getCurrentValue() + 1) * 500);
        serdebugF2("Setting heater delay to ", heaterDelay);
        taskManager.scheduleOnce(heaterDelay, heaterOnFn);
    }
    else heaterOn = false;

    ioDeviceDigitalWriteS(io8574, HEATER_PIN, heaterOn);
}

//
// Below are the call back methods that are executed when a menu item changes.
//

void CALLBACK_FUNCTION onWindowOpen(int /*id*/) {
    Serial.print("Window is ");
    Serial.println(menuWindowOpen.getBoolean() ? "Open" : "Closed");
    if(menuWindowOpen.getBoolean()) {
        windowOpenFn();
    }
}

const char heaterOnHighWarningPgm[] PROGMEM = "Set heater to";

//
// this method is called when the dialog is dismissed.
//
void onDialogFinished(ButtonType btnPressed, void* /*userdata*/) {
    // if user did not accept the change, set back to the first setting.
    if(btnPressed != BTNTYPE_OK) {
        menuHeaterPower.setCurrentValue(1);
    }
}

//
// Called when the heater power is changed, we draw a dialog when there is a
// change that is greater that 1. The dialog is configured to call the method
// above when dismissed.
//
void CALLBACK_FUNCTION onHeaterPower(int /*id*/) {
    Serial.print("Heater power setting changed to ");
    int pwr = menuHeaterPower.getCurrentValue();
    Serial.println(pwr);
    if(pwr >= 1) {
        BaseDialog* dlg = renderer.getDialog();
        if(dlg) {
            dlg->setButtons(BTNTYPE_OK, BTNTYPE_CANCEL, 1);
            dlg->show(heaterOnHighWarningPgm, true, onDialogFinished); // true = shows on remote sessions.
            char sz[10];
            menuHeaterPower.copyEnumStrToBuffer(sz, sizeof(sz), pwr);
            dlg->copyIntoBuffer(sz);
        }
    }
}

void CALLBACK_FUNCTION onWindowOpening(int /*id*/) {
    Serial.print("Window setting changed");
    Serial.println(menuWinOpening.getCurrentValue());
}

void CALLBACK_FUNCTION onElectricHeater(int /*id*/) {
    Serial.print("Electric heater ");
    Serial.println(menuElectricHeater.getBoolean() ? "ON" : "OFF");
    if(menuElectricHeater.getBoolean()) {
        heaterOnFn();
    }
}

const char allSavedPgm[] PROGMEM = "All saved to EEPROM";
void CALLBACK_FUNCTION onSaveAll(int id) {
    Serial.println("Saving values to EEPROM");
    menuMgr.save(*eeprom);
    // on esp you must commit after calling save.
    EEPROM.commit();

    BaseDialog* dlg = renderer.getDialog();
    if(dlg) {
        dlg->setButtons(BTNTYPE_NONE, BTNTYPE_CLOSE);
        dlg->show(allSavedPgm, false); // false = shows only on device
        dlg->copyIntoBuffer("Committed to FLASH");
    }

}

void CALLBACK_FUNCTION onFileChoice(int id) {
    Serial.print("file chosen: ");
    auto entryNum = menuFile.getCurrentValue();
    char sz[12];
    menuFile.valueAtPosition(sz, sizeof(sz), entryNum);
    Serial.println(sz);
}

void CALLBACK_FUNCTION onLoadFiles(int id) {
    strcpy(fileChoicesArray, "MyDocument");
    strcpy(&fileChoicesArray[10], "SecretFile");
    strcpy(&fileChoicesArray[20], "AnotherFle");
    strcpy(&fileChoicesArray[30], "MenuItem");
    strcpy(&fileChoicesArray[40], "MyDocument");
    menuFile.setNumberOfRows(5);
}
