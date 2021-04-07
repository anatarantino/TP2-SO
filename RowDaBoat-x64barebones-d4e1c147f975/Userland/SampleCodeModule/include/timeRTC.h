#ifndef TIMERTC_H
#define TIMERTC_H

#include <stdint.h>

typedef enum{
    YEAR = 9,
    MONTH = 8,
    DAY = 7,
    DAYOFWEEK = 6,
    HOURS = 4,
    MINUTES = 2,
    SECONDS = 0
}time_type;

uint8_t getTime(time_type descriptor);

uint64_t ticks_elapsed();

#endif