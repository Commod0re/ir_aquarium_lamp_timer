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
    FeatherOLED_Aquarium(int reset=-1): Adafruit_FeatherOLED_WiFi(reset) {
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
