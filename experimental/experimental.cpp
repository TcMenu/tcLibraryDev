/**
 * My environment is platformIO (CLion IDE), I've tried this on various versions of the Stm32Duino platform back to
 * about a year ago. In one project it reliably fails, in the other it works perfectly.
 *
 * Unfortunately, I cannot run the sketch in Arduino as I'd have to add the board I'm using to the boards file there.
 * I know the board is working properly when the marked line below is commented out.
 *
 * It's possible I've made a mistake and would be very very glad to stand corrected, or even that the problem lies
 * within my build chain (although I've got this problem on two devices), or even that there is a problem elsewhere
 * in the tool chain.
 */

#include <Arduino.h>

// First we create an interface that we will use later - to create a virtual table.
class MyAbstraction {
public:
    virtual void pinDirection(uint8_t pin, uint8_t mode)=0;
};

// Then we create the implementation that has the pinDirection method implementation.
class MyAbstractionImpl : public MyAbstraction {
public:
    void pinDirection(uint8_t pin, uint8_t mode) override {
        pinMode(pin, mode);
    }
};

volatile int counter;

void interruptCallback() {
    // in the interrupt we just increment the counter
    counter++;
}

void setup() {
    // commenting out this line makes the sketch work in the environment that fails.
    auto* digitalIo = new MyAbstractionImpl();
    Serial.begin(115200);

    // If I replace the above with the below line everything works as expected and the interrupt is registered.
    pinMode(PC8, INPUT_PULLUP);

    // here we register the interrupt
    ::attachInterrupt(digitalPinToInterrupt(PC8), interruptCallback, CHANGE);
}

void loop() {
    // we just print out the contents of the counter every second.
    Serial.print("Counter is ");
    Serial.println(counter);
    delay(1000);
}
