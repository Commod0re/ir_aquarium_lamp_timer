/* Copyright 2017 Michael Greene
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
