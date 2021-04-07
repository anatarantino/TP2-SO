#define TOTAL_MEM 128
#define BLOCK_SIZE 64
#define NULL -1

char start[TOTAL_MEM];
char *next=start;

void * memoryManager(){
    char *r=next;
    if((next-start) <= (TOTAL_MEM-BLOCK_SIZE)){
        next+=BLOCK_SIZE;
        return r;
    }else{
        return NULL;//buscar reemplazo para NULL
        //si no hay mas lugar hace falta resizearlo o no?
    }

    //no admite free

    //asigna siempre la misma cantidad de memoria o NULL

}