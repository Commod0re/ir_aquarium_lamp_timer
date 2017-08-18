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

#include <TimeLib.h>
#include "util.h"


/*
 * get_dst_start
 * find the second Sunday in March for the given year
 */
uint8_t get_dst_start(uint16_t year) {
    tmElements_t td = {0};
    time_t ts;
    uint8_t days_to_sunday = 0;

    // get the time_t for March 1st of the specified year
    td.Year = year - 1970;
    td.Month = 3;
    td.Day = 1;
    ts = makeTime(td);

    // now we can tell which day of the week March 1st is by calling weekday(ts)
    // if we set Sunday == 0 instead of 1, we can do some unsigned math:
    days_to_sunday -= (weekday(ts) - 1);
    days_to_sunday = (days_to_sunday <= 6) ? days_to_sunday : days_to_sunday + 7;

    // now we know that DST starts on the second sunday of March,
    // so we just add 1 week to that and voila!
    return days_to_sunday + 8;
}


/*
 * get_dst_end
 * find the first Sunday in November for the given year
 */
uint8_t get_dst_end(uint16_t year) {
    tmElements_t td = {0};
    time_t ts;
    uint8_t days_to_sunday = 0;

    // get the time_t for November 1st of the specified year
    td.Year = year - 1970;
    td.Month = 3;
    td.Day = 1;
    ts = makeTime(td);

    // now we can tell which day of the week November 1st is by calling weekday(ts)
    // if we set Sunday == 0 instead of 1, we can do some unsigned math:
    days_to_sunday -= (weekday(ts) - 1);
    days_to_sunday = (days_to_sunday <= 6) ? days_to_sunday : days_to_sunday + 7;

    // now we know that DST starts on the first Sunday of November
    // so just return days_to_sunday
    return days_to_sunday + 1;
}
