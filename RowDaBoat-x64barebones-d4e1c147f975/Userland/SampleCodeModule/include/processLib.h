#ifndef PROCESSLIB_H
#define PROCESSLIB_H

#include <stdint.h>

uint64_t addProcess(void (*fn)(void));
void printProcess();
void blockProcess(uint64_t pid);
void niceProcess(uint64_t pid, uint64_t newPrio);
void killProcess(uint64_t pid);
uint64_t getPidProcess();
void loopProcess();

#endif