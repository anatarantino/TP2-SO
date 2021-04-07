#ifndef SYSCALLDISPATCHER_H
#define SYSCALLDISPATCHER_H

#include <stdint.h>

#define READ 0
#define WRITE 1
#define SCREEN 2
#define CLEAR 3
#define REGISTERS 4
#define TIMERTC 5
#define DRAW 6
#define CLEARSPACE 7
#define WRITEONSCREEN 8
#define TICKSELAPSED 9
#define CHARINTERRUPT 10


#define TOTALREGS 16
#define CANT_SYS 11

typedef struct
{
      uint64_t rax, rdi, rsi, rdx, r10, r8, r9;
} t_registers;

uint64_t sysCallDispatcher();

#endif