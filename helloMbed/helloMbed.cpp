/*
  Note this is not a general purpose example, it is purely used to test that most
  items are working on mbed during IoAbstraction development. See the IoAbstraction
  examples for better, easier to understand sketches.
*/

#include <mbed.h>
#include <IoAbstractionWire.h>
#include <TaskManager.h>
#include "LiquidCrystalIO.h"
#include <SwitchInput.h>
#include <math.h>
#include <AnalogDeviceAbstraction.h>

// set up the pins that you'll use with the i2cbackpack.
// there's two common arrangement. RS RW EN and EN RW RS
const int rs = 0, en = 2, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
//const int rs = 2, en = 1, d4 = 4, d5 = 5, d6 = 6, d7 = 7;

const int lcdWidth = 20;
const int lcdHeight = 4;

// Enable logging of debug statements to USB serial.
// This conincides with the enablement of the logging flag, see IoLogging.h
Serial console(USBTX, USBRX);
MBedLogger LoggingPort(console);

MBedAnalogDevice analogDevice;

// here we create an I2C bus on the hardware port and then connect
// an 8574 with the LiquidCrystal library
I2C i2c(PF_0, PF_1);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7, ioFrom8574(0x40, 0xff, &i2c));

IoAbstractionRef mcp23017 = ioFrom23017(0x4E, ACTIVE_LOW_OPEN, PF_12, &i2c);

const uint8_t smiley[8] = {
        0b00000,
        0b00000,
        0b01010,
        0b00000,
        0b00000,
        0b10001,
        0b01110,
        0b00000
};

int oldPosition = 0;
volatile int pressCount = 0;

class RotationPlotter : public Executable {
    float angle = 0.0;
public:
    void start() {
        analogDevice.initPin(PB_11, DIR_PWM);
    }
    void exec() override {
        angle += .0062F;
        if(angle > (2 * M_PI)) angle = 0.0F;
        auto amp = sin(angle);
        analogDevice.setCurrentFloat(PB_11, amp);
    }
} rotationPlotter;

bool ledOn23017;

int main() {
    console.baud(115200);
    lcd.configureBacklightPin(3);
    lcd.setBacklight(true);

    // set up the LCD's number of columns and rows:
    lcd.begin(lcdWidth, lcdHeight);
    lcd.createChar(1, smiley);
    lcd.setCursor(0,0);
    lcd.print("Counter in seconds");

    switches.initialiseInterrupt(internalDigitalIo(), true);
    switches.addSwitch(PA_5, [](pinid_t, bool) {
        pressCount++;
    });
    switches.addSwitch(USER_BUTTON, [](pinid_t, bool) {
        pressCount--;
    }, 25, true);

    setupRotaryEncoderWithInterrupt(PA_6, PD_14, [] (int val) {
    });
    switches.getEncoder()->changePrecision(255, 128);

    analogDevice.initPin(A0, DIR_IN);

    rotationPlotter.start();
    taskManager.scheduleFixedRate(1, &rotationPlotter);

    ioDevicePinMode(mcp23017, 0, OUTPUT);
    ioDevicePinMode(mcp23017, 1, INPUT);

    taskManager.scheduleFixedRate(500, [] {
       ledOn23017 = !ledOn23017;
       ioDeviceDigitalWriteS(mcp23017, 0, ledOn23017);
       if(ioDeviceDigitalRead(mcp23017, 1) != 0) {
           pressCount++;
       }
    });

    taskManager.scheduleFixedRate(200, [] {
        // set the cursor to column 0, line 1
        lcd.setCursor(0, 1);
        lcd.print("                  ");
        lcd.setCursor(0, 1);
        // print the number of seconds since reset as a float:
        double fractionalMillis = (millis() / 1000.0) - 10000.0;
        lcd.print(fractionalMillis);
        lcd.setCursor(12, 1);
        lcd.print(analogDevice.getCurrentFloat(A0));
        lcd.setCursor(oldPosition % lcdWidth, 2);
        lcd.print(' ');
        oldPosition++;
        lcd.setCursor(oldPosition % lcdWidth, 2);
        lcd.write(0x01);
        lcd.setCursor(0, 3);
        lcd.print("                  ");
        lcd.setCursor(0, 3);
        lcd.print(pressCount);
        lcd.setCursor(7, 3);
        lcd.print("Enc=");
        lcd.print(switches.getEncoder()->getCurrentReading());

    });

    while(1) {
        taskManager.runLoop();
    }
}





