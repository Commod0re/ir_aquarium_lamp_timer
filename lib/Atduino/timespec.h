#ifndef _TIMESPEC_H_
#define _TIMESPEC_H_

#include <Arduino.h>

/* struct atduino_timespec_t
 */
struct atduino_timespec_t {
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint16_t year;
    // TODO: weekday?

    // operator overloads, so we can compare these directly
    inline bool operator==(struct atduino_timespec_t &other) {
        return year == other.year && month == other.month && day == other.day && hour == other.hour && minute == other.minute;
    }

    // < and <=, for ordering
    inline bool operator<(struct atduino_timespec_t &other) {
        // TODO: refactor this because the arrow pattern is super stinky
        if (year < other.year) {
            return true;
        }
        if (year == other.year) {
            if (month < other.month) {
                return true;
            }
            if (month == other.month) {
                if (day < other.day) {
                    return true;
                }

                if (day == other.day) {
                    if (hour < other.hour) {
                        return true;
                    }

                    if (hour == other.hour) {
                        return minute < other.minute;
                    }
                }
            }
        }

        return false;
    }

    inline bool operator<=(struct atduino_timespec_t &other) {
        return *this == other || *this < other;
    }
};


#endif /* _TIMESPEC_H_ */
