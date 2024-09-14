
#include <TaskManagerIO.h>
#include <SwitchInput.h>
#include <extras/SPIHelper.h>
#include <hardware/i2c.h>
#include "PlatformDeterminationWire.h"
#include "EepromAbstractionWire.h"

taskid_t millisTask;
int counter = 0;

#define BUTTON_EEPROM_PIN 22
#define ENC_A_PIN 16
#define ENC_B_PIN 17
#define ENC_OK_PIN 21

#define I2C_SDA 8
#define I2C_SCL 9

I2cAt24Eeprom myEeprom(0x50, PAGESIZE_AT24C128);

void eepromTest();

int main() {
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ioaWireBegin(i2c_default);
    
    IOLOG_START_SERIAL
    serlogF(SER_DEBUG, "Starting IoAbstraction demo");
    serlogF(SER_DEBUG, "Press pin 5 for eeprom tests")

    switches.init(internalDigitalIo(), SWITCHES_POLL_EVERYTHING, true);

    switches.addSwitch(BUTTON_EEPROM_PIN, [](pinid_t key, bool held) {
        serlogF3(SER_DEBUG, "Switches press ", key, held);
        eepromTest();
    }, NO_REPEAT);

    switches.addSwitch(ENC_OK_PIN, [](pinid_t key, bool held) {
        serlogF3(SER_DEBUG, "OK press ", key, held);
    });

    setupRotaryEncoderWithInterrupt(ENC_A_PIN, ENC_B_PIN, [](int val) {
        serlogF2(SER_DEBUG, "Encoder = ", val);
    });
    switches.changeEncoderPrecision(100, 50);

    while(1) {
        taskManager.runLoop();
    }
}

const char* toWrite = "This is some text to write into the eeprom storage unit that should cross a buffer segment size";

void eepromTest() {
    myEeprom.write8(500, 0xab);
    myEeprom.write8(501, 0xcd);
    myEeprom.write16(502, 0xface);
    myEeprom.write32(506, 0xf00dd00d);
    size_t len = strlen(toWrite);
    myEeprom.writeArrayToRom(510, (const uint8_t*)toWrite, len);
    serlogF2(SER_DEBUG, "EEPROM write done error=", myEeprom.hasErrorOccurred());
    
    
    serlogFHex2(SER_DEBUG, "EEPROM read 8 = ", myEeprom.read8(500), myEeprom.read8(501));
    serlogFHex(SER_DEBUG, "EEPROM read 16 = ", myEeprom.read8(502));
    serlogFHex(SER_DEBUG, "EEPROM read 32 = ", myEeprom.read8(506));
    serlogFHex(SER_DEBUG, "EEPROM read 32 = ", myEeprom.read8(506));
    
    uint8_t data[128];
    myEeprom.readIntoMemArray(data, 510, len);
    data[len] = 0;
    serlogF2(SER_DEBUG, "EEPROM read array = ", (char*)data);
}