
#include "stm32f429zi/stm32f429i_discovery_lcd.h"
#include <IoLogging.h>

void setup() {
    BSP_LCD_Init();
    serlogF3(SER_DEBUG, "BSP inited size is", BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
}

void loop() {
    delay(1000);
}