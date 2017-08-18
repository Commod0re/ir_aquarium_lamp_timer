#include <ESP8266WiFi.h>
#include "Atduino.h"

#define _iso8601str(t) (String(t.year) + "-" + String(t.month) + "-" + String(t.day) + " " + (t.hour < 10?"0":"") + String(t.hour) + ":" + (t.minute < 10?"0":"") + String(t.minute))


/*
 * pop_task
 * pop a task from the queue, shifting everything else after it back by 1
 * then decrement _qlen
 */
void Atduino::pop_task(uint8_t idx) {
    if (idx > _qlen) {
        // index is out of bounds
        return;
    }

    for (uint8_t p = idx, i = idx + 1; i < _qlen; ++p, ++i) {
        // copy everything from _queue[i] to _queue[p]
        _queue[p].timespec.minute = _queue[i].timespec.minute;
        _queue[p].timespec.hour = _queue[i].timespec.hour;
        _queue[p].timespec.day = _queue[i].timespec.day;
        _queue[p].timespec.month = _queue[i].timespec.month;
        _queue[p].timespec.year = _queue[i].timespec.year;
        _queue[p].taskfn = _queue[i].taskfn;
    }

    _qlen--;
}


/*
 * shift_back
 * shift everything in _queue starting with idx back by 1
 * then increment _qlen
 */
void Atduino::shift_back(uint8_t idx) {
    if (_qlen > 0) {
        for (uint8_t n = _qlen + 1, i = _qlen; i >= idx; --n, --i) {
            _queue[n].timespec.minute = _queue[i].timespec.minute;
            _queue[n].timespec.hour = _queue[i].timespec.hour;
            _queue[n].timespec.day = _queue[i].timespec.day;
            _queue[n].timespec.month = _queue[i].timespec.month;
            _queue[n].timespec.year = _queue[i].timespec.year;
            _queue[n].taskfn = _queue[i].taskfn;
        }
    }
    _qlen++;
}


void Atduino::add_task(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, void (*taskfn)(struct atduino_timespec_t &scheduled)) {
    struct atduino_timespec_t target = {minute, hour, day, month, year};
    uint8_t i;

    // otherwise, find where in the queue to insert ourselves
    for (i = 0; i < _qlen; ++i) {
        if (target <= _queue[i].timespec) {
            // found it!
            break;
        }
    }

    Serial.println("insert task at idx: " + String(i) + "/" + String(_qlen) + " (" + _iso8601str(target) + ")");
    shift_back(i);

    _queue[i].timespec.minute = minute;
    _queue[i].timespec.hour = hour;
    _queue[i].timespec.day = day;
    _queue[i].timespec.month = month;
    _queue[i].timespec.year = year;
    _queue[i].taskfn = taskfn;
}


void Atduino::check(struct atduino_timespec_t &curtime) {
    // check if it's time to run the next task in the queue
    if (_qlen > 0 && _queue[0].timespec <= curtime) {
        Serial.println(_iso8601str(_queue[0].timespec) + " <= " + _iso8601str(curtime));
        Serial.print("dispatching next task function: ");
        _queue[0].taskfn(_queue[0].timespec);
        pop_task(0);

        Serial.println("next task at: " + _iso8601str(_queue[0].timespec));
    }

    // Serial.println("next task at: " + String(_queue[0].timespec.year) + "-" + String(_queue[0].timespec.month) + "-" + String(_queue[0].timespec.day) +
    //                " " + String(_queue[0].timespec.hour) + ":" + String(_queue[0].timespec.minute));
}
