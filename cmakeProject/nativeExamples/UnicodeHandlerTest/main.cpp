
#include <PlatformDetermination.h>
#include <Adafruit_GFX.h>
#include <tcUnicodeHelper.h>
#include <tcUnicodeAdaGFX.h>
#define PROGMEM
//#include "Fonts/OpenSansCyrillicLatin12.h"
#include <Fonts/OpenSansRegular8pt.h>

#define OLED_IS_I2C false

#if OLED_IS_I2C
#include <oled/AdafruitSSD1306I2c.h>
// for I2C
#define OLED_ADDRESS_I2C 0x3C
AdafruitSSD1306I2c* display;
#else
#include <oled/AdafruitSSD1306Spi.h>
// for SPI
#define OLED_CS_PIN 15
#define OLED_DC_PIN 14
#define OLED_RST_PIN 13
#define OLED_SPI_TX_PIN 3
#define OLED_SPI_RX_PIN 4
#define OLED_SPI_CLK_PIN 2
AdafruitSSD1306Spi* display;
#endif

UnicodeFontHandler* fontHandler;

int main() {
    stdio_init_all();

#if OLED_IS_I2C
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    ioaWireBegin(i2c_default);
    display = new AdafruitSSD1306I2c(defaultWireTypePtr, 0xff, OLED_ADDRESS_I2C, 64, 128, ENCMODE_UTF8, SH_1106);
#else
    // This example will use SPI0 at 10MHz.
    spi_init(spi_default, SPI_TEN_MHZ);
    gpio_set_function(OLED_SPI_CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(OLED_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(OLED_SPI_RX_PIN, GPIO_FUNC_SPI);
    display = new AdafruitSSD1306Spi(
            SPIWithSettings(spi_default, OLED_CS_PIN, SPI_TEN_MHZ),
            OLED_DC_PIN, OLED_RST_PIN, 64, 128, ENCMODE_UTF8, SSD_1306);
#endif

    display->begin();
    display->clearDisplay();

    display->drawLine(0, 18, 127, 18, WHITE);
    fontHandler = new UnicodeFontHandler(newAdafruitTextPipeline(display), ENCMODE_UTF8);
    fontHandler->setCursor(0,18);
    fontHandler->setDrawColor(WHITE);
    fontHandler->setFont(OpenSansRegular8pt);
    fontHandler->print("AaYyGg@\'");
    display->display();

    taskManager.schedule(repeatMillis(250), [] {
        display->fillRect(0, 25, 128, 44, BLACK);
        fontHandler->setCursor(0, 40);
        fontHandler->print(millis());
        display->display();
    });

    while(true) {
        taskManager.runLoop();
    }
}