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
