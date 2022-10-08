
#include "stm32f429zi/stm32f429i_discovery_lcd.h"
#include <IoLogging.h>
#include "tcMenuStChromaArt.h"

StChromaArtDrawable chromaArtDrawable;
extern LTDC_HandleTypeDef hLtdcHandler;

void setup() {
    Serial.begin(115200);
    Serial.println("Starting up");

    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(0, SDRAM_DEVICE_ADDR);
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    BSP_LCD_DrawPixel(100, 100, LCD_COLOR_WHITE);
    serlogF3(SER_DEBUG, "BSP inited size is", BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

/*    chromaArtDrawable.transaction(true, true);
    chromaArtDrawable.setColors(RGB(255, 255, 255), RGB(0, 0, 0));
    chromaArtDrawable.drawBox(Coord(0,0), Coord(240, 320), true);
    Coord line[2] = { Coord(10, 20), Coord(100, 20) };
    chromaArtDrawable.drawPolygon(line, 2, false);*/

}

void loop() {
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillRect(50, 100, 100, 100);
    delay(1000);
}