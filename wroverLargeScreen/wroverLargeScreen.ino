#include "wroverLargeScreen_menu.h"
#include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeSans9pt7b.h"
#include <ArduinoEEPROMAbstraction.h>
#include <stockIcons/wifiAndConnectionIcons16x12.h>

#define TFT_BACKLIGHT 5

ArduinoEEPROMAbstraction *eeprom = NULL;
void wifiEventAlert(system_event_t *event);

TitleWidget connectedWidget(iconsConnection, 2, 16, 12, nullptr);
TitleWidget wifiWidget(iconsWifi, 5, 16, 12, &connectedWidget);

class WifiEvent : public BaseEvent {
public:
    enum WifiStatus { NOT_STARTED, CONNECTING, AUTHENTICATION_FAILED, FULLY_CONNECTED, NOT_CONNECTED };
private:
    WifiStatus currentStatus;
    IPAddress ipAddr;
    TextMenuItem* connectivityInfoItem;
    IpAddressMenuItem* ipAddressItem;
    TitleWidget* signalStrengthWidget;
public:
    WifiEvent(IpAddressMenuItem* ipAddressMenuItem, TextMenuItem* connectivityInfoItem_ = nullptr) {
        ipAddressItem = ipAddressMenuItem;
        connectivityInfoItem = connectivityInfoItem_;
        currentStatus = NOT_CONNECTED;
        setTriggered(true);
    }

    void exec() override {
        ipAddressItem->setIpAddress(ipAddr[0],ipAddr[1],ipAddr[2],ipAddr[3]);
        if(connectivityInfoItem) connectivityInfoItem->setTextValue(wifiStatusAsString(currentStatus));
        char sz[20];
        ipAddressItem->copyValue(sz, sizeof sz);
        serdebugF3("WiFi state change to ", currentStatus, sz);
    }

    const char* wifiStatusAsString(WifiStatus s) {
        switch(s) {
            case NOT_STARTED:
                return "Not Started";
            case CONNECTING:
                return "Connecting";
            case AUTHENTICATION_FAILED:
                return "Auth Failed";
            case FULLY_CONNECTED:
                return "Connected";
            default:
            case NOT_CONNECTED:
                return "Disconnected";
        }
    }

    uint32_t timeOfNextCheck() override {
        return 5000;
    }

    void setIpAddress(IPAddress ip) {
        ipAddr = ip;
    }

    void statusChanged(WifiStatus status) {
        currentStatus = status;
        markTriggeredAndNotify();
    }
} wifiStatusEvent(&menuConnectivityDHCPIP, &menuConnectivityWiFiStatus);

void setup() {
    SPI.begin(19, 25, 23);
    SPI.setFrequency(20000000);
    Serial.begin(115200);

    EEPROM.begin(512);
    eeprom = new ArduinoEEPROMAbstraction(&EEPROM);

    pinMode(TFT_BACKLIGHT, OUTPUT);
    digitalWrite(TFT_BACKLIGHT, LOW);

    menuMgr.setRootMenu(&menuVolume);
    menuMgr.load(*eeprom);

    if(strlen(menuConnectivitySSID.getTextValue()) == 0) {
        WiFi.mode(WIFI_MODE_AP);
        WiFi.softAP("tcmenu", "secret");
        serdebugF("Started up in AP mode, connect with 'tcmenu' and 'secret'");
    }
    else {
        taskManager.registerEvent(&wifiStatusEvent);
        WiFi.onEvent(wifiEventAlert);
        WiFi.mode(WIFI_MODE_STA);
        WiFi.begin(menuConnectivitySSID.getTextValue(), menuConnectivityPassword.getTextValue());
    }

    renderer.setFirstWidget(&wifiWidget);

    setupMenu();

    gfxConfig.titleFont = &FreeSans12pt7b;
    gfxConfig.titleFontMagnification = 1;
    gfxConfig.itemFont = &FreeSans9pt7b;
    gfxConfig.itemFontMagnification = 1;
    gfxConfig.bgTitleColor = RGB(126, 158, 218);
    gfxConfig.bgSelectColor = RGB(40, 66, 119);
    makePadding(gfxConfig.itemPadding, 0, 4, 6, 4);
}

void wifiEventAlert(system_event_t *event) {
    switch(event->event_id) {
        case SYSTEM_EVENT_STA_CONNECTED:
            wifiStatusEvent.statusChanged(WifiEvent::CONNECTING);
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            // once we have a connection, then we start tracking the WiFi RSSI
            taskManager.scheduleFixedRate(1, [] {
                wifiWidget.setCurrentState(fromWiFiRSSITo4StateIndicator(WiFi.RSSI()));
            }, TIME_SECONDS);
            wifiStatusEvent.setIpAddress(WiFi.localIP());
            wifiStatusEvent.statusChanged(WifiEvent::FULLY_CONNECTED);
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            wifiStatusEvent.statusChanged(WifiEvent::NOT_CONNECTED);
        case SYSTEM_EVENT_STA_WPS_ER_FAILED:
        case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
            wifiStatusEvent.statusChanged(WifiEvent::AUTHENTICATION_FAILED);
        default:
            break;
    }
}


void loop() {
    taskManager.runLoop();
}


void CALLBACK_FUNCTION onVolumeChanged(int id) {
    // TODO - your menu change code
}

void CALLBACK_FUNCTION onChannelChanged(int id) {
    // TODO - your menu change code
}

void CALLBACK_FUNCTION onTrimChange(int id) {
    // TODO - your menu change code
}

const char hdrPgmCommitted[] PROGMEM = "Configuration Saved";

void CALLBACK_FUNCTION onSave(int id) {
    menuMgr.save(*eeprom);
    EEPROM.commit();
    auto dlg = renderer.getDialog();
    if(dlg && !dlg->isInUse()) {
        dlg->setButtons(BTNTYPE_NONE, BTNTYPE_OK);
        dlg->copyIntoBuffer("Flash was updated");
        dlg->show(hdrPgmCommitted, true);
    }
}
