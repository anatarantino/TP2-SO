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

typedef struct process_t{
    struct process_t * next;
    uint64_t pid;
} process_t;

typedef struct {
    process_t * first;
    process_t * last;
    uint32_t size;
}semList_t;

typedef struct sem_t{
    char name[MAX_LEN];
    uint64_t value;
    uint8_t lock;
    uint64_t pcount;
    uint8_t isOn;
    semList_t * pBlocked;
} sem_t;

static int sem_create(char * name, uint64_t value);
static void pPrint(process_t * process);
static void process_enqueue(semList_t * pBlocked, process_t * process);
static uint64_t process_dequeue(semList_t * pBlocked);
static int getFreeSem();
static int sem_index(char * name);
static void freeList(semList_t * pblocked);

static sem_t sems[SEM_MAX];
static uint8_t globalLock;

static int sem_create(char * name, uint64_t value){
    if(name == NULL){
        return ERROR;
    }

    int index = getFreeSem();
    if(index == ERROR){ //no tengo lugar para un nuevo semaforo
        return ERROR;
    }

    sem_t * sem = &sems[index]; //lo creo en el index que encontre
    strcopy(sem->name, name);
    sem->value = value;
    sem->lock = UNLOCK;
    sem->pcount = 0;
    sem->isOn = 1;
    sem->pBlocked = memalloc(sizeof(semList_t));
    if(sem->pBlocked == NULL){
        return ERROR;
    }
    sem->pBlocked->first = NULL;
    sem->pBlocked->last = NULL;
    sem->pBlocked->size = 0;
    return index;
}

int sem_open(char * name, uint64_t value){
    if(name == NULL){
        return ERROR;
    }
    int index;

    enter_region(&globalLock);

    index = sem_index(name);

    if(index == -1){ //el semeaforo no existe y lo debo crear
        index = sem_create(name, value); 
        if(index == -1){ //si no puedo crear otro semaforo retorno error
            leave_region(&globalLock);
            return ERROR;
        }   
    }

    sems[index].pcount++;
    leave_region(&globalLock);
    return index;
}

int sem_wait(int index){
    sem_t * sem = &sems[index];

    if(!sem->isOn){
        return -1;
    }

    enter_region(&sem->lock);

    if(sem->value > 0){
        sem->value--;
        leave_region(&sem->lock);
        return 1;
    }
    uint64_t pid = currentProcessPid();
    process_t * p = memalloc(sizeof(process_t));
    if(p == NULL){
        leave_region(&sem->lock);
        return -1;
    }
    p->pid = pid;
    process_enqueue(sem->pBlocked, p);
    leave_region(&sem->lock);
    block(pid);

    return 1;
}

int sem_post(int index){
    if(index > SEM_MAX || index < 0){
        return ERROR;
    }
    sem_t * sem = &sems[index];

    if(!sem->isOn){ //si el semaforo no está en uso es como si no existiese
        return -1;
    }
    enter_region(&sem->lock);
    if(sem->value > 0 || (sem->pBlocked->size == 0) ) {
        sem->value++;
        leave_region(&sem->lock);
        return 1;
    }
    
    uint64_t pid;
    pid = process_dequeue(sem->pBlocked);
    leave_region(&sem->lock);

    unblock(pid);
    
    return 1;
}

int sem_close(int index){
    sem_t * sem = &sems[index];
    if(!sem->isOn){
        return -1;
    }

    enter_region(&sem->lock);

    sem->pcount--;

    if(sem->pcount > 0){
        leave_region(&sem->lock);
        return 1;
    }
    if(sem->pBlocked->size > 0){
        printf("Error in sem_close, processes still blocked");
        printNewLine();
        leave_region(&sem->lock);
        return -1;
    }
    sem->isOn = 0;
    freeList(sem->pBlocked);
    leave_region(&sem->lock);
    return 1;
}

void sem_changeValue(int index, uint64_t value){
    sem_t * sem = &sems[index];
    enter_region(&sem->lock);
    sem->value = value;
    leave_region(&sem->lock);
}

void sems_print(){
    printf("\nNAME   VALUE    BLOCKED PROCESSES PID'S\n");
    for(int i=0; i<SEM_MAX; i++){
        sem_print(i);
    }
}

void sem_print(int index){
    sem_t * sem = &sems[index];
    enter_region(&sem->lock);
    printf(sem->name);
    printf("\t\t\t");
    printInt(sem->value);
    printf("\t\t\t");
    pPrint(sem->pBlocked->first);
    printNewLine();
    leave_region(&sem->lock);
}

static void pPrint(process_t * process){
    while(process){
        printInt(process->pid);
        printf("\t");
        process = process->next;
    }
}

static void process_enqueue(semList_t * pBlocked, process_t * process){
    if(pBlocked->size == 0){
        pBlocked->first = process;
        pBlocked->last = pBlocked->first;
    } else {
        pBlocked->last->next = process;
        process->next = NULL;
        pBlocked->last = process;
    }
    pBlocked->size++;
}

static uint64_t process_dequeue(semList_t * pBlocked){
    if(pBlocked->size == 0){
        return 0;
    }
    process_t * currentProcess = pBlocked->first;
    uint64_t pid = currentProcess->pid;
    pBlocked->first = currentProcess->next;
    pBlocked->size--;
    memfree(currentProcess);
    return pid;
}

static int getFreeSem(){
    for(uint32_t i=0 ; i < SEM_MAX ; i++){
        if(!sems[i].isOn){
            return i;
        }
    }
    return -1;
}

static int sem_index(char * name){
    for(int i = 0; i < SEM_MAX ; i++){
        if(sems[i].isOn){
            if(strcmp(sems[i].name,name) == 0){
                return i;
            }
        }
    }
    return -1;
}

static void freeList(semList_t * pblocked){
    process_t * p = pblocked->first;
    process_t * aux;
    int i = 0;
    while(p != NULL && i < pblocked->size){
        aux = p;
        p = p->next;
        memfree(aux);
        i++;
    }
    memfree(pblocked);
}
