#include <scheduler.h>
#include <memory.h>
#include <interrupts.h>

#define ERROR -1
#define KILLED 0
#define READY 1
#define BLOCKED 2
#define SIZE_OF_STACK (4 * 1024)

typedef struct pcb{
	uint64_t pid;                   /* ID of the proces */
	uint64_t ppid;
    uint64_t prio;                  /* process priority */
	char name[20];   /* Name of the process */
    uint64_t state;
    uint64_t rsp;
    uint64_t rbp;
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

static process_list* processes;
static process_node* currentProcess;
static process_node* lazyProcess; 
static uint64_t ticksLeft;

void initializeSch(){
    processes= memalloc(sizeof(process_list));
    processes->first=NULL;
    processes->last=NULL;
    processes->size=0;
    processes->pready=0;

    addProcess();

}

void *scheduler(uint64_t rsp){
    if(currentProcess!=NULL){
       if(currentProcess->pcb.state==READY && ticksLeft>0){
           ticksLeft--;
           return rsp;
       }

        currentProcess->pcb.rsp=rsp;

        if(lazyProcess->pcb.pid != currentProcess->pcb.pid){
            if(currentProcess->pcb.state != KILLED){
                enqueue(currentProcess);
            }else{
                process_node* parent=getProcess(currentProcess->pcb.ppid);
                if(parent!=NULL && parent->pcb.state==BLOCKED){
                    unblock(parent);
                }
                remove(currentProcess);
            }            
        }
    }

    if(processes->pready <= 0){
        currentProcess=lazyProcess;
    }else{
        currentProcess=dequeue();
        while(currentProcess->pcb.state != READY){
            if(currentProcess->pcb.state == BLOCKED){
                enqueue(currentProcess);
            }else if(currentProcess->pcb.state == KILLED){
                remove(currentProcess);
            }
            currentProcess=dequeue();
        }
    }
    ticksLeft=currentProcess->pcb-prio;
    return currentProcess->pcb.rsp;
}


void addProcess(){

}


void unblock(process_node* process){
    if(process->pcb.state==KILLED || process == NULL){
        return ERROR;
    }
}

void block(process_node* process){
    if(process->pcb.state==KILLED || process == NULL){
        return ERROR;
    }
}


static process_node* getProcess(uint64_t pid){
    if(currentProcess != NULL && currentProcess->pcb.pid == pid){
        return currentProcess;
    }

    process_node* node=processes->first;

    while(node != NULL){
        if(node->pcb.pid == pid){
            return node;
        }
        node=node->next;
    }

    return NULL;

}

static void enqueue(process_node* process){
    if(!isEmpty()){
        processes->last->next=process;
        process->next=NULL;
        processes->last=process;
    }else{
        processes->first=process;
        processes->last=processes->first;
    }
    if(process->pcb.state == READY){
        processes->pready++;
    }

    processes->size++;
}

static process_node* dequeue(){
    if(isEmpty()){
        return NULL;
    }

    process_node* first=processes->first;
    processes->first=processes->first->next;
    processes->size--;

    if(first->pcb.state == READY){
        processes->pready--;
    }

    return first;
}

static void remove(process_node* process){
    memfree(process->pcb.rbp - SIZE_OF_STACK + 1);
    memfree(process);
}

static int isEmpty(){
    return processes->size==0;
}