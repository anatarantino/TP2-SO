// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <tests.h>
#include <semLib.h>
#include <processLib.h> 
#include <strings.h>
#include <prints.h>
#include <timeRTC.h>


#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID "sem"
#define NULL 0

int64_t global;  //shared memory

static void slowInc(int64_t *p, int64_t inc){
  int64_t aux = *p;
  aux += inc;
  yield();
  *p = aux;
}

static int inc(int args, char **argv){
  int sem = atoi(argv[1]);
  int value = atoi(argv[2]);
  int N = atoi(argv[3]);
  int semIndex;
  uint64_t i;

  if (sem){
    if((semIndex = sem_open(SEM_ID, 1)) == -1){
      printf("ERROR OPENING SEM");
      newln();
      return -1;
    }
  }

  for (i = 0; i < N; i++){
  
    if (sem){
      sem_wait(semIndex);
    } 
    slowInc(&global, value);
    if (sem){
      sem_post(semIndex);
    } 

  }

  if (sem){
    sem_close(semIndex);
  } 
  if(sem){
    printf("SYNC Final value: ");
    printIntNeg(global);
    newln();
  }else{
    printf("NO SYNC Final value: ");
    printIntNeg(global);
    newln();
  }
  return 0;
}

void test_sync(){
  uint64_t i;
  global = 0;

  newln();
  printf("CREATING PROCESSES...(WITH SEM)");
  newln();

  char * argv1[]={"inc","1","1","10000"};
  char * argv2[]={"inc","1","-1","10000"};

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    addProcess(&inc,4,argv1,0,0);
    addProcess(&inc,4,argv2,0,0);
  }
}

void test_no_sync(){
  uint64_t i;
  global = 0;

  newln();
  printf("CREATING PROCESSES...(WITHOUT SEM)");
  newln();

  char * argv1[]={"inc","0","1","10000"};
  char * argv2[]={"inc","0","-1","10000"};

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    addProcess(&inc,4,argv1,0,0);
    addProcess(&inc,4,argv2,0,0);
  }
}

