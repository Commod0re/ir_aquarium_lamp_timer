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
