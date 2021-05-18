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
    uint64_t lock;
    uint64_t pcount;
    uint8_t isOn;
    semList_t * pBlocked;
} sem_t;

static int sem_create(char * name, uint64_t value);
static void pPrint(process_t * process);
static void exchanging(uint64_t * semval, uint64_t val);
static void process_enqueue(semList_t * pBlocked, uint64_t pid);
static uint64_t process_dequeue(semList_t * pBlocked);
static int getFreeSem();

static sem_t sems[SEM_MAX];
static int dim;
static uint64_t newSem;
static uint64_t endSem;

int sem_init(){
    newSem = 0;
    endSem = 0;
    return 0;
}

static int sem_create(char * name, uint64_t value){
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
    int i;
    exchanging(&newSem, LOCK);
    sem_t * sem;
    for(i=0; i<dim; i++){
        sem = &sems[i];
        if(strcmp(sem->name, name) == 0){
            sem->pcount++;
            exchange(&newSem, UNLOCK);
            return i;
        }
    }

    i = sem_create(name, value); //si no existe el semaforo que estoy buscando lo creo

    if(i == -1){ //si no puedo crear otro semaforo retorno error
        exchange(&newSem,UNLOCK);
        return ERROR;
    }
    printNewLine();
    sems[i].pcount++;
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
        process_enqueue(sem->pBlocked,pid);
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

    if(!sem->isOn){ //si el semaforo no está en uso es como si no existiese
        return -1;
    }
    exchanging(&sem->lock, LOCK);

    if(sem->value > 0 || (sem->pBlocked->size == 0) ) {
        sem->value++;
        exchange(&sem->lock, UNLOCK);
        return 1;
    }
    

    uint64_t pid;
    pid = process_dequeue(sem->pBlocked);
    unblock(pid);
    
    exchange(&sem->lock, UNLOCK); //fijarse si no hay que cambiar el orden de esta linea con la 136 tengo que desbloquear la region para desbloquear el proceso??
    return 1;
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
        return;
    }
    sem_t * sem = &sems[index];
    exchanging(&sem->lock, LOCK);
    sem->value = value;
    exchange(&sem->lock, UNLOCK);
}

void sems_print(){
    printf("\nNAME   VALUE    BLOCKED PROCESSES PID'S\n");
    for(int i=0; i<dim; i++){
        sem_print(i);
    }
}

void sem_print(int index){
    sem_t * sem = &sems[index];
    exchanging(&sem->lock, LOCK);
    printf(sem->name);
    printf("\t\t\t");
    printInt(sem->value);
    printf("\t\t\t");
    pPrint(sem->pBlocked->first);
    printNewLine();
    exchange(&sem->lock,UNLOCK);
}

static void pPrint(process_t * process){
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

static void process_enqueue(semList_t * pBlocked, uint64_t pid){
    if(pBlocked == NULL){
        return;
    }
    process_t * process = memalloc(sizeof(process_t));
    process->pid = pid;
    process->next = NULL;

    if(pBlocked->first == NULL){
        pBlocked->first = process;
        pBlocked->last = pBlocked->first;
    } else {
        process_t * last = pBlocked->last;
        last->next = NULL;
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