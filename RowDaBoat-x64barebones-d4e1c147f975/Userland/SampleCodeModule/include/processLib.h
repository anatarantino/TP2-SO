#ifndef PROCESSLIB_H
#define PROCESSLIB_H

#include <stdint.h>

uint64_t addProcess(void (*fn)(int, char **), int argc, char ** argv, uint8_t fg);
void printProcess();
uint64_t blockProcess(uint64_t pid);
uint64_t unblockProcess(uint64_t pid);
void niceProcess(uint64_t pid, uint64_t newPrio);
void killProcess(uint64_t pid);
uint64_t getPidProcess();
void loopProcess();
void yield();
void skip();

#endif