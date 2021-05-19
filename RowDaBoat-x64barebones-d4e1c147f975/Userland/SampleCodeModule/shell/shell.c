#include <syscall.h>
#include <prints.h>
#include <strings.h>
#include <shell.h>
#include <timeRTC.h>
#include <graphics.h>
#include <chess.h>
#include <colors.h>
#include <lib.h>
#include <processLib.h>
#include <ipcLib.h>
#include <memoryLib.h>
#include <semLib.h>
#include <phylo.h>
#include <tests.h>

#define TOTAL_SIZE 150
#define TOTAL_COMMANDS 24
#define TOTAL_REG 17
#define TOTAL_ARGUMENTS 5
#define STLINE 43

enum comm_num{INFOREG=0,PRINTMEM,TIME,CHESS,HELP,CLEARSC,DIVZERO,OPCODE,PSS,LOOPS,KILLS,NICES,BLOCKS,UNBLOCKS, MEMS, SEMS, CATS, WCS, FILTERS, PIPES, PHYLOS};

//static char * commands[] = {"inforeg","printmem","time","chess","help","clear","divByZeroException","opCodeException","ps","loop","kill","nice","block","unblock", "mem", "sem", "cat", "wc", "filer", "pipe", "phylo"};
static char * user = "grupo6@user:~$ ";

static char * registers[] = {"R15: ", "R14: ", "R13: ", "R12: ", "R11: ", "R10: ", "R9:  ",
                           "R8:  ", "RSI: ", "RDI: ", "RBP: ", "RDX: ", "RCX: ", "RBX: ","RAX: ",
                            "RIP: ", "RSP: "};


static char buff[TOTAL_SIZE]={0};
static int index=0,flag=1;

//static functions
static void printMessage();
static void analizeChar(char c);
static void processCommand();
//static void applyCommand(int command_num,char*arguments[],int totArgs);
static void removeChar();
static void inforeg(int args, char *arguments[]);
static void printmem(int args, char *arguments[]);
static void time(int args, char *arguments[]);
static void printTime(time_type desc);
static void chess(int args, char *arguments[]);
static void help(int args, char *arguments[]);
static void clear(int args, char *arguments[]);
static void divisionByZero(int args,char *arguments[]);
static void opCodeException(int args,char *arguments[]);
static void cleanBuffer();
static void ps(int args, char *arguments[]);
static void loop(int args, char *arguments[]);
static void kill(int args, char *arguments[]);
static void nice(int args, char *arguments[]);
static void block(int args, char *arguments[]);
static void unblock(int args, char *arguments[]);
static void printu();
static void mem(int args, char *arguments[]);
static void sem(int args, char *arguments[]);
static void cat(int args, char *arguments[]);
static void wc(int args, char *arguments[]);
static void filter(int args, char *arguments[]);
static void pipe(int args, char *arguments[]);
static void phylo(int args, char *arguments[]);
static void testMM(int args, char *arguments[]);
static void testSync(int args, char *arguments[]);
static void testNoSync(int args, char *arguments[]);
static void testProcesses(int args, char *arguments[]);
static void testPriority(int args, char *arguments[]);

static t_command functions[] = {
    {&help,"help",""},
    {&inforeg,"inforeg","inforeg -> prints registers values, press ctrl + r to update values.\n"},
    {&printmem,"printmem","printmem -> receives a pointer and makes a memory dump of 32 bytes on screen starting on the direction introduced.\n"},
    {&time,"time","time -> prints system time on screen.\n"},
    {&chess,"chess","chess -> this command starts a chess game.\n"},
    {&clear,"clear","clear -> clearclears the screen.\n"},
    {&divisionByZero,"divByZeroException","divByZeroException -> tests the division by zero exception.\n"},
    {&opCodeException,"opCodeException","opCodeException -> tests the exception caused by an invalid operation code.\n"},
    {&ps,"ps","ps -> prints all the processes with their properties.\n"},
    {&loop,"loop","loop -> prints the PID in a certain amount of seconds. To terminate the loop please press ctrl+c\n"},
    {&kill,"kill","kill -> kills a process given it's ID.\n"},
    {&nice,"nice","nice -> change a process priority with the given ID.\n"},
    {&block,"block","block -> blocks a process with the given ID.\n"},
    {&unblock,"unblock","unblock -> unblocks a process with the given ID.\n"},
    {&mem, "mem","mem -> prints memory's status.\n"},
    {&sem, "sem","sem -> prints semaphores information.\n"},
    {&cat, "cat","cat -> prints stdin.\n"},
    {&wc, "wc","wc -> counts the number of lines in the input.\n"},
    {&filter, "filter","filter -> filter the vowels of the input.\n"},
    {&pipe, "pipe","pipe -> prints all the pipes with their properties.\n"},
    {&phylo, "phylo","phylo -> dining philosophers problem, receives a starting number of phylosophers (maximum: 10, minimum: 2).\n"},
    {&testMM, "testMM","testMM -> tests memory manager.\n"},
    {&testSync, "testSync","testSync -> tests semaphore's sync.\n"},
    {&testNoSync, "testNoSync","testNoSync -> tests semaphore's sync.\n"},
    {&testProcesses, "testProcesses","testProcesses -> tests process creation.\n"},
    {&testPriority, "testPriority","testPriority -> tests scheduler priority."},
};

void startShell(int argc, char *argv[]){
    char c=0;
    cleanBuffer();
    if(flag){
        for(int i; i<STLINE; i++){
            newln();
        }
        printMessage();
    }
    printf(user);
    while(1){
        c=getChar();
        analizeChar(c);
    }

}

static void analizeChar(char c){
    switch (c)
    {
    case '\n':
        processCommand();
        cleanBuffer();
        break;
    case '\b':
        removeChar();
        break;
    default:
        buff[index++]=c;
        putChar(c);
        break;
    }
}

static int getCommand(char * name){
    for(int i = 0 ; i< TOTAL_COMMANDS ; i++){
        if(stringcmp(name,functions[i].name) == 1){
            return i;
        }
    }
    return -1;
}

static void processCommand(){

    char * argv[TOTAL_ARGUMENTS] = {0};

    int totArgs = strtok(buff,' ', argv, TOTAL_ARGUMENTS);

    //int flag=0;
    //int comm = -1;
    uint8_t fg = 1;

    if(argv[totArgs - 1][0] == '&') {
        fg = 0;
        totArgs--;
    }

    int command_index = getCommand(argv[0]);
    if(command_index == -1){
        newln();
        printColor("Invalid command",RED,BLACK);
        printu();
        return;
    }
    flag = 1;
    addProcess((int (*)(int,char**))functions[command_index].command,totArgs,argv,fg, 0);



    // if(comm != CLEARSC && comm !=CHESS){
    //     printf("entre :O");
    //     newln();
    // }

    // for (int i = 0; i < TOTAL_COMMANDS; i++){
    //     result=stringcmp(argv[0],commands[i]);
    //     if(result==1){
    //         addProcess(functions[i].command,totArgs,argv,fg);
    //         applyCommand(i,argv+1,totArgs);
    //         flag=1;
    //         comm = i;
    //     }
    // }

}
/*
static void applyCommand(int command_num,char *arguments[],int totArgs){
    switch (command_num)
    {
    case INFOREG:
        inforeg(totArgs,arguments);
        break;
    case PRINTMEM:
        printmem(totArgs,arguments);
        break;
    case TIME:
        time(totArgs,arguments);
        break;
    case HELP:
        help(totArgs,arguments);
        break;
    case CLEARSC:
        clear(totArgs,arguments);
        break;
    case DIVZERO:
        divisionByZero(totArgs,arguments);
        break;
    case OPCODE:
        opCodeException(totArgs,arguments);
        break;
    case CHESS:
        chess(totArgs,arguments);
        break;
    case PSS:
        ps(totArgs,arguments);
        break;
    case LOOPS:
        loop(totArgs,arguments);
        break;
    case KILLS:
        kill(totArgs,arguments);
        break;
    case NICES:
        nice(totArgs,arguments);
        break;
    case BLOCKS:
        block(totArgs,arguments);
        break;
    case UNBLOCKS:
        unblock(totArgs,arguments);
        break;
    case MEMS:
        mem(totArgs,arguments);
        break;
    case SEMS:
        sem(totArgs,arguments);
        break;
    case CATS:
        cat(totArgs,arguments);
        break;
    case WCS:
        wc(totArgs,arguments);
        break;
    case FILTERS:
        filter(totArgs,arguments);
        break;
    case PIPES:
        pipe(totArgs,arguments);
        break;
    case PHYLOS:
        phylo(totArgs,arguments);
        break;
    }
}*/

static void removeChar(){
    if(index>0){
        buff[index]=0;
        index--;
        putChar('\b');
    }
}

static void inforeg(int args, char *arguments[]){
    if(args!=1){
        invalidAmount();
        return;
    }
    newln();
    uint64_t* regs=(uint64_t *)getRegisters();
    for(int i=0; i< TOTAL_REG; i++){
        printColor(registers[i],VIOLET,BLACK);
        printHex(regs[i]);
        newln();
    }
    printu();
}

static void printmem(int args, char *arguments[]){
   uint64_t num=hexastrToInt(arguments[1]);
    char buffer[50];
    if(args!=2){
        invalidAmount();
        return;
    }
    if(num==0){
        newln();
        printColor("Invalid argument", RED, BLACK);
        return;
    }
    newln();
    uint8_t *rsp=(uint8_t *) num;
      for (int i = 0; i < 4; i++){
        for (int j = 0; j < 8; j++){
            uintToBase(rsp[8*i + j], buffer, 16);
            printf(buffer);
            putChar(' ');
        }
        newln();
    }
    printf(user);
}

static void time(int args, char *arguments[]){
    if(args!=1){
        invalidAmount();
        return;
    }
    newln();
    printColor("Date time:",YELLOW,BLACK);
    newln();
    printTime(DAYOFWEEK);
    newln();
    printTime(DAY);
    putChar('/');
    printTime(MONTH);
    putChar('/');
    printTime(YEAR);
    newln();
    printTime(HOURS);
    putChar(':');
    printTime(MINUTES);
    putChar(':');
    printTime(SECONDS);
    printu();
}

static void printTime(time_type desc){
    char buff_aux[3]={0};
    int aux;
    switch(desc){
        case YEAR:
            uintToBase(getTime(YEAR),buff_aux,10);
            printf(buff_aux);
            break;
        case MONTH:
            uintToBase(getTime(MONTH),buff_aux,10);
            printf(buff_aux);
            break;
        case DAY:
            uintToBase(getTime(DAY),buff_aux,10);
            printf(buff_aux);
            break;
        case DAYOFWEEK:
            switch(getTime(DAYOFWEEK)){
                case 1:
                    printf("Sunday");
                    break;
                case 2:
                    printf("Monday");
                    break;
                case 3:
                    printf("Tuesday");
                    break;
                case 4:
                    printf("Wednesday");
                    break;
                case 5:
                    printf("Thursday");
                    break;
                case 6:
                    printf("Friday");
                    break;
                case 7:
                    printf("Saturday");
                    break;
            }
            break;
        case HOURS:
            aux = getTime(HOURS);
            if(aux < 3)
                aux = 24 - 3 + aux;
            else
                aux -= 3;
            uintToBase(aux,buff_aux,10);
            printf(buff_aux);
            break;
        case MINUTES:
            uintToBase(getTime(MINUTES),buff_aux,10);
            printf(buff_aux);
            break;
        case SECONDS:
            uintToBase(getTime(SECONDS),buff_aux,10);
            printf(buff_aux);
            break;
        default:
            return;
    }
}

static void help(int args, char *arguments[]){
    if(args!=1){
        invalidAmount();
        return;
    }
    newln();
    for (int i = 0; i < TOTAL_COMMANDS; i++) {
        printf(functions[i].description);
    }
    printu();
}

static void clear(int args, char *arguments[]){
    if(args!=1){
        invalidAmount();
        newln();
        return;
    }
    clearScreen();
    printf(user);
}

static void printMessage(){
    printColor("Welcome to the shell!\n",GREEN,BLACK);
    printColor("Please enter one of the following commands:\n",GREEN,BLACK);
    printColor("- inforeg - printmem - time - chess - clear - divByZeroException - opCodeException - ps - loop - kill - nice - block - unblock - mem - sem - cat - wc - filter - pipe - phylo\n",GREEN,BLACK);
    printColor("Or press 'help' to see more information on the commands of this shell.\n",GREEN,BLACK);
}

static void divisionByZero(int args,char *arguments[]){
    if(args!=1){
        invalidAmount();
        return;
    }
    flag=0;
    int a = 0;
    int b=10/a;
    a=b;
}

static void opCodeException(int args,char *arguments[]){
    if(args!=1){
        invalidAmount();
        return;
    }
    flag=0;
    __asm__("ud2");
}

static void chess(int args, char *arguments[]){
    if(args!=1){
        invalidAmount();
        return;
    }
    clearScreen();

    playChess();
    printf(user);
}

static void cleanBuffer(){
    for (int i = 0; i < TOTAL_SIZE; i++) {
        buff[i] = 0;
    }
    buff[index] = 0;
	index=0;
}

void invalidAmount(){
    newln();
    printColor("Invalid ammount of arguments", RED, BLACK);
    printu();
}

static void ps(int args, char *arguments[]){
    if(args!=1){
        invalidAmount();
        newln();
        return;
    }
    printProcess();
    printf(user);
}

static void loop(int args, char *arguments[]){
    if(args!=1){
        invalidAmount();
        newln();
        return;
    }
    loopProcess();
}

static void kill(int args, char *arguments[]){
    int pid;
    strToInt(arguments[1], &pid);
    if(args!=2){
        invalidAmount();
        return;
    }
    killProcess((uint64_t)pid);
    printu();
}

static void nice(int args, char *arguments[]){
    if(args!=3){
        invalidAmount();
        newln();
        return;
    }
    int pid;
    strToInt(arguments[1], &pid);
    int newPrio;
    strToInt(arguments[2],&newPrio);

    niceProcess((uint64_t)pid, (uint64_t)newPrio);
    printu();
}

static void block(int args, char *arguments[]){
    if(args!=2){
        invalidAmount();
        newln();
        return;
    }
    int pid;
    strToInt(arguments[1], &pid);
    blockProcess((uint64_t)pid);
    printu();
}

static void unblock(int args, char *arguments[]){
    if(args!=2){
        invalidAmount();
        newln();
        return;
    }
    int pid;
    strToInt(arguments[1], &pid);
    unblockProcess((uint64_t)pid);
    printu();
}

static void printu(){
    newln();
    printf(user);
}

static void mem(int args, char *arguments[]){
    if(args!=1){
        invalidAmount();
        newln();
        return;
    }
    memdata();
    printu();
}

static void sem(int args, char *arguments[]){
    if(args!=1){
        invalidAmount();
        newln();
        return;
    }
    sem_print();
    printu();
}

static void cat(int args, char *arguments[]){
    if(args!=2){
        invalidAmount();
        newln();
        return;
    }
    catFunc();
    printu();
}

static void wc(int args, char *arguments[]){
    if(args!=2){
        invalidAmount();
        newln();
        return;
    }
    wcFunc();
    printu();
}

static void filter(int args, char *arguments[]){
    if(args!=2){
        invalidAmount();
        newln();
        return;
    }
    filterFunc();
    printu();
}

static void pipe(int args, char *arguments[]){
    if(args!=1){
        invalidAmount();
        newln();
        return;
    }
    plist();
    printu();
}

static void phylo(int args, char *arguments[]){
    if(args!=2){           
        invalidAmount();
        newln();
        return;
    }
    newln();
    int count = atoi(arguments[1]);
    phyloFunc(count);
    printu();
}

static void testMM(int args, char *arguments[]){
    if(args!=1){
        invalidAmount();
        newln();
        return;
    }
    test_mm();
}


static void testSync(int args, char *arguments[]){
    if(args!=1){
        invalidAmount();
        newln();
        return;
    }
    test_sync();
}

static void testNoSync(int args, char *arguments[]){
    if(args!=1){
        invalidAmount();
        newln();
        return;
    }
    test_no_sync();
}

static void testProcesses(int args, char *arguments[]){
    if(args!=1){
        invalidAmount();
        newln();
        return;
    }
    test_processes();
}
static void testPriority(int args, char *arguments[]){
if(args!=1){
        invalidAmount();
        newln();
        return;
    }
    test_prio();
}