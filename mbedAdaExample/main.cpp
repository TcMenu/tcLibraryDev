/* Example of Reading all the ADC pins PIN and display ot OLED display using
 Nucleo_F303K8. Displays voltage read from first 2 ADC lines on OLED display

  By Joseph Ellsworth CTO of A2WH
  Take a look at A2WH.com Producing Water from Air using Solar Energy
  March-2016 License: https://developer.mbed.org/handbook/MIT-Licence
  Please contact us http://a2wh.com for help with custom design projects.


  Used to Drive:
   ebay part http://www.ebay.com/itm/152000005331?_trksid=p2057872.m2749.l2649&ssPageName=STRK%3AMEBIDX%3AIT
  0-96-I2C-IIC-SPI-Serial-White-OLED-LCD-LED-Display-Module-128X64
  The Datasheet.  https://www.adafruit.com/datasheets/SSD1306.pdf

  Unfortunately this part never turns on using the SSD1308 library.
  but did find that the https://developer.mbed.org/users/nkhorman/code/Adafruit_GFX/
  library works.  Unfortunately the Adafruit library doesn't include the scroll functionality.

  Using my I2C address scanner I found that it responds on Added 120 (x78) ,121 (x79)
  and when the part was unsoldered nothing responded on 120 or 121. According to
  to the page #19 (8.1.5) of the data sheet the I2C address should be 011110
  which seems to map correctly to dec=120 hex=79

 */

#include <cstdio>
#include <TaskManager.h>
#include <SwitchInput.h>
#include "mbed.h"
#include "Adafruit_SSD1306.h"
#include <IoLogging.h>

// Host PC Communication channels
BufferedSerial pc(USBTX, USBRX); // tx, rx
FILE* serPort = fdopen(&pc, "w");
MBedLogger LoggingPort(pc);

// an I2C sub-class that provides a constructed default

I2C i2c(PF_0,PF_1);
Adafruit_SSD1306_I2c gfx(i2c, NC, SSD_I2C_ADDRESS, 64, 132, SH_1106);

bool exitApp = false;

const uint8_t iconWifiThreeBar[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x03,
        0x30, 0x0c, 0x08, 0x10, 0xc0, 0x03, 0x20, 0x04, 0x80, 0x01, 0x80, 0x01
};
int main()
{
    i2c.frequency(400000);
    pc.set_baud(115200);
    fprintf(serPort, "Hello from mbed graphics demo\n");

    switches.initialise(internalDigitalIo(), true);
    switches.addSwitch(PE_4, [] (pinid_t id, bool held) {
        fprintf(serPort, "Switch Pressed %d, %d\n", (int)id, (int) held);
    });
    switches.addSwitch(USER_BUTTON, [] (pinid_t id, bool held) {
        fprintf(serPort, "User Pressed %d, %d\n", (int)id, (int) held);
    }, NO_REPEAT, true);

    setupRotaryEncoderWithInterrupt(PE_2, PE_5, []( int val) {
        fprintf(serPort, "Encoder %d\n", val);
    });

    fprintf(serPort, "Created\n");

    // Display with the Adafruit Library
    gfx.clearDisplay();

    taskManager.scheduleFixedRate(75, [] {
        gfx.setCursor(10, 10);
        gfx.print("hello world");
        gfx.setCursor(10, 25);
        gfx.fillRect(10, 25, 50, 10, BLACK);
        gfx.print((double) millis() / 1000.0);
        gfx.drawXBitmap(40, 40, iconWifiThreeBar, 16, 12, WHITE);
        gfx.drawCircle(100, 40, 10, WHITE);
        gfx.display();
        ThisThread::sleep_for(75);
    });

    while(!exitApp) {
        taskManager.runLoop();
    }
}