//
// Created by David Cherry on 26/07/2020.
//

#include "mbed.h"
#include <clib/u8x8.h>
#include <TaskManager.h>
#include "U8g2MbedHal.h"

BtreeList<long, U8g2MbedContainer> u8g2StructMapping;


extern "C" uint8_t u8x8_mbed_gpio_callback(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, U8X8_UNUSED void *arg_ptr) {
    auto container = u8g2StructMapping.getByKey(long(u8x8));
    if(container == nullptr) return 0;
noInterrupts()
    switch (msg) {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
            container->getPtr()->initGpio();
            break;

        case U8X8_MSG_DELAY_NANO:
            taskManager.yieldForMicros(arg_int == 0 ? 0 : 1);
            break;

        case U8X8_MSG_DELAY_10MICRO:
        case U8X8_MSG_DELAY_100NANO:
            /* not used at the moment */
            break;

        case U8X8_MSG_DELAY_MILLI:
            taskManager.yieldForMicros(arg_int * 1000);
            break;
        case U8X8_MSG_DELAY_I2C:
            /* arg_int is 1 or 4: 100KHz (5us) or 400KHz (1.25us) */
            taskManager.yieldForMicros((arg_int <= 2 ? 5 : 2) * 1000);
            break;
        case U8X8_MSG_GPIO_I2C_CLOCK:
        case U8X8_MSG_GPIO_I2C_DATA: {
            auto pin = (msg == U8X8_MSG_GPIO_I2C_DATA) ? container->getPtr()->getDataOut() : container->getPtr()->getClock();
            if (arg_int == 0) {
                pinMode(pin, OUTPUT);
                digitalWrite(pin, 0);
            } else {
                pinMode(pin, INPUT_PULLUP);
            }
            break;
        }
        default:
            return 0;
    }
    return 1;
}

extern "C" uint8_t u8x8_mbed_byte_callback(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr) {
    auto container = (u8g2StructMapping.getByKey(long(u8x8)));
    if(container == nullptr) return 0;

    switch (msg) {
        case U8X8_MSG_BYTE_SEND:
            container->getPtr()->writeToBus((uint8_t *) arg_ptr, (size_t) arg_int);
            break;
        case U8X8_MSG_BYTE_INIT:
        case U8X8_MSG_BYTE_SET_DC:
            // These parameters are not used with this particular hal
            break;
        case U8X8_MSG_BYTE_START_TRANSFER:
        case U8X8_MSG_BYTE_END_TRANSFER:
            container->getPtr()->commitIfNeeded();
            break;
        default:
            return 0;
    }
    return 1;
}
