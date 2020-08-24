#include <mbed.h>
#include <AnalogDeviceAbstraction.h>
#include "mbedMenu_menu.h"
#include "NTPTime.h"
#include "U8g2lib.h"

I2C i2c(PF_0, PF_1);

Serial console(USBTX, USBRX);
MBedLogger LoggingPort(console);

MBedAnalogDevice analogDevice;

U8g2MbedI2c_1306_128x64_i2c gfx(&i2c, U8G2_R0);

void prepareRealtimeClock();
void monitorAnalogIn();

void setup() {
    console.baud(115200);

    gfx.begin();
    taskManager.scheduleFixedRate(1000, [] {
        gfx.print("A");
        gfx.sendBuffer();
    });
/*    setupMenu();

    prepareRealtimeClock();
    monitorAnalogIn();

    switches.addSwitch(USER_BUTTON, [](pinid_t, bool ) {
        auto bval = menuABoolean.getBoolean();
        menuABoolean.setBoolean(!bval);
    }, 100, true);*/


}

int main() {
    setup();

    while(1) {
        taskManager.runLoop();
    }
}

void monitorAnalogIn() {
    analogDevice.initPin(A0, DIR_IN);
    analogDevice.initPin(PB_11, DIR_PWM);
    taskManager.scheduleFixedRate(250, [] {
        menuA0Value.setFloatValue(analogDevice.getCurrentFloat(A0) * 100.0F);
    });
}

void prepareRealtimeClock() {
    if(remoteServer.isBound()) {

        NTPTime ntp(remoteServer.networkInterface(), "2.pool.ntp.org", 123);
        time_t timestamp = ntp.getTimeFromNTP();

        if (timestamp > 0) {
            set_time(timestamp);

            taskManager.scheduleFixedRate(1, [] {
                auto timeNow = time(NULL);
                auto tm = gmtime(&timeNow);
                menuRTCDate.getUnderlyingData()->year = (tm->tm_year + 1900);
                menuRTCDate.getUnderlyingData()->month = tm->tm_mon;
                menuRTCDate.getUnderlyingData()->day = tm->tm_mday;
                menuRTCTime.getUnderlyingData()->hours = tm->tm_hour;
                menuRTCTime.getUnderlyingData()->minutes = tm->tm_min;
                menuRTCTime.getUnderlyingData()->seconds = tm->tm_sec;
                menuRTCTime.setSendRemoteNeededAll();
                menuRTCDate.setSendRemoteNeededAll();
                menuRTCTime.setChanged(true);
                menuRTCDate.setChanged(true);

            }, TIME_SECONDS);

            return;
        }

    }
    serdebugF("Waiting for Interface / NTP");
    taskManager.scheduleOnce(5, prepareRealtimeClock, TIME_SECONDS);
}


void CALLBACK_FUNCTION onUserButton(int id) {
    serdebugF2("On change called", id);
}


void CALLBACK_FUNCTION onAnalogChange(int id) {
    auto currentPwm = menuAnalogValue.getCurrentValue() / 100.0F;
    analogDevice.setCurrentFloat(PB_11, currentPwm);
}
