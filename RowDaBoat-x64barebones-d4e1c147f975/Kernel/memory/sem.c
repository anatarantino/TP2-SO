#include <sem.h>
#include <memory.h>
#include <lib.h>
#include <strings.h>
#include <scheduler.h>
#include <prints.h>

#define SEM_MAX 20
#define ERROR -1
#define LOCK 1
#define UNLOCK 0

static int sem_create(sem_t * sem, char*name, uint64_t value, int index);
static void pPrint(process_node * process);
static void exchanging(uint64_t * semval, uint64_t val);
static void process_enqueue(sem_t * sem, uint64_t pid);
static uint64_t process_dequeue(sem_t * sem);

static sem_t sems[SEM_MAX];
static int dim;
static uint64_t newSem;
static uint64_t endSem;

int sem_init(){
    newSem = 0;
    endSem = 0;
    return 0;
}

static int sem_create(sem_t * sem, char*name, uint64_t value, int index){
    if(index == SEM_MAX){
        return ERROR;
    }
    strcopy(sem->name, name);
    sem->value = value;
    sem->lock = UNLOCK;
    sem->pcount = 1;
    sem->firstInLine = NULL;
    sem->lastInLine = NULL;
    sems[index] = *sem;
    dim++;
    return index;
}

int sem_open(char * name, uint64_t value){
    if(name == NULL){
        return ERROR;
    }
    exchanging(&newSem, LOCK);
    int i;
    sem_t * sem;
    for(i=0; i<dim; i++){
        sem = &sems[i];
        if(strcmp(sem->name, name) == 0){
            sem->pcount++;
            exchange(&newSem, UNLOCK);
            return i;
        }
    }
    sem = memalloc(sizeof(sem_t));
    if(sem == NULL){
        exchange(&newSem, UNLOCK);
        return ERROR;
    }
    i = sem_create(sem,name,value,i);
    exchange(&newSem, UNLOCK);
    return i;
}

int sem_wait(int index){
    if(index > dim || index < 0){
        return ERROR;
    }
    sem_t * sem = &sems[index];
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

int sem_post(int index){
    if(index > dim || index < 0){
        return ERROR;
    }
    sem_t * sem = &sems[index];
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

int sem_close(int index){
    if(index > dim || index < 0){
        return ERROR;
    }
    sem_t * sem = &sems[index];
    exchanging(&endSem, LOCK);
    sem->pcount--;
    if(sem->pcount == 0){
        memfree(sem);
        for(int i=index; i<dim-1; i++){
            sems[i]=sems[i+1];
        }
        dim--;
    }
    exchange(&endSem, UNLOCK);
    return 0;
}

void sem_changeValue(int index, uint64_t value){
    if(index > dim || index < 0){
        return ERROR;
    }
    sem_t * sem = &sems[index];
    exchanging(&sem->lock, LOCK);
    sem->value = value;
    exchange(&sem->lock, UNLOCK);
}

void sems_print(){
    printf("\nNAME   VALUE    BLOCKED PROCESSES PID'S\n");
    sem_t * sem;
    for(int i=0; i<dim; i++){
        sem = &sems[i];
        sem_print(sem);
    }
}

void sem_print(sem_t * sem){
    exchanging(&sem->lock, LOCK);
    printf(sem->name);
    printf("\t\t\t");
    printInt(sem->value);
    printf("\t\t\t");
    pPrint(sem->firstInLine);
    printNewLine();
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
