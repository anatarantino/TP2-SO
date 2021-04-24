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
static void unblock(process_node* process);
static void block(process_node* process);

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
       if(currentProcess->control_block.state==READY && ticksLeft>0){
           ticksLeft--;
           return rsp;
       }

        currentProcess->control_block.rsp=rsp;

        if(lazyProcess->control_block.pid != currentProcess->control_block.pid){
            if(currentProcess->control_block.state != KILLED){
                enqueue(currentProcess);
            }else{
                process_node* parent=getProcess(currentProcess->control_block.ppid);
                if(parent!=NULL && parent->control_block.state==BLOCKED){
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
        while(currentProcess->control_block.state != READY){
            if(currentProcess->control_block.state == BLOCKED){
                enqueue(currentProcess);
            }else if(currentProcess->control_block.state == KILLED){
                remove(currentProcess);
            }
            currentProcess=dequeue();
        }
    }
    ticksLeft=currentProcess->control_block.prio;
    return currentProcess->control_block.rsp;
}


void addProcess(){

}


static void unblock(process_node* process){
    if(process->control_block.state==KILLED || process == NULL){
        return ERROR;
    }
}

static void block(process_node* process){
    if(process->control_block.state==KILLED || process == NULL){
        return ERROR;
    }
}


static process_node* getProcess(uint64_t pid){
    if(currentProcess != NULL && currentProcess->control_block.pid == pid){
        return currentProcess;
    }

    process_node* node=processes->first;

    while(node != NULL){
        if(node->control_block.pid == pid){
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
    if(process->control_block.state == READY){
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

    if(first->control_block.state == READY){
        processes->pready--;
    }

    return first;
}

static void remove(process_node* process){
    memfree(process->control_block.rbp - SIZE_OF_STACK + 1);
    memfree(process);
}

static int isEmpty(){
    return processes->size==0;
}