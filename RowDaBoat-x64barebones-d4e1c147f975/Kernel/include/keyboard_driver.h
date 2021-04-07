#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#include <stdint.h>

void keyboard_handler(uint64_t rsp);
char getChar();
uint64_t * returnReg();
char waitCharInterruption();

#endif