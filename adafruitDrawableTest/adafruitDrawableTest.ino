#include "adafruitDrawableTest_menu.h"
#include <TaskManagerIO.h>
#include "dashboardConfig.h"

void setup() {
    Serial1.begin(115200);
    SPI.setRX(4);
    SPI.setTX(3);
    SPI.setSCK(2);
    SPI.begin();

    setupMenu();

    setupDashboard();

    taskManager.scheduleFixedRate(150, [] {
        menuFloat.setFloatValue((rand() % 100000) / 100.0F);
        menuAnalog.setCurrentValue((rand() % 1000));
        widgetConnected.setCurrentState(rand() % 2);
    });
}

void loop() {
    taskManager.runLoop();
}
