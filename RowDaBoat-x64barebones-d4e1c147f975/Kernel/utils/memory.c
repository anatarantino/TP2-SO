#ifndef BUDDY

#include <memory.h>
#include <prints.h>
#include <colors.h>
#define TOTAL_MEM 128*1024*1024
#define HEADER_SIZE sizeof(Header)

//esto lo sacamos del libro de C de Kernighan (para el informe)
typedef long Align;

union header{
    struct{
        union header * next;
        unsigned size;
    }s;  
    Align x;
};
typedef union header Header;

static Header * base_p;
static Header * free_p;
uint64_t memory_size;

static void blockManager(Header * left, Header * right);

void initializeMem(void * base,uint64_t size){
    base_p = base;
    if(size > TOTAL_MEM){
        memory_size = TOTAL_MEM;
    }else{
        memory_size = size;
    }
    free_p = base_p;
    free_p->s.size = memory_size / HEADER_SIZE;
    free_p->s.next = NULL;
}

void * memalloc(uint32_t nbytes){
    if(nbytes == 0){
        return NULL;
    }
    uint32_t size_needed = (nbytes + HEADER_SIZE - 1) / HEADER_SIZE + 1;
    Header * current;
    Header * previous = free_p;
    uint32_t csize;
    for(current = previous ; current != NULL ; previous = current , current = current->s.next ){
        csize = current->s.size;
        if(csize >= size_needed){
            if(csize > size_needed){
                current->s.size -= size_needed;
                current += current->s.size;
                current->s.size = size_needed;
            }else{
                if(current == free_p){
                    free_p = current->s.next;
                }else{
                    previous->s.next = current->s.next;
                }
            }
            size_needed-=current->s.size;
            return (void *)(current + 1);
        }
    }
    return (void *) NULL;
}

void memfree(void * ptr){
    if(ptr == NULL){
        return;
    }
    Header * aux = (Header *) ptr - 1;
    if(aux < base_p || aux >= (base_p + memory_size * HEADER_SIZE) || ( ((char *)aux - (char *)base_p) % HEADER_SIZE !=0  )){
        return;
    }
    if(free_p == NULL){
        base_p = aux;
        base_p->s.next = NULL;
        return;
    }
    if(aux < free_p){
        blockManager(aux,free_p);
        free_p = aux;
        return;
    }

    Header * current = free_p;
    while(current->s.next != NULL && !(current < aux && aux < current->s.next)){
        current = current->s.next;
    }
    memory_size += aux->s.size;

    blockManager(aux,current->s.next);
    blockManager(current,aux);
}

void memdata(){
    uint64_t index = 0;
    Header * curr = free_p;

    printColor("Memory information:\n",VIOLET,BLACK);
    while(curr != NULL){
        printColor("Block number: ",YELLOW,BLACK);
        printInt(index);
        printNewLine();
        printColor("Current address: ",YELLOW,BLACK);
        printHex((uint64_t)curr);
        printNewLine();
        printColor("Free blocks: ",YELLOW,BLACK);
        printInt(curr->s.size);
        curr = curr->s.next;
        index++;
    }
}

static void blockManager(Header * left, Header * right){
    if(left == NULL || (right != NULL && left >= right)) {
        return;
    }
    if( left + left->s.size == right){
        left->s.size += right->s.size;
        left->s.next = right->s.next;
    }else{
        left->s.next = right;
    }
}

#endif
