#include <ipc.h>
#include <sem.h>
#include <memory.h>
#include <prints.h>
#include <keyboard_driver.h>
#include <scheduler.h>
#include <strings.h>

static int pcreate(char * name);
static void printPipe(pipe_node * pipe);
static int getIndex();
static int searchByName(char * name);
static int valid(int index);

static pipes_array pipes;

int pinit(){ 
    if((pipes.lockPipe = sem_open("lockPipe", 1)) == ERROR){
        return ERROR;
    } 
    return 0;
}

static int pcreate(char * name){
    int index = getIndex();
    if(index == -1){
        return -1;
    }
    pipe_node* pipe = &pipes.parray[index];
    strcopy(pipe->name, name);
    pipe->activity = 1;
    pipe->processes = 0;
    pipe->writePos = 0;
    pipe->readPos = 0;

    int len = strleng(name);
    char str[len+2];

    strcopycat(str, name, "_r");
    pipe->readSem = sem_open(str,0);
    if(pipe->readSem == ERROR){
        return ERROR;
    }
    strcopycat(str, name, "_w");
    pipe->writeSem = sem_open(str, MAX_BUFF);
    if(pipe->writeSem == ERROR){
        return ERROR;
    }
    return index;
}

static int getIndex(){
    for(int i=0; i<PIPES_MAX; i++){
        if(!pipes.parray[i].activity){
            return i;
        }
    }
    return -1;
}

int popen(char * name){
    if(name == NULLP){
        return ERROR;
    }
    if((sem_wait(pipes.lockPipe)) == ERROR){
        return ERROR;
    }
    int index = searchByName(name);
    if(index == -1){
        index = pcreate(name);
        if(index == -1){
            sem_post(pipes.lockPipe);
            return -1;
        }
    }

    pipes.parray[index].processes++;

    sem_post(pipes.lockPipe);
    
    return index+1;
}

static int searchByName(char * name){
    for(int i=0; i<PIPES_MAX; i++){
        if(pipes.parray[i].activity){
            if(strcomp(name, pipes.parray[i].name) == 0){
                return i;
            }
        }
    }
    return -1;
}

int pclose(int index){
    index--;
    if(!valid(index)){
        return -1;
    }
    if((sem_wait(pipes.lockPipe)) == ERROR){
        return ERROR;
    }
    pipe_node* pipe = &pipes.parray[index];
    pipe->processes--;
    if(pipe->processes > 0){
        if((sem_post(pipes.lockPipe)) == ERROR){
            return ERROR;
        }
        return 1;
    }
    if((sem_close(pipe->readSem) == ERROR) || (sem_close(pipe->writeSem) == ERROR)){
        return ERROR;
    }
    pipe->activity = 0;
    if((sem_post(pipes.lockPipe)) == ERROR){
        return ERROR;
    }
    return 1;
}

char pread(int index){
    index--;
    if(!valid(index)){
        return -1;
    }
    pipe_node* pipe = &pipes.parray[index];
    if(sem_wait(pipe->readSem) == ERROR){
        return ERROR;
    }
    char ret = pipe->buff[pipe->readPos];
    pipe->readPos = (pipe->readPos + 1) % MAX_BUFF; 
    if(sem_post(pipe->writeSem) == ERROR){
        return ERROR;
    }
    return ret;
}

int pwriteChar(int index, char c){
    index--;
    if(!valid(index)){
        return -1;
    }
    pipe_node* pipe = &pipes.parray[index];
    if(sem_wait(pipe->writeSem) == ERROR){
        return ERROR;
    }
    pipe->buff[pipe->writePos] = c;
    pipe->writePos = (pipe->writePos + 1) % MAX_BUFF; 
    if(sem_post(pipe->readSem) == ERROR){
        return ERROR;
    }
    return 0;
}

int pwriteStr(int index, char * str){
    index--;
    if(!valid(index)){
        return -1;
    }
    while(*str!= 0){
        pwriteChar(index+1, *str++);
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
    sem_print(pipe->readSem);
    printNewLine();
    sem_print(pipe->writeSem);
    printNewLine();
}

char getCharP(){
    uint64_t input = getInput();
    if(input == 0){
        if(getFg()){
            return getChar();
        }
        else{
            return -1;
        }
    }
    return pread(input);
}

void printfP(char* string, uint8_t length, uint64_t f_color, uint64_t bg_color){
    uint64_t output = getOutput();
    if(output == 0){
        syscallWrite(string,length,f_color,bg_color);
    }
    else{
        pwriteStr(output, string);
    }
}

static int valid(int index){
    if(index < 0 || index >= PIPES_MAX || !pipes.parray[index].activity){
        return 0;
    }
    return 1;
}