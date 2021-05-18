#include <syscallDispatcher.h>
#include <keyboard_driver.h>
#include <video_driver.h>
#include <prints.h>
#include <RTCtime.h>
#include <time.h>
#include <colors.h>
#include <memory.h>
#include <scheduler.h>
#include <sem.h>
#include <ipc.h>

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
            case MALLOC:
                return (uint64_t) memalloc((uint32_t)r->rdi);
            case FREE:
                memfree((void *)r->rdi);
                break;
            case KILL:
                kill((uint64_t)r->rdi);
                break;
            case PS:
                ps();
                break;
            case BLOCK:
                return block((uint64_t)r->rdi);
                break;
            case NICE:
                nice((uint64_t)r->rdi, (uint64_t)r->rsi);
                break;
            case GETPID:
                return getPid();
            case ADD_PROCESS:
                return addProcess((int (*)(int, char **))r->rdi,(int)r->rsi,(char **)r->rdx,(uint8_t)r->r10);
            case UNBLOCK:
                return unblock((uint64_t)r->rdi);  
            case YIELD:
                yield();
                break;
            case SEM_OPEN:
                return sem_open((char *)r->rdi, (uint64_t)r->rsi);
            case SEM_WAIT:
                return sem_wait((uint64_t)r->rdi);
            case SEM_POST:
                return sem_post((uint64_t)r->rdi);
            case SEM_CLOSE:
                return sem_close((uint64_t)r->rdi);
            case SEM_CHANGE_VALUE:
                sem_changeValue((uint64_t)r->rdi, (uint64_t)r->rsi);
                break;
            case SEMS_PRINT:
                sems_print();
                break;
            case PIPE_INIT:
                return pinit();
            case PIPE_OPEN:
                return popen((char *) r->rdi);
            case PIPE_CLOSE:
                return pclose((uint64_t)r->rdi);
            case PIPE_READ:
                return pread((uint64_t)r->rdi);
            case PIPE_WRITE:
                return pwrite((uint64_t)r->rdi, (char)r->rsi);
            case PIPE_LIST:
                plist();
                break;
            case WAIT:
                wait((uint64_t)r->rdi);
                break;
            case MEMDATA:
                memdata();
                break; 
        }    
    }
    return 0;
}

