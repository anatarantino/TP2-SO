// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <RTCtime.h>

uint8_t getTime(time_type descriptor){
    uint8_t aux = getRTC(descriptor);
    return aux / 16 * 10 + aux % 16;
}