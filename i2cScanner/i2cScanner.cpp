// --------------------------------------
// i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    https://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// Version 6, November 27, 2015.
//    Added waiting for the Leonardo serial communication.
//
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//

#include <Arduino.h>
#include <Wire.h>
#include <IoLogging.h>
#include <TaskManagerIO.h>

//IOLOG_MBED_PORT_IF_NEEDED(USBTX, USBRX)

void setup() {
    Serial.begin(115200);
    while(!Serial);
    serdebugF("I2C Scanner starting wire");

    //Wire.setSDA(12);
    //Wire.setSCL(13);
    Wire.begin();

    serdebugF("I2C Scanner running");
}


void loop()
{
    byte error, address;
    int nDevices;

    serdebugF("Scanning...");

    nDevices = 0;
    for(address = 1; address < 127; address++ )
    {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0)
        {
            serdebugFHex("I2C device found at address 0x", address);
            nDevices++;
        }
        else if (error==4)
        {
            serdebugFHex("Unknown error at address 0x", address);
        }
    }
    if (nDevices == 0) {
        serdebugF("No I2C devices found\n");
    } else {
        serdebugF("done\n");
    }

    taskManager.yieldForMicros(5000000UL);           // wait 5 seconds for next scan
}