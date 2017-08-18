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
 
#ifndef _ATDUINO_H_
#define _ATDUINO_H_

#include <Arduino.h>
#include "timespec.h"

// until I learn how to do variable length arrays safely in arduinoland
#define ATD_QUEUE_SIZE 6


struct atduino_task_t {
    struct atduino_timespec_t timespec;
    void (*taskfn)(struct atduino_timespec_t &scheduled);
};


class Atduino {
protected:
    struct atduino_task_t _queue[ATD_QUEUE_SIZE];
    uint8_t _qlen;

    void pop_task(uint8_t idx);
    void shift_back(uint8_t idx);

public:
    // constructor
    Atduino(void) {

    }

    void add_task(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, void (*taskfn)(struct atduino_timespec_t &scheduled));
    void check(struct atduino_timespec_t &curtime);
};

#endif /* _ATDUINO_H_ */
