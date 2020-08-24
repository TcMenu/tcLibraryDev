//
// Created by David Cherry on 24/08/2020.
//

#ifndef TCMENU_MBED_ARDUINO_H
#define TCMENU_MBED_ARDUINO_H

#include <IoAbstraction.h>

extern IoAbstractionRef globalAbstraction;

inline void setDefaultIoDevice(IoAbstractionRef ref) {
    globalAbstraction = ref;
}

inline void pinMode(pinid_t pin, uint8_t mode) {
    ioDevicePinMode(globalAbstraction, pin, mode);
}

inline void digitalWrite(pinid_t pin, uint8_t state) {
    ioDeviceDigitalWriteS(globalAbstraction, pin, state);
}

inline int digitalRead(pinid_t pin) {
    return ioDeviceDigitalReadS(globalAbstraction, pin);
}

#endif //TCMENU_MBED_ARDUINO_H
