#ifndef SYSCALL_H
#define SYSCALL_H

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
#define MALLOC 11
#define FREE 12
#define KILL 13
#define PS 14
#define BLOCK 15
#define NICE 16
#define GETPID 17
#define ADD_PROCESS 18
#define UNBLOCK 19
#define YIELD 20
#define SEM_OPEN 21
#define SEM_WAIT 22
#define SEM_POST 23
#define SEM_CLOSE 24
#define SEM_CHANGE_VALUE 25
#define SEMS_PRINT 26
#define PIPE_INIT 27
#define PIPE_OPEN 28
#define PIPE_CLOSE 29
#define PIPE_READ 30
#define PIPE_WRITE 31
#define PIPE_LIST 32
#define WAIT 33
#define SKIP 34

uint64_t syscalls(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9);

#endif