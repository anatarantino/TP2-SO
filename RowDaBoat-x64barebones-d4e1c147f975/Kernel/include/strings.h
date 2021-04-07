#ifndef STRINGS_H
#define STRINGS_H
#include <stdint.h>

int strlen(char * str);
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
uint32_t uintToBaseHexa(uint64_t value, char * buffer, uint32_t base, uint32_t length);

#endif