#include <ipc.h>
#include <sem.h>
#include <memory.h>
#include <strings.h>

static pipes_array pipes;

int pinit(){
    if((pipes.newPipe = sem_open("newPipe", 1) == NULL) || (pipes.endPipe = sem_open("endPipe", 1) == NULL)){
        return ERROR;
    }
    pipes.dim = PIPES_MAX;
    pipes.size = 0;
    return 0;
}

int pcreate(char * name){
    
}

int popen(char * name){
    sem_wait(pipes.newPipe);
    int i;
    for(i=0; i<pipes.dim; i++){
        if(pipes.parray[i].activity){
            if(strcmp(pipes.parray[i].name, name) == 0){
                pipes.parray[i].processes++;
                sem_post(pipes.newPipe);
                return i;
            }
        }
    }
    if(i == pipes.dim){
        i = pcreate(name);
        if ( i != -1){
            sem_post(pipes.newPipe);
            return i;
        }
    }
    
    return i;
}

int pclose(){
    
}

int pread(){
    
}

int pwrite(){
    
}

int plist(){
    
}