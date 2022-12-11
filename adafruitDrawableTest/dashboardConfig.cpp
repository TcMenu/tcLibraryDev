
#include "adafruitDrawableTest_menu.h"
#include "extras/DrawableDashboard.h"
#include <Adafruit_ILI9341.h>
#include <stockIcons/wifiAndConnectionIcons16x12.h>

DrawableDashboard* mainDashboard;
TitleWidget widgetConnected(iconsConnection, 2, 16, 12);

DashDrawParametersIntUpdateRange::IntColorRange drawEnumColorRanges[] {
        {ILI9341_YELLOW, ILI9341_RED, 0, 1},
        {ILI9341_CYAN, ILI9341_BLUE, 2, 3}
};
DashDrawParametersIntUpdateRange drawEnumWithIntRange(ILI9341_WHITE, ILI9341_BLACK, ILI9341_BLACK, ILI9341_YELLOW,
                                                       &RobotoMedium24, drawEnumColorRanges, 3);

DashDrawParametersIntUpdateRange::IntColorRange drawAnalogColorRanges[] {
        {ILI9341_LIGHTGREY, ILI9341_BLUE, 0, 50},
        {ILI9341_YELLOW, ILI9341_RED, 51, 100}
};
DashDrawParametersIntUpdateRange drawAnalogValueWithIntRange(ILI9341_WHITE, ILI9341_BLACK, ILI9341_BLACK, ILI9341_WHITE,
                                                       &RobotoMedium24, drawAnalogColorRanges, 3);
DashDrawParameters titleDrawParameters(ILI9341_WHITE, ILI9341_BLACK, &RobotoMedium24);

void setupDashboard() {
    mainDashboard = new DrawableDashboard(renderer.getDeviceDrawable(), &renderer, &widgetConnected,
                                          DrawableDashboard::DASH_ON_RESET_CLICK_EXIT);
    mainDashboard->setBaseColors(RGB(0, 0, 0), RGB(220, 220, 220));
    mainDashboard->addDrawingItem(&menuAnalog, Coord(0, 0), &drawAnalogValueWithIntRange, 10, nullptr, 10);
    mainDashboard->addDrawingItem(&menuEnum, Coord(0, 50), &drawEnumWithIntRange, 10, nullptr, 10);
    mainDashboard->addDrawingItem(&menuSettings, Coord(0, 100), &titleDrawParameters, 0, "Dashboard");
    renderer.setCustomDrawingHandler(mainDashboard);
}