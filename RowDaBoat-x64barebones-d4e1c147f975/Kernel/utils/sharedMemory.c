#include <sharedMemory.h>
#include <memory.h>
#include <lib.h>


static uint64_t * memory[10]; 

uint64_t * shm_open(uint64_t id, uint32_t size){
    if(memory[id]==0){
        memory[id]=memalloc(size);
        memset(memory[id],0,size);
    }
    return memory[id];
}

