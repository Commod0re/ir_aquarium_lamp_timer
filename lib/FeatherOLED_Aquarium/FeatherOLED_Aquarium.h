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

#ifndef _FEATHEROLED_AQUARIUM_H_
#define _FEATHEROLED_AQUARIUM_H_

#include <Adafruit_FeatherOLED_WiFi.h>

enum AquariumIcon {
    unknown,
    nighttime,
    daytime
};

class FeatherOLED_Aquarium : public Adafruit_FeatherOLED_WiFi {
private:
    void renderIcon_unknown(void);
    void renderIcon_night(void);
    void renderIcon_day(void);

protected:
    AquariumIcon _icon;

    void renderIcon (void);

public:
    // Constructor
    FeatherOLED_Aquarium(TwoWire *wire=&Wire, int reset=-1): Adafruit_FeatherOLED_WiFi(wire, reset) {
        _icon = unknown;
        _batteryVisible = false;
    }

    void setIcon(AquariumIcon icon) { _icon = icon; }
    void refreshIcons(void);

    // battery icon DISABLED
    void setBattery (float vbat) {}
    void setBatteryVisible (bool enable) { _batteryVisible = false; }
    void setBatteryIcon (bool enable) { _batteryIcon = false; }
};

#endif /* _FEATHEROLED_AQUARIUM_H_ */
