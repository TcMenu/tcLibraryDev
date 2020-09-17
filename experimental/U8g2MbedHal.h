//
// Created by David Cherry on 26/07/2020.
//

#ifndef TCMENU_PLUGIN_U8G2MBEDHAL_H
#define TCMENU_PLUGIN_U8G2MBEDHAL_H

//class U8G2_SSD1306_128X64_NONAME_F_3W_SW_SPI : public U8G2 {
//  public: U8G2_SSD1306_128X64_NONAME_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
//    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, rotation, u8x8_byte_arduino_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
//    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
//  }
//};

#include <mbed.h>
#include <U8g2lib.h>

extern "C" uint8_t u8x8_mbed_gpio_callback(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
extern "C" uint8_t u8x8_mbed_byte_callback(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr);

class U8g2Mbed;

class U8g2Mbed : public U8G2 {
protected:
    pinid_t clock = NC;
    pinid_t dataOut = NC;
    pinid_t dataIn = NC;
    pinid_t cs = NC;
    pinid_t reset = NC;
public:
    virtual void initGpio() = 0;
    virtual void writeToBus(uint8_t* data, size_t len) = 0;
    virtual void commitIfNeeded() = 0;

    pinid_t getClock() const { return clock; }
    pinid_t getDataOut() const { return dataOut; }
    pinid_t getDataIn() const { return dataIn; }
    pinid_t getCS() const { return cs; }
    pinid_t getReset() const { return reset; }

    static U8g2Mbed *theInstance;
};

class U8g2MbedI2c_1306_128x64_i2c : public U8g2Mbed {
private:
    uint8_t buffer[128];
    size_t bufferPos = 0;
    I2C *i2c;
    uint8_t i2cAddr;
public:
    U8g2MbedI2c_1306_128x64_i2c(I2C* i2c_, const u8g2_cb_t *rotation, pinid_t reset = NC) {
        theInstance = this;
        i2c = i2c_;
        u8g2_Setup_ssd1306_i2c_128x64_noname_1(&u8g2, rotation, u8x8_mbed_byte_callback, u8x8_mbed_gpio_callback);
        this->reset = reset;
    }

    void initGpio() override {
        if(reset != NC) {
            pinMode(reset, OUTPUT);
            digitalWrite(reset, LOW);
            taskManager.yieldForMicros(50);
            digitalWrite(reset, HIGH);
        }
    }

    void writeToBus(uint8_t* data, size_t len) override {
        i2c->write(getU8x8()->i2c_address << 1, (const char*)data, len);

    }
    void commitIfNeeded() override {
    }
};



#endif //TCMENU_PLUGIN_U8G2MBEDHAL_H
