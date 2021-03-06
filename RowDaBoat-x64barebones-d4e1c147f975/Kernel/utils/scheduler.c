// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <scheduler.h>
#include <memory.h>
#include <interrupts.h>
#include <prints.h>
#include <strings.h>
#include <lib.h>

#define ERROR -1
#define KILLED 0
#define READY 1
#define BLOCKED 2
#define SIZE_OF_STACK (4 * 1024)
#define BGPRIO 1
#define FGPRIO 2
#define MAXPRIO 30
#define MAX_LEN 20

typedef struct pcb{
	uint64_t pid;                   /* ID of the proces */
	uint64_t ppid;
    uint64_t prio;                  /* process priority */
	char name[MAX_LEN];                  /* Name of the process */
    uint64_t state;
    uint64_t rsp;
    uint64_t rbp;
    uint64_t foreground;
    uint64_t input;
    uint64_t output;
}pcb;

typedef struct process_node {
      pcb control_block;
      struct process_node* next;
}process_node;

typedef struct process_list{
    uint32_t size;
    uint32_t pready;
    process_node* first;
    process_node* last;
}process_list;

static void enqueue(process_node* process);
static process_node* dequeue();
static void remove(process_node* process);
static int isEmpty();
static process_node* getProcess(uint64_t pid);
static int initializeProcess(pcb * process, char* argv, uint8_t fg, uint16_t * fd);
static void initializeStackFrame(uint64_t rbp,int (*process)(int, char **),int argc, char** argv);
static uint64_t newPid();
static void printProcess(pcb block);
static int changeState(uint64_t pid, int state);
static void wrapper(void (*entryPoint)(int, char**), int argc, char** argv);
static int shellBackup(int argc, char** argv);

static process_list* processes;
static process_node* currentProcess;
static uint64_t ticksLeft;
static uint64_t last_pid = 1;
static process_node* backupProcess;

void initializeSch(){
    processes = memalloc(sizeof(process_list));
    processes->first=NULLP;
    processes->last=NULLP;
    processes->size=0;
    processes->pready=0;

    char* argv[] = {"Shell Backup Process"};
    addProcess(&shellBackup, 1, argv, 1, 0);
    backupProcess = dequeue();
}

uint64_t scheduler(uint64_t rsp){
    if(currentProcess!=NULLP){
       if(currentProcess->control_block.state==READY && ticksLeft>0){
           ticksLeft--;
           return rsp;
       }
        currentProcess->control_block.rsp=rsp;

        if(currentProcess->control_block.pid != backupProcess->control_block.pid){
            if(currentProcess->control_block.state != KILLED){
                enqueue(currentProcess);
            }else{
                process_node* parent=getProcess(currentProcess->control_block.ppid);
                if(parent!=NULLP && parent->control_block.state==BLOCKED && currentProcess->control_block.foreground){
                    unblock(parent->control_block.pid);
                }
                remove(currentProcess);
            }
        }
        
    }
    if(processes->pready > 0){
        currentProcess=dequeue();
        while(currentProcess->control_block.state != READY){
            if(currentProcess->control_block.state == BLOCKED){
                enqueue(currentProcess);
            }else if(currentProcess->control_block.state == KILLED){
                remove(currentProcess);
            }
            currentProcess=dequeue();
        }
    }
    else{
        currentProcess = backupProcess;
    }
    
    ticksLeft=currentProcess->control_block.prio;
    return currentProcess->control_block.rsp;
}

uint64_t addProcess(int (*process)(int,char**),int argc, char** argv, uint8_t fg, uint16_t * fd){
    if(process == NULLP){
        return 0;
    }
    process_node * newProcess = memalloc(sizeof(process_node));

    if(newProcess == NULLP){
        return 0;
    }
    if(initializeProcess(&newProcess->control_block,argv[0],fg,fd) == 1){
        return 0;
    }

    char ** copy = memalloc(sizeof(char*)*argc);
    int size;

    for(int i=0; i<argc; i++){
        size = strleng(argv[i]);
        copy[i] = memalloc(sizeof(char) * size +1);
        strcopy(copy[i],argv[i]);
    }

    initializeStackFrame(newProcess->control_block.rbp,process,argc,copy);

    enqueue(newProcess);
    if(newProcess->control_block.foreground && newProcess->control_block.ppid){
        block(newProcess->control_block.ppid);
    }

    return newProcess->control_block.pid;
}

static int initializeProcess(pcb * process, char* argv, uint8_t fg, uint16_t * fd){
    strcopy(process->name,argv);
    process->pid = newPid();

    if(currentProcess == NULLP){
        process->ppid = 0;
    }else{
        process->ppid = currentProcess->control_block.pid;
    }

    if(fg > 1){
        return 1;
    }
    if(currentProcess == NULLP){
        process->foreground = fg;
    }else{
        if(currentProcess->control_block.foreground){
            process->foreground = fg;
        }
        else{
            process->foreground = 0;
        }
    }

    process->rbp = (uint64_t) memalloc(SIZE_OF_STACK);
    if(process->rbp == NULLP){
        return 1;
    }
    if(process->foreground){
        process->prio = FGPRIO;
    }
    else{
        process->prio = BGPRIO;
    }
    process->rbp = process->rbp + SIZE_OF_STACK - 1;
    process->rsp = (uint64_t)((stackFrame *)process->rbp - 1);
    process->state = READY;
    if(fd){
        process->input = fd[0];
        process->output = fd[1];
    }else{
        process->input = 0;
        process->output = 0;
    }

    return 0;
}

static void initializeStackFrame(uint64_t rbp,int (*process)(int, char **),int argc, char** argv){
    stackFrame * sframe = (stackFrame *)rbp-1;

    sframe->gs = 0x01;
    sframe->fs = 0x02;
    sframe->r15 = 0x03;
    sframe->r14 = 0x04;
    sframe->r13 = 0x05;
    sframe->r12 = 0x06;
    sframe->r11 = 0x07;
    sframe->r10 = 0x08;
    sframe->r9 = 0x09;
    sframe->r8 = 0x0A;
    sframe->rsi = (uint64_t)argc;
    sframe->rdi = (uint64_t)process;
    sframe->rbp = 0x0D;
    sframe->rdx = (uint64_t)argv;
    sframe->rcx = 0x0F;
    sframe->rbx = 0x10;
    sframe->rax = 0x11;
    sframe->rip = (uint64_t)wrapper;
    sframe->cs = 0x8;
    sframe->rflags = 0x202;
    sframe->rsp = (uint64_t)&sframe->bp;
    sframe->ss = 0x0;
    sframe->bp = 0x0;
}

static uint64_t newPid(){
    return last_pid++;
}

static process_node* getProcess(uint64_t pid){
    if(currentProcess != NULLP && currentProcess->control_block.pid == pid){
        return currentProcess;
    }

    process_node* node=processes->first;

    while(node != NULLP){
        if(node->control_block.pid == pid){
            return node;
        }
        node=node->next;
    }

    return NULLP;

}

void ps(){
    printf("\nPID   PRIORITY    STACK POINTER   BASE POINTER    FOREGROUND     STATE     NAME\n");

    if(currentProcess != NULLP){
        printProcess(currentProcess->control_block);
    }

    for (process_node * node = processes->first; node != NULLP; node = node->next) {
        printProcess(node->control_block);
    }


}

uint64_t kill(uint64_t pid){
    int aux = changeState(pid, KILLED);

    if(pid == currentProcess->control_block.pid){
        callTimerTick();
    }

    return aux;
}

void nice(uint64_t pid, uint64_t newPrio){
    process_node* node = getProcess(pid);
    if (newPrio <= MAXPRIO && node != NULLP) {
        node->control_block.prio = newPrio;
    }
}

uint64_t getPid(){
    if(currentProcess){
        return currentProcess->control_block.pid;
    }

    return 0;
}

static void printProcess(pcb block){
    printInt(block.pid);
    printf("\t\t\t");
    printInt(block.prio);
    printf("\t\t\t");
    printInt(block.rsp);
    printf("\t\t");
    printInt(block.rbp);
    printf("\t\t\t");
    printInt(block.foreground);
    printf("\t\t\t  ");
    if(block.state == KILLED){
        printf("KILLED ");
    }else if(block.state == BLOCKED){
        printf("BLOCKED");
    }else{
        printf("READY  ");
    }
    printf("\t");
    printf(block.name);
    putChar('\n');
}

static int changeState(uint64_t pid, int state){
    process_node * process = getProcess(pid);
    if(process == NULLP || process->control_block.state == KILLED){
        return -1;
    }

    if(process == currentProcess){
        if(currentProcess->control_block.state == state){
            return 1;
        }
        currentProcess->control_block.state = state;
        return 0;
    }

    if(process->control_block.state == state){
        return 1;
    }

    if(process->control_block.state != READY && state == READY){
        processes->pready++;
    }else if(process->control_block.state == READY && state != READY){
        processes->pready--;
    }
    process->control_block.state = state;
    return 0;
}

uint64_t unblock(uint64_t pid){
    return changeState(pid,READY);
}

uint64_t block(uint64_t pid){
    int aux = changeState(pid,BLOCKED);
    if(pid == currentProcess->control_block.pid){
        callTimerTick();
    }
    return aux;
}

static void enqueue(process_node* process){
    if(!isEmpty()){
        processes->last->next=process;
        process->next=NULLP;
        processes->last=process;
    }else{
        processes->first=process;
        processes->last=processes->first;
    }
    if(process->control_block.state == READY){
        processes->pready++;
    }

    processes->size++;
}

static process_node* dequeue(){
    if(isEmpty()){
        return NULLP;
    }

    process_node* first=processes->first;
    processes->first=processes->first->next;
    processes->size--;

    if(first->control_block.state == READY){
        processes->pready--;
    }

    return first;
}

static void remove(process_node* process){
    memfree((char *)process->control_block.rbp - SIZE_OF_STACK + 1);
    memfree(process);
}

static int isEmpty(){
    return processes->size==0;
}

void killLoop(){
    if (currentProcess != NULLP && currentProcess->control_block.foreground && currentProcess->control_block.state == READY){
        kill(currentProcess->control_block.pid);
        return;
    }
}

static void wrapper(void (*process)(int, char**), int argc, char** argv){
    process(argc, argv);
    for(int i = 0 ; i < argc ; i++){
        memfree(argv[i]);
    }
    memfree(argv);
    exit();
}

void exit(){
    kill(currentProcess->control_block.pid);
    callTimerTick();
}

void yield(){
    ticksLeft = 0;
    callTimerTick();
}

void wait(uint64_t pid){
      process_node * p=getProcess(pid);
      if (p != NULLP) {
            p->control_block.foreground=1;
            block(currentProcess->control_block.pid);
      }
}

uint64_t getInput(){
    return currentProcess->control_block.input;
}

uint64_t getOutput(){
    return currentProcess->control_block.output;
}

uint64_t getFg(){
    return currentProcess->control_block.foreground;
}

static int shellBackup(int argc, char** argv) {
      while (1){
            _hlt();
      }
      return 0;
}