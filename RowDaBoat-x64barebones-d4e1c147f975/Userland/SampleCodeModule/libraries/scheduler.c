#include <scheduler.h>
#include <stdint.h>
#include <syscall.h>

void printProcess(){    //ps
    syscall(PS,0,0,0,0,0,0);
}

void blockProcess(uint64_t pid){
    syscall(BLOCK,pid,0,0,0,0,0);
}

void niceProcess(uint64_t pid, uint64_t newPrio){
    syscall(NICE,pid,newPrio,0,0,0,0);
}

void killProcess(uint64_t pid){
    syscall(KILL,pid,0,0,0,0,0);
}

uint64_t getPidProcess(){
    syscall(GETPID,0,0,0,0,0,0);
}