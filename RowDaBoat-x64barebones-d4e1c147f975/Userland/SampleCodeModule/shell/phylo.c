// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <phylo.h>
#include <shell.h>
#include <strings.h>
#include <prints.h>
#include <colors.h>
#include <semLib.h>
#include <processLib.h>
#include <timeRTC.h>

#define MAX_PHYLO 5
#define MIN_PHYLO 2
#define NAME_LEN 10
#define EATING 1
#define WAITING 2
#define PHYLO_NAME "_phy"

static void analizeChar(char c);
static int initializePhylo(int phyloCount);
static int leftPhylo(int args, char *arguments[]);
static int rightPhylo(int args, char *arguments[]);
static void updateTable(uint16_t phylo_id, int state);
static void addPhylo();
static void removePhylo();
static void freeResources();
static void freeRightFork(uint16_t phylo);

static void updateTable(uint16_t phylo_id, int state);

typedef struct phylo{
    uint64_t state;
    uint64_t pid;
    uint8_t run;
    uint8_t right_fork;
}t_phylo;

static int phy_count;
static uint64_t forks[MAX_PHYLO];
static char table[MAX_PHYLO + 1];
static t_phylo phylos[MAX_PHYLO];


void phyloFunc(int count){
    phy_count=count;
    if(phy_count > MAX_PHYLO){
        phy_count=MAX_PHYLO;
    }else if(phy_count < MIN_PHYLO){
        phy_count=MIN_PHYLO;
    }
    if(initializePhylo(phy_count)==ERROR){
        printColor("Error initializing phylo",RED,BLACK);
        return;
    }
    int c=0;
    analizeChar(c);
    freeResources();

}

static void analizeChar(char c){
    while ((c = getChar()) != -1) {
        if((char)c == 'a' || (char)c == 'A') {
            addPhylo();
        }
        else if((char)c == 'r' || (char)c == 'R'){
            removePhylo();
        }
    }
}

static int initializePhylo(int phyloCount){
    char phy_name[NAME_LEN];
    char phy_id[NAME_LEN];

    for(int i=0;i<phy_count;i++){
        uintToBase(i,phy_name, 10);
        strcat(phy_name, PHYLO_NAME);
        forks[i]=sem_open(phy_name,1);
        table[i]='.';
    }
    for(int i=0;i<phy_count;i++){
        uintToBase(i, phy_name, 10);
        strcat(phy_name, PHYLO_NAME);
        uintToBase(i, phy_id, 10);
        phylos[i].state=WAITING;
        phylos[i].run=1;
        phylos[i].right_fork=0;
        char* arguments[]={phy_name,phy_id};
        printf("Added to table phylosopher ");
        printInt(i+1);
        newln();
        phylos[i].pid=addProcess((i % 2 == 0)? rightPhylo : leftPhylo, 2, arguments, 0, 0);
    }
    return 1;
}

static int leftPhylo(int args, char *arguments[]){
    uint16_t phy_id=atoi(arguments[1]);
    if(phy_id % 2 == 0){
        return 1;
    }
    while(phylos[phy_id].run){
        sleep(2);

        sem_wait(forks[phy_id]);

        phylos[phy_id].right_fork = 1;
        sem_wait(forks[(phy_id + 1) % phy_count]);

        updateTable(phy_id, EATING);
        sleep(15);

        sem_post(forks[(phy_id + 1) % phy_count]);
        phylos[phy_id].right_fork = 0;

        sem_post(forks[phy_id]);

        table[phy_id] = '.';
    }
    return 0;
}

static int rightPhylo(int args, char *arguments[]){
    uint16_t phy_id=atoi(arguments[1]);
    if(phy_id % 2 != 0){
        return 1;
    }
    while(phylos[phy_id].run){
            sleep(2);
            phylos[phy_id].right_fork = 1;
            sem_wait(forks[(phy_id + 1) % phy_count]);

            sem_wait(forks[phy_id]);
            updateTable(phy_id, EATING);
            sleep(15);

            sem_post(forks[phy_id]);

            sem_post(forks[(phy_id + 1) % phy_count]);
            phylos[phy_id].right_fork = 0;

            table[phy_id] = '.';
    }
    return 0;
}

static void addPhylo(){
    if(phy_count>=MAX_PHYLO){
        return;
    }
    char phy_name[NAME_LEN];
    char phy_id[NAME_LEN];
    uint16_t phylo=phy_count;

    uintToBase(phylo,phy_name,10);
    uintToBase(phylo,phy_id,10);
    strcat(phy_name,PHYLO_NAME);

    phylos[phylo].state=WAITING;
    table[phylo]='.';
    phylos[phylo].run=1;
    phylos[phylo].right_fork=0;

    freeRightFork(phylo-1);

    forks[phylo]=sem_open(phy_name,1);
    phy_count++;

    unblockProcess(phylos[phylo-1].pid);

    printf("Added to table phylosopher ");
    printInt(phylo+1);
    newln();
    char* arguments[]={phy_name,phy_id};
    phylos[phylo].pid=addProcess((phylo % 2 == 0)? rightPhylo : leftPhylo, 2, arguments, 0, 0);

}

static void removePhylo(){
    if(phy_count<=MIN_PHYLO){
        return;
    }
    uint16_t phylo=phy_count-1;
    uint16_t phylo_left=phylo-1;

    phylos[phylo].run=0;
    freeRightFork(phylo_left);
    wait(phylos[phylo].pid);

    table[phylo]=0;

    phy_count--;

    unblockProcess(phylos[phylo_left].pid);

    sem_close(forks[phylo]);

    printf("Leaving table phylosopher ");
    printInt(phylo+1);
    newln();
}

static void freeResources(){
    for(uint16_t i = 0; i < phy_count; i++){
        phylos[i].run = 0;
    }

    for(uint16_t i = 0; i < phy_count; i++){
        wait(phylos[i].pid);
    }

    for(uint16_t i = 0; i < phy_count; i++){
        printf("Phylosopher ");
        printInt(i+1);
        printf(" left.\n");
        sem_close(forks[i]);
    }
    
    phy_count = 0;

    for(uint16_t i = 0; table[i] != 0; i++){
        table[i] = 0;
    }
}

static void freeRightFork(uint16_t phylo){
    while(1){
        if(blockProcess(phylos[phylo].pid)==1){
            yield();
        }else if(phylos[phylo].right_fork){
            unblockProcess(phylos[phylo].pid);
            yield();
        }else{
            return;
        }
    }
}

static void updateTable(uint16_t phylo_id, int state){
    if(state==EATING){
        table[phylo_id]='E';
    }else{
        table[phylo_id]='.';
    }
    printf(table);
    newln();
}