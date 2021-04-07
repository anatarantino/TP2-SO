#ifndef RTCTIME_H
#define RTCTIME_H

#include <stdint.h>

typedef enum{
    YEAR = 0x09,
    MONTH = 0x08,
    DAY = 0x07,
    DAYOFWEEK = 0x06,
    HOURS = 0X04,
    MINUTES = 0X02,
    SECONDS = 0X00
}time_type;

uint8_t getTime(time_type descriptor); //descriptor is for hours, minutes or seconds
uint8_t getRTC(uint64_t descriptor);

#endif