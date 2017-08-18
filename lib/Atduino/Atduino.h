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
