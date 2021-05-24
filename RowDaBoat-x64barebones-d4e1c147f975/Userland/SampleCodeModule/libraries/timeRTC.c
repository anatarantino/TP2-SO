// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <timeRTC.h>
#include <syscall.h>
#include <libasm.h>

uint8_t getTime(time_type descriptor){
    return syscalls(TIMERTC,descriptor,0,0,0,0,0);
}

uint64_t ticks_elapsed(){
    return syscalls(TICKSELAPSED,0,0,0,0,0,0);
}

void sleep(uint16_t ticks){
    ticks += ticks_elapsed();
    while(ticks > ticks_elapsed()){
        _hlt();
    }  
    return;
}