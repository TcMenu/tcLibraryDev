
#include <LiquidCrystalIO.h>
#include <IoAbstractionWire.h>
#include <stdio.h>

#define LCD_ADDRESS_I2C 0x27

LiquidCrystal lcd(0, 1, 2, 4, 5, 6, 7);

int main() {
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    ioaWireBegin(i2c_default);

    lcd.setIoAbstraction(ioFrom8574(LCD_ADDRESS_I2C, 0xff, defaultWireTypePtr));
    lcd.begin(16, 2);
    lcd.configureBacklightPin(3);
    lcd.backlight();

    lcd.print("hello over i2c!");

    //
    // when using this version of liquid crystal, it interacts (fairly) nicely with task manager.
    // instead of doing stuff in loop, we can schedule things to be done. But just be aware than
    // only one task can draw to the display. Never draw to the display in two tasks.
    //
    // You don't have to use the library with task manager like this, it's an option.
    //
    taskManager.scheduleFixedRate(250, [] {
        // set the cursor to column 0, line 1
        // (note: line 1 is the second row, since counting begins with 0):
        lcd.setCursor(0, 1);
        // print the number of seconds since reset:
        float secondsFraction = (float)millis() / 1000.0F;
        lcd.print(secondsFraction);
    });

    while(true) {
        taskManager.runLoop();
    }
}
