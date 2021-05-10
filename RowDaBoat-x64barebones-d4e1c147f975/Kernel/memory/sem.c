#include <sem.h>
#include <memory.h>
#include <lib.h>
#include <strings.h>
#include <scheduler.h>
#include <prints.h>

#define SEM_MAX 15
#define ERROR -1
#define LOCK 1
#define UNLOCK 0

static void sem_create(sem_t * process, char*name, uint64_t value);
static void sem_print(sem_t * sem);
static void pPrint(process_node * process);
static void exchanging(uint64_t * semval, uint64_t val);
static void process_enqueue(sem_t * sem, uint64_t pid);
static uint64_t process_dequeue(sem_t * sem);

static sem_list* semList;
static uint64_t newSem;
static uint64_t endSem;

int sem_init(){
    semList = memalloc(sizeof(sem_list));
    if(semList == NULL){
        return ERROR;
    }
    semList->first = NULL;
    newSem = 0;
    endSem = 0;
    return 0;
}

static void sem_create(sem_t * sem, char*name, uint64_t value){
    strcopy(sem->name, name);
    sem->value = value;
    sem_t * nextSem = semList->first;
    semList->first = sem;
    sem->next = nextSem;
    sem->lock = UNLOCK;
    sem->pcount = 1;
    sem->firstInLine = NULL;
    sem->lastInLine = NULL;
}

sem_t * sem_open(char * name, uint64_t value){
    if(name == NULL){
        return NULL;
    }
    exchanging(&newSem, LOCK);
    sem_t * sem = semList->first;
    while(sem){
        if(strcmp(sem->name, name) == 0){
            sem->pcount++;
            exchange(&newSem, UNLOCK);
            return sem;
        }
        sem = sem->next;
    }
    sem = memalloc(sizeof(sem_t));
    if(sem == NULL){
        exchange(&newSem, UNLOCK);
        return NULL;
    }
    sem_create(sem,name,value);
    exchange(&newSem, UNLOCK);
    return sem;
}

int sem_wait(sem_t * sem){
    if(sem == NULL){
        return ERROR;
    }
    exchanging(&sem->lock, LOCK);
    sem->value--;
    if(sem->value < 0){
        uint64_t pid = getPid();
        process_enqueue(sem,pid);
        exchange(&sem->lock, UNLOCK);
        block(pid);
    } else{
        exchange(&sem->lock, UNLOCK);
    }

    return 0;
}

int sem_post(sem_t * sem){
    if(sem == NULL){
        return ERROR;
    }
    exchanging(&sem->lock, LOCK);
    sem->value++;
    uint64_t pid;
    if(sem->firstInLine != NULL){
        pid = process_dequeue(sem);
        unblock(pid);
    }
    exchange(&sem->lock, UNLOCK);
    return 0;
}

int sem_close(sem_t * sem){
    if(sem == NULL){
        return ERROR;
    }
    exchanging(&endSem, LOCK);
    sem_t * currentSem = semList->first;
    sem_t * previousSem = semList->first;
    while(currentSem){
        if(sem == currentSem){
            sem->pcount--;
            if(sem->pcount == 0){
                if(currentSem == previousSem){
                    semList->first = currentSem->next;
                } else {
                    previousSem->next = currentSem->next;
                }
                memfree(currentSem);
            }
            exchange(&endSem, UNLOCK);
            return 0;
        }
        previousSem = currentSem;
        currentSem = currentSem->next;
    }
    exchange(&endSem, UNLOCK);
    return ERROR;
}

void sem_changeValue(sem_t * sem, uint64_t value){
    if(sem == NULL){
        return;
    }
    exchanging(&sem->lock, LOCK);
    sem->value = value;
    exchange(&sem->lock, UNLOCK);
}

void sems_print(){
    sem_t * sem = semList->first;
    printf("\nNAME   VALUE    BLOCKED PROCESSES PID'S\n");
    while(sem){
        sem_print(sem);
        sem = sem->next;
    }
}

static void sem_print(sem_t * sem){
    exchanging(&sem->lock, LOCK);
    printf(sem->name);
    printf("\t\t\t");
    printInt(sem->value);
    printf("\t\t\t");
    pPrint(sem->firstInLine);
    exchange(&sem->lock,UNLOCK);
}

static void pPrint(process_node * process){
    while(process){
        printInt(process->pid);
        printf("\t");
        process = process->next;
    }
}

static void exchanging(uint64_t * semval, uint64_t val){
    uint64_t ex = exchange(semval,val);
    while(ex != 0){
        ex = exchange(semval,val);
    }
}

static void process_enqueue(sem_t * sem, uint64_t pid){
    if(sem == NULL){
        return;
    }
    process_node * process = memalloc(sizeof(process_node));
    process->pid = pid;
    process->next = NULL;

    if(sem->firstInLine == NULL){
        sem->firstInLine = process;
        sem->lastInLine = process;
    } else {
        process_node * last = sem->lastInLine;
        last->next = process;
        sem->lastInLine = process;
    }
}

static uint64_t process_dequeue(sem_t * sem){
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
