#include <syscallDispatcher.h>
#include <keyboard_driver.h>
#include <video_driver.h>
#include <prints.h>
#include <RTCtime.h>
#include <time.h>
#include <colors.h>

//                          codigo        
//                          uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9
uint64_t syscallDispatcher(t_registers * r){
    if(r->rax >= 0 && r->rax < CANT_SYS){
        switch (r->rax)
        {
            case READ:
                return getChar();
            case WRITE:
                syscallWrite((char *) r->rdi,(uint64_t) r->rsi, (uint64_t) r->rdx, (uint64_t) r->r10);
                break;
            case SCREEN:
                initializeVideo((uint64_t) r->rdx, (uint64_t) r->r10);
                break;
            case CLEAR:
                clearScreen(BLACK);
                break;
            case REGISTERS:
                return (uint64_t) returnReg();
            case TIMERTC:
                return getTime((time_type)(r->rdi));
            case DRAW:
                drawPixel((int)r->r8,(int)r->r9,(int)r->rdx);
                break;
            case CLEARSPACE:
                clearSpace((uint32_t)r->rdi, (uint32_t) r->rsi,(uint32_t) r->rdx, (uint32_t) r->r8, (uint64_t)r->r10);
                break;
            case WRITEONSCREEN:
                syscallWriteOnCurrentPos((char *)r->rdi,(uint64_t)r->rsi,(uint64_t)r->rdx, (uint64_t) r->r10,(uint32_t)r->r8,(uint32_t)r->r9);
                break;
            case TICKSELAPSED:
                return ticks_elapsed();
            case CHARINTERRUPT:
                return waitCharInterruption();
        
        }    
    }
    return 0;
}

