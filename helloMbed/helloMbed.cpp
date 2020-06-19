/*
  LiquidCrystal Library - Hello World for mbed over i2c.

  *** NOTE this example is only for mbed framework. Most other examples are Arduino ***

  This assumes that an LCD display has been attached with a fairly regular I2C adapter
  it will count up from 0 onto the display once per second.
*/

#include <mbed.h>
#include <IoAbstractionWire.h>
#include <TaskManager.h>
#include "LiquidCrystalIO.h"

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

// here we create an I2C bus on the hardware port and then connect
// an 8574 with the LiquidCrystal library
I2C i2c(PF_0, PF_1);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7, ioFrom8574(0x40, 0xff, &i2c));

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

int main() {
    lcd.configureBacklightPin(3);
    lcd.setBacklight(true);

    // set up the LCD's number of columns and rows:
    lcd.begin(lcdWidth, lcdHeight);
    lcd.createChar(1, smiley);
    lcd.setCursor(0,0);
    lcd.print("Counter in seconds");

    //
    // when using this version of liquid crystal, it interacts (fairly) nicely with task manager.
    // instead of doing stuff in loop, we can schedule things to be done. But just be aware than
    // only one task can draw to the display. Never draw to the display in two tasks.
    //
    // You don't have to use the library with task manager like this, it's an option.
    //
    taskManager.scheduleFixedRate(200, [] {
        // set the cursor to column 0, line 1
        lcd.setCursor(0, 1);

        // print the number of seconds since reset as a float:
        double fractionalMillis = (millis() / 1000.0) - 10000.0;
        lcd.print(fractionalMillis);
        lcd.setCursor(oldPosition % lcdWidth, 2);
        lcd.print(' ');
        oldPosition++;
        lcd.setCursor(oldPosition % lcdWidth, 2);
        lcd.write(0x01);
        lcd.setCursor(0, 3);
        lcd.print(oldPosition);
    });

    while(1) {
        taskManager.runLoop();
    }
}





