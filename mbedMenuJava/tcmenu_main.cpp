#include <mbed.h>
#include "mbedMenuJava_menu.h"
#include "NTPTime.h"
#include <IoLogging.h>
#include <AnalogDeviceAbstraction.h>
#include <stockIcons/wifiAndConnectionIcons16x12.h>
#include "Fonts/FreeSans9pt7b.h"

//I2C i2c(PF_0, PF_1);
SPI spi(PB_5, PB_4, PB_3);

BufferedSerial console(USBTX, USBRX);
MBedLogger LoggingPort(console);
MBedAnalogDevice analogDevice;
bool exitDisplayProc = false;

//
// Forward references of a few functions we use later on
//
void prepareRealtimeClock();
void monitorAnalogIn();
void myDisplayProc(unsigned int encoderVal, RenderPressMode press);

TitleWidget ethernetConnectionWidget(iconsEthernetConnection, 2, 16, 12, nullptr);

void setup() {
    // set up the serial port and I2C
    console.set_baud(115200);
    //i2c.frequency(400000);

    renderer.setFirstWidget(&ethernetConnectionWidget);
    taskManager.scheduleFixedRate(1000, [] {
        ethernetConnectionWidget.setCurrentState(remoteServer.isBound() ? 1 : 0);
    });

    // Here we run the tcMenu core menu setup, part of every script.
    setupMenu();

    gfxConfig.titleFont = &FreeSans9pt7b;
    makePadding(gfxConfig.titlePadding, 0, 4, 2, 3);

    // Now we can work with menu items as they are fully setup on the renderer
    prepareRealtimeClock();
    monitorAnalogIn();

    // Here we add another switch (the user switch) and toggle the user button menu item, we also
    // drop out of our custom renderer if it's pressed.
    switches.addSwitch(USER_BUTTON, [](pinid_t pin, bool held) {
       menuUserButton.setBoolean(!menuUserButton.getBoolean());
       exitDisplayProc = true;
    }, 20, true);

    // If you want to have another screen when the system is idle, you can get a call back when the renderer resets
    // after it's idle for some time.
    renderer.setResetCallback([] {
       // renderer.takeOverDisplay(myDisplayProc);
    });
    //renderer.setResetIntervalTimeSeconds(seconds);

    // start in the screen saver
    //renderer.takeOverDisplay(myDisplayProc);
}

int main() {
    setup();
    // every task manager based app must call the runLoop as frequently as possible
    while(1) {
        taskManager.runLoop();
    }
}

class NTPTimeMenuSetup : public NTPTimeBaseEvent {
public:

    NTPTimeMenuSetup(NetworkInterface *interface, const char *timeServer, int timePort)
                    : NTPTimeBaseEvent(interface, timeServer, timePort) {

    }

    void exec() override {
        if (theTime > 0) {
            set_time(theTime);

            taskManager.scheduleFixedRate(1, [] {
                auto timeNow = time(NULL);
                auto tm = gmtime(&timeNow);
                menuRTCDate.getUnderlyingData()->year = (tm->tm_year + 1900);
                menuRTCDate.getUnderlyingData()->month = tm->tm_mon + 1;
                menuRTCDate.getUnderlyingData()->day = tm->tm_mday;
                menuRTCTime.getUnderlyingData()->hours = tm->tm_hour;
                menuRTCTime.getUnderlyingData()->minutes = tm->tm_min;
                menuRTCTime.getUnderlyingData()->seconds = tm->tm_sec;
                menuRTCTime.setSendRemoteNeededAll();
                menuRTCDate.setSendRemoteNeededAll();
                menuRTCTime.setChanged(true);
                menuRTCDate.setChanged(true);

            }, TIME_SECONDS);
            setCompleted(true);
        }
    }
};

void prepareRealtimeClock() {
    if(remoteServer.isBound()) {
        SocketAddress addr;
        if(remoteServer.networkInterface()->get_ip_address(&addr) == NSAPI_ERROR_OK) {
            menuSubMenuIPAddress.setIpAddress(addr.get_ip_address());
            taskManager.registerEvent(new NTPTimeMenuSetup(remoteServer.networkInterface(), "2.pool.ntp.org", 123),  true);
            return;
        }
    }
    serdebugF("Waiting for Interface / NTP");
    taskManager.scheduleOnce(5, prepareRealtimeClock, TIME_SECONDS);
}

void monitorAnalogIn() {
    analogDevice.initPin(A0, DIR_IN);
    analogDevice.initPin(PB_11, DIR_PWM);
    taskManager.scheduleFixedRate(250, [] {
        menuA0Value.setFloatValue(analogDevice.getCurrentFloat(A0) * 100.0F);
    });
}

int offsetX = 0, offsetY = 0, renderTickCount = 0;

void myDisplayProc(unsigned int encoderVal, RenderPressMode press) {
    if(press != RPRESS_NONE || exitDisplayProc) {
        renderer.giveBackDisplay();
        return;
    }
    gfx.clearDisplay();
    gfx.setFont(&FreeSans9pt7b);
    gfx.setTextSize(1);

    if((renderTickCount % 100) == 0) {
        offsetX = rand() % 64;
        offsetY = rand() % 25;
    }

    gfx.setCursor(offsetX, offsetY + 12);
    gfx.print("Mbed demo");

    gfx.setFont(nullptr);
    gfx.setCursor(5 + offsetX, 20 + offsetY);
    char sz[32];
    menuRTCDate.copyValue(sz, sizeof sz);
    gfx.print(sz);

    gfx.setCursor(5 + offsetX, 32 + offsetY);
    menuRTCTime.copyValue(sz, sizeof sz);
    gfx.print(sz);

    gfx.display();
    renderTickCount++;
}

void CALLBACK_FUNCTION onUserButton(int id) {
    serdebugF("User button menu item changed");
}

void CALLBACK_FUNCTION onTakeDisplay(int id) {
    renderer.takeOverDisplay(myDisplayProc);
}
