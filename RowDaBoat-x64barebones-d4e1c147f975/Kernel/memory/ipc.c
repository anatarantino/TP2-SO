#include <ipc.h>
#include <sem.h>
#include <memory.h>
#include <prints.h>

static int pcreate(char * name, int index);
static void printPipe(pipe_node * pipe);

static pipes_array pipes;

int pinit(){ 
    pipes.newPipe = sem_open("newPipe", 1);
    pipes.endPipe = sem_open("endPipe", 1);
    if( pipes.newPipe == NULL || pipes.endPipe == NULL){
        return ERROR;
    }
    return 0;
}

static int pcreate(char * name, int index){
    if(index == -1){
        return ERROR;
    }
    pipe_node* pipe = &pipes.parray[index];
    strcopy(pipe->name, name);
    pipe->activity = 1;
    pipe->processes = 0;
    pipe->writePos = 0;
    pipe->readPos = 0;

    int len = strlen(name);
    char str[len+2];

    strcopycat(str, name, "_r");
    pipe->readSem = sem_open(str,0);
    if(pipe->readSem == NULL){
        return ERROR;
    }
    strcopycat(str, name, "_w");
    pipe->writeSem = sem_open(str, MAX_BUFF);
    if(pipe->writeSem == NULL){
        return ERROR;
    }
    return 0;
}

int popen(char * name){
    if(name == NULL){
        return ERROR;
    }
    sem_wait(pipes.newPipe);
    int i;
    int available = -1;
    for(i=0; i<PIPES_MAX; i++){
        if(pipes.parray[i].activity == 1){
            if(strcmp(pipes.parray[i].name, name) == 0){
                pipes.parray[i].processes++;
                sem_post(pipes.newPipe);
                return i;
            }
        }
        else{
            if(available == -1){
                available = i;
            }
        }
    }
    if(i == PIPES_MAX){
        i = pcreate(name, available);
        if ( i != -1){
            sem_post(pipes.newPipe);
            return i;
        }
    }
    
    return i;
}

int pclose(int index){
    if(index < 0 || index >= PIPES_MAX || pipes.parray[index].activity == 0){
        return ERROR;
    }
    sem_wait(pipes.endPipe);
    pipe_node* pipe = &pipes.parray[index];
    pipe->processes--;
    if(pipe->processes > 0){
        sem_post(pipes.endPipe);
        return 0;
    }
    if((sem_close(pipe->readSem) == -1) || (sem_close(pipe->writeSem) == -1)){
        return ERROR;
    }
    pipe->activity = 0;
    sem_post(pipes.endPipe);
    return 0;
}

char pread(int index){
    if(index < 0 || index >= PIPES_MAX || pipes.parray[index].activity == 0){
        return ERROR;
    }
    pipe_node* pipe = &pipes.parray[index];
    if(sem_wait(pipe->readSem) == -1){
        return ERROR;
    }
    char ret = pipe->buff[pipe->readPos];
    pipe->readPos = (pipe->readPos + 1) % MAX_BUFF; 
    if(sem_post(pipe->writeSem) == -1){
        return ERROR;
    }
    return ret;
}

int pwrite(int index, char c){
    if(index < 0 || index >= PIPES_MAX || pipes.parray[index].activity == 0){
        return ERROR;
    }
    pipe_node* pipe = &pipes.parray[index];
    if(sem_wait(pipe->writeSem) == -1){
        return ERROR;
    }
    pipe->buff[pipe->writePos] = c;
    pipe->writePos = (pipe->writePos + 1) % MAX_BUFF; 
    if(sem_post(pipe->readSem) == -1){
        return ERROR;
    }
    return 0;
}

void plist(){
    printf("Pipes: \n\n");
    for(int i=0; i<PIPES_MAX; i++){
        if(pipes.parray[i].activity == 1){
            printPipe(&pipes.parray[i]);
        }
    }
}

static void printPipe(pipe_node * pipe){
    printf("Pipe name: ");
    printf(pipe->name);
    printNewLine();

    printf("Processes: ");
    printInt(pipe->processes);
    printNewLine();

    printf("Semaphores blocked: \n");
    GIT(pipe->readSem);
    printNewLine();
    sem_print(pipe->writeSem);
    printNewLine();
}