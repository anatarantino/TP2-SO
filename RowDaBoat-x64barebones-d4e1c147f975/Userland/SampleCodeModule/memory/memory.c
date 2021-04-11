#define TOTAL_MEM 128
#define BLOCK_SIZE 64
#define NULL -1

/*
#ifdef BUDDY
    ...
#else
    ...
#endif
*/

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

int getTotalMem(){
    return TOTAL_MEM;
}

int getFreeMem(){
    return TOTAL_MEM - sizeof(start);
}

int getOccuppiedMem(){
    return sizeof(start);
}

void reserveMem(int size){
    
}
// consignas
// admitir liberacion de memoria
// poder reservar memoria
// poder consultar memoria total, ocupada, libre y cualquier otra variable que consideremos necesaria

// BUDDY
//tiene que poder intercambiarse con buddy (que es buddy???)