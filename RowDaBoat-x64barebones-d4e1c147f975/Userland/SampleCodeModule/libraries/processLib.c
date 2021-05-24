// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <processLib.h>
#include <stdint.h>
#include <syscall.h>
#include <prints.h>
#include <timeRTC.h>

uint64_t addProcess(int (*fn)(int, char **), int argc, char ** argv, uint8_t fg,uint16_t * fd){
    return syscalls(ADD_PROCESS,(uint64_t)fn,argc,(uint64_t)argv,fg,(uint64_t)fd,0);
}

void printProcess(){
    syscalls(PS,0,0,0,0,0,0);
}

uint64_t blockProcess(uint64_t pid){
    return syscalls(BLOCK,pid,0,0,0,0,0);
}

uint64_t unblockProcess(uint64_t pid){
    return syscalls(UNBLOCK,pid,0,0,0,0,0);
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

void loopProcess(){
    uint64_t pid = getPidProcess();
    int ticks = 20;
    printf("\n");
    while(1){
        int final=ticks_elapsed() + ticks;
        while(ticks_elapsed() <= final);
        printInt(pid);
    }
}

void yield(){
    syscalls(YIELD,0,0,0,0,0,0);
}

void wait(uint64_t pid){
    syscalls(WAIT,pid,0,0,0,0,0);
}