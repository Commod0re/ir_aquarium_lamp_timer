#include "FeatherOLED_Aquarium.h"


void FeatherOLED_Aquarium::renderIcon_unknown(void) {
    // icon is a question mark
    drawLine(110, 0, 112, 0, WHITE);
    drawLine(113, 1, 113, 2, WHITE);
    drawLine(111, 3, 112, 3, WHITE);
    drawPixel(110, 4, WHITE);
    drawPixel(111, 6, WHITE);
    drawPixel(109, 1, WHITE);
}


void FeatherOLED_Aquarium::renderIcon_night(void) {
    // icon is a moon
    fillCircle(110, 6, 3, WHITE);
    fillCircle(108, 5, 2, BLACK);
}


void FeatherOLED_Aquarium::renderIcon_day(void) {
    // icon is a sun
    drawLine(110, 0, 110, 1, WHITE);
    drawLine(110, 9, 110, 10, WHITE);
    drawLine(105, 5, 106, 5, WHITE);
    drawLine(114, 5, 115, 5, WHITE);
    drawPixel(107, 2, WHITE);
    drawPixel(113, 2, WHITE);
    drawPixel(107, 8, WHITE);
    drawPixel(113, 8, WHITE);
    fillCircle(110, 5, 2, WHITE);
}


void FeatherOLED_Aquarium::renderIcon(void) {
    switch (_icon) {
    case unknown:
        renderIcon_unknown();
        break;
    case nighttime:
        renderIcon_night();
        break;
    case daytime:
        renderIcon_day();
        break;
    }
}


void FeatherOLED_Aquarium::refreshIcons(void) {
    fillRect(0, 0, 128, 8, BLACK);
    fillRect(0, 24, 128, 8, BLACK);
    renderBattery();
    renderRSSI();
    renderConnected();
    renderIPAddress();
    renderIcon();
    display();
}
