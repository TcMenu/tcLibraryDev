
#include <IoAbstraction.h>
#include <SwitchInput.h>

//
// First we define the pins on which we connect encoder #1
//
const int enc1pinA = 2;// must be interrupt capable
const int enc1pinB = 6;
const int enc1OK = A3;

//
// Now define the pins on which we connect encoder #2
//
const int enc2pinA = 3;// must be interrupt capable
const int enc2pinB = 7;
const int enc2OK = A4;

void setup() {
    Serial.begin(115200);

    Serial.println("Starting setup");

    // initialise switch without using interrupts, you could use interrupts if you wanted though here.
    switches.initialise(ioUsingArduino(), true);

    Serial.println("Adding switches");

    // Now we configure both the encoder switches

    switches.addSwitch(enc1OK, [] (pinid_t pin, bool held) {
        Serial.println("The encoder button was pressed.");
    }, NO_REPEAT);

    switches.addSwitch(enc2OK, [] (pinid_t pin, bool held) {
        Serial.println("The 2nd encoder button was pressed.");
    }, 20);

    // then we set up the encoders.

    Serial.println("Setup encoder 1");

    setupRotaryEncoderWithInterrupt(enc1pinA, enc1pinB, [] (int newVal) {
        Serial.print("Encoder 1 change ");
        Serial.println(newVal);
    });
    switches.getEncoder()->changePrecision(200, 100);

    Serial.println("Setup encoder 2");

    auto secondEncoder = new HardwareRotaryEncoder(enc2pinA, enc2pinB, [](int newVal) {
        Serial.println("Encoder 2 change ");
        Serial.println(newVal);
    });

    taskManager.addInterrupt(ioUsingArduino(), enc1pinB, CHANGE);
    switches.setEncoder(1, secondEncoder);

    Serial.println("Finished setup");
}

void loop() {
    taskManager.runLoop();
}