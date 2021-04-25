#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

void printProcess();
void blockProcess(uint64_t pid);
void niceProcess(uint64_t pid, uint64_t newPrio);
void killProcess(uint64_t pid);
uint64_t getPidProcess();

#endif