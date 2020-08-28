
#include <unity.h>
#include <EepromAbstraction.h>
#include "IoAbstraction.h"
#include "MockIoAbstraction.h"

// We can only reset the clock to a new value on AVR, this is very useful and allows us to ensure the
// rollover cases work properly at least for milliseconds. As millisecond and microsecond logic are very
// similar it gives some degree of confidence that it's working properly.
//
// Keep this test on it's own in this package. It messes around with the millisecond counter.

#ifdef __AVR__

const char * memToWrite = "This is a very large string to write into the rom to ensure it crosses memory boundaries in the rom";

void avrEepromWrapperTestCase() {
    AvrEeprom eeprom;
    eeprom.write8(700, 0xfe);
    eeprom.write16(701, 0xf00d);
    eeprom.write32(703, 0xbeeff00d);
    auto memLen = strlen(memToWrite) + 1;
    eeprom.writeArrayToRom(710, (const uint8_t*)memToWrite, memLen);

    char readBuffer[128];
    TEST_ASSERT_EQUAL(0xfe, eeprom.read8(700));
    TEST_ASSERT_EQUAL(0xf00d, eeprom.read16(701));
    TEST_ASSERT_EQUAL(0xbeeff00d, eeprom.read32(703));
    eeprom.readIntoMemArray((uint8_t*)readBuffer, 710, memLen);
    TEST_ASSERT_EQUAL_STRING(memToWrite, readBuffer);

    // now try other values to ensure the prior test worked
    eeprom.write8(700, 0xaa);
    TEST_ASSERT_EQUAL(0xaa, eeprom.read8(700));
}

void eepromClassWrapperTestCase() {
    AvrEeprom eeprom;
    eeprom.write8(720, 0xfe);
    eeprom.write16(721, 0xf00d);
    eeprom.write32(723, 0xbeeff00d);
    auto memLen = strlen(memToWrite) + 1;
    eeprom.writeArrayToRom(730, (const uint8_t*)memToWrite, memLen);

    char readBuffer[128];
    TEST_ASSERT_EQUAL(0xfe, eeprom.read8(720));
    TEST_ASSERT_EQUAL(0xf00d, eeprom.read16(721));
    TEST_ASSERT_EQUAL(0xbeeff00d, eeprom.read32(723));
    eeprom.readIntoMemArray((uint8_t*)readBuffer, 730, memLen);
    TEST_ASSERT_EQUAL_STRING(memToWrite, readBuffer);

    // now try other values to ensure the prior test worked
    eeprom.write8(720, 0xaa);
    TEST_ASSERT_EQUAL(0xaa, eeprom.read8(720));
}

void setup() {
    while(!Serial);
    delay(2000);
    UNITY_BEGIN();    // IMPORTANT LINE!
    RUN_TEST(avrEepromWrapperTestCase);
    RUN_TEST(eepromClassWrapperTestCase);
}

#else // not __AVR__

void setup() {
    delay(2000);
    UNITY_BEGIN();    // IMPORTANT LINE!

    TEST_FAIL_MESSAGE("Only supported on AVR");
}

#endif // __AVR__

void loop() {
    UNITY_END();
}