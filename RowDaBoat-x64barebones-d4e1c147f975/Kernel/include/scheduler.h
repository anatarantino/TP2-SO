#ifndef SCHEDULER_H

#include <stdint.h>

typedef struct stackFrame{
      uint64_t gs;
      uint64_t fs;
      uint64_t r15;
      uint64_t r14;
      uint64_t r13;
      uint64_t r12;
      uint64_t r11;
      uint64_t r10;
      uint64_t r9;
      uint64_t r8;
      uint64_t rsi;
      uint64_t rdi;
      uint64_t rbp;
      uint64_t rdx;
      uint64_t rcx;
      uint64_t rbx;
      uint64_t rax;

      uint64_t rip;
      uint64_t cs;
      uint64_t rflags;
      uint64_t rsp;
      uint64_t ss;
      uint64_t bp;
} stackFrame;



void initializeSch();
uint64_t scheduler();
uint64_t addProcess(void (*process)(int,char**),int argc, char** argv, uint8_t fg);
uint64_t kill(uint64_t pid);
uint64_t unblock(uint64_t pid);
uint64_t block(uint64_t pid);
void ps();
void nice(uint64_t pid, uint64_t newPrio);
uint64_t getPid();
void killLoop();

#endif