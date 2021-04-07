#include <RTCtime.h>

uint8_t getTime(time_type descriptor){
    uint8_t aux = getRTC(descriptor);
    return aux / 16 * 10 + aux % 16;
}