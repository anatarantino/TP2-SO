#include <scheduler.h>
#include <stdint.h>
#include <syscall.h>

void printProcess(){    //ps
    syscalls(PS,0,0,0,0,0,0);
}

void blockProcess(uint64_t pid){
    syscalls(BLOCK,pid,0,0,0,0,0);
}

void niceProcess(uint64_t pid, uint64_t newPrio){
    syscalls(NICE,pid,newPrio,0,0,0,0);
}

void killProcess(uint64_t pid){
    syscalls(KILL,pid,0,0,0,0,0);
}

uint64_t getPidProcess(){
    return syscalls(GETPID,0,0,0,0,0,0);
}