#include <sem.h>
#include <stdint.h>
#include <memory.h>
#include <lib.h>
#include <strings.h>
#include <scheduler.h>

#define SEM_MAX 15
#define MAX_LEN 20
#define ERROR -1
#define LOCK 1
#define UNLOCK 0

typedef struct sem_t{
    char name[MAX_LEN];
    uint64_t value;
    struct sem_t * next;
    uint64_t lock;
    uint64_t count;
    process_node * firstInLine;
    process_node * lastInLine;
} sem_t;

typedef struct process_node{
    struct process_node * next;
    uint64_t pid;
} process_node;

typedef struct process_list{
    process_node* first;
}process_list;

static void process_init(sem_t * process, char*name, uint64_t value);
static void exchanging(uint64_t * sem, uint64_t val);
static void sem_enqueue(sem_t * sem, uint64_t pid);
static uint64_t sem_dequeue(sem_t * sem);

static process_list* processes;
static uint64_t newSem;
static uint64_t finalSem;

void sem_init(){
    processes = memalloc(sizeof(process_list));
    processes->first = NULL;
}

static void process_init(sem_t * process, char*name, uint64_t value){
    strcpy(process->name, name);
    process->value = value;
    sem_t * nextProcess = processes->first;
    processes->first = process;
    process->next = nextProcess;
    process->lock = UNLOCK;
    process->count = 1;
    process->firstInLine = NULL;
    process->lastInLine = NULL;
}

sem_t * sem_open(char * name, uint64_t value){
    if(name == NULL){
        return ERROR;
    }
    exchanging(&newSem, LOCK);
    sem_t * process = processes->first;
    while(process){
        if(strcmp(process->name, name) == 0){
            process->count++;
            exchange(&newSem, UNLOCK);
            return process;
        }
        process = process->next;
    }
    process = memalloc(sizeof(sem_t));
    if(process == NULL){
        exchange(&newSem, UNLOCK);
        return NULL;
    }
    process_init(process,name,value);
    exchange(&newSem, UNLOCK);
    return process;
}

int sem_wait(sem_t * sem){
    exchanging(&sem->lock, LOCK);
    sem->value--;
    if(sem->value < 0){
        uint64_t pid = getPid();
        sem_enqueue(sem,pid);
        exchange(&sem->lock, UNLOCK);
        block(pid);
    } else{
        exchange(&sem->lock, UNLOCK);
    }

    return 0;
}

int sem_post(sem_t * sem){
    exchanging(&sem->lock, LOCK);
    sem->value++;
    int pid;
    if(sem->firstInLine != NULL){
        pid = sem_dequeue(sem);
        unblock(pid);
    }
    exchange(&sem->lock, UNLOCK);
    return 0;
}

int sem_close(sem_t * sem){
    exchanging(&finalSem, LOCK);
    sem_t * currentProcess = processes->first;
    sem_t * previousProcess = processes->first;
    while(currentProcess){
        if(sem == currentProcess){
            sem->count--;
            if(sem->count == 0){
                if(currentProcess == previousProcess){
                    process->first = currentProcess->next;
                } else {
                    previousProcess->next = currentProcess->next;
                }
                memfree(currentProcess);
            }
            exchange(&finalSem, UNLOCK);
            return 0;
        }
        previousProcess = currentProcess;
        currentProcess = currentProcess->next;
    }
    exchange(&finalSem, UNLOCK);
    return ERROR
}

static void exchanging(uint64_t * sem, uint64_t val){
    uint64_t ex = exchange(sem,val);
    while(ex != 0){
        ex = exchange(sem,val);
    }
}

static void sem_enqueue(sem_t * sem, uint64_t pid){
    process_node * process = memalloc(sizeof(process_node));
    process->pid = pid;
    process->next = NULL;

    if(sem->firstInLine == NULL){
        sem->firstInLine = process;
        sem->lastInLine = process;
    } else {
        sem->lastInLine->next = process;
        sem->lastInLine = process;
    }
}

static uint64_t sem_dequeue(sem_t * sem){
    if(sem == NULL || sem->firstInLine == NULL){
        return ERROR;
    }
    process_node * currentProcess = sem->firstInLine;
    uint64_t pid = currentProcess->pid;
    sem->firstInLine = currentProcess->next;
    if(currentProcess->next == NULL){
        sem->lastInLine = NULL;
    }
    memfree(currentProcess);

    return pid;
}