#include "adafruitDrawableTest_menu.h"
#include <TaskManagerIO.h>

TcGFXcanvas2 canvas(320, 40);

const color_t palette[] = {ILI9341_PURPLE, ILI9341_CYAN, ILI9341_YELLOW, ILI9341_GREEN};

void testAdafruitPixelBuffer() {
    gfx.fillScreen(0);

    canvas.fillScreen(0);
    //canvas.fillRect(10, 10, 50, 20, 1);
    //canvas.fillRect(100, 10, 50, 20, 2);
    drawCookieCutBitmap2bpp(&gfx, 0, 20, canvas.getBuffer(), 320, 40, 320, 0, 0, palette);

    delay(10000);
}

void setup() {
    SPI.setFrequency(20000000);
    SPI.begin();

    setupMenu();

    testAdafruitPixelBuffer();

    taskManager.scheduleFixedRate(150, [] {
        menuFloat.setFloatValue((rand() % 100000) / 100.0F);
    });
}

void loop() {
    taskManager.runLoop();
}
