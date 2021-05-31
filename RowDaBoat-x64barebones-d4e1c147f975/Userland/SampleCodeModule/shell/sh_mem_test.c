#include <sh_mem_test.h>
#include <syscall.h>
#include <memoryLib.h>
#include <prints.h>

#define SEMID 5
#define N 100000

static void slowInc(uint64_t *p, uint64_t inc);

static void slowInc(uint64_t *p, uint64_t inc){
  uint64_t aux = *p;
  aux += inc;
  for (int k = 0; k < 1000; k++);
  *p = aux;
}


void inc(){
	uint64_t * ptr = shm_open(5, 8);

	if (ptr == 0)
		printf("Error shm_open\n");

  	for (int i = 0; i < N+2 ; i++) //ponemos +2 para ver que despues el puntero termina en algo distinto de 0
		slowInc(ptr, 1);
	printf("El valor de ptr es:");
	printInt(*ptr);
	newln();
}



void dec(){
	uint64_t * ptr = shm_open(5, 8);

	if (ptr == 0)
		printf("Error shm_open\n");

	for (int i = 0; i < N; i++)
		slowInc(ptr, -1);
	printf("El valor del ptr es:");
	printInt(*ptr);
}
