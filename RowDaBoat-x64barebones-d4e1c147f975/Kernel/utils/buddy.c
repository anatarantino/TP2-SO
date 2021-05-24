// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef MEMORY

#include <memory.h>
#include <prints.h>
#include <strings.h>

enum powersOfTwo{KILO = 10, MEGA = 20, GIGA = 30};
#define MIN_ALLOC_LOG_2 6                  //64 B min aloc
#define MAX_LEVELS (30 - MIN_ALLOC_LOG_2)  //1GB MB max aloc
#define BIN_POW(x) (1 << (x))

typedef struct list_t {
      uint8_t is_free;
      uint8_t level;
      struct list_t *prev, *next;
} list_t;

static void printBlockSize(uint8_t exp);
static void initList(list_t *list);
static void pushList(list_t *list, list_t *entry);
static void removeList(list_t *entry);
static list_t *popList(list_t *list);
static int isListEmpty(list_t *list);
static void addNode(list_t* list, list_t* node , uint8_t level);
static list_t * getAddress(list_t *node);
static list_t * getBuddy(list_t *node);
static uint8_t getBucket(uint32_t request);
static int getFirstAvailableLevel(uint8_t minBucket);
static uint8_t isPowerOf2(uint32_t num);

static list_t *base_ptr;
static uint32_t max_size;
static uint8_t bucket_count;
static list_t buckets[MAX_LEVELS];


void initializeMem(void * base,uint64_t size){
    if (base == NULL){
        return;
    }
    base_ptr=base;
    max_size=size;
    bucket_count = log2(max_size) - MIN_ALLOC_LOG_2 + 1;

    if(bucket_count < 1){
        return; 
    }

    if(bucket_count > MAX_LEVELS){
        bucket_count = MAX_LEVELS;
    }

    for(uint64_t i = 0; i < bucket_count; i++){
        initList(&buckets[i]);
        buckets[i].is_free = 0;
        buckets[i].level = i;
    }

    addNode(&buckets[bucket_count-1],base_ptr,bucket_count);

}

void * memalloc(uint32_t nbytes){
    if(nbytes == 0){
        return NULL;
    }

    uint32_t size_needed = nbytes + sizeof(list_t);

    size_needed+=sizeof(list_t);

    if(size_needed > max_size){
        return NULL;
    }

    uint8_t bucket=getBucket(nbytes);

    int first_bucket=getFirstAvailableLevel(bucket);

    if(first_bucket==-1){
        return NULL;
    }

    list_t *node;

    for(node=popList(&buckets[first_bucket]) ; bucket<first_bucket ; first_bucket--){
        node->level--;
        addNode(&buckets[first_bucket-1],getBuddy(node),first_bucket-1);
    }
    node->is_free=0;

    return (void *)node;
}

void memfree(void * ptr){
    if(ptr == NULL){
        return;
    }
    
    list_t *node=(list_t *)ptr -1;

    node->is_free=1;

    list_t *buddy=getBuddy(node);

    while(node->level != bucket_count-1 && buddy->level == node->level && buddy->is_free){
        removeList(buddy);
        node=getAddress(node);
        node->level++;
        buddy=getBuddy(node);
    }
    pushList(&buckets[node->level],node);
}

void memData(){
    list_t *ptr,*aux;
    uint32_t index=0;
    uint32_t av_space=0;

    for(int i=bucket_count-1;i>=0;i++){
        ptr=&buckets[i];
        
        if(!isListEmpty(ptr)){
            printf("Available blocks size: ");
            printBlockSize(i+MIN_ALLOC_LOG_2);

            for(aux=ptr->next,index=0;aux!=ptr;index++,aux=aux->next){
                printf("Node number: ");
                printInt(index);
                printf(". Pointer: 0x");
                printHex((uintptr_t) aux);
                printf(". Next: 0x");
                printHex((uintptr_t) aux->next);
                printf(". Prev: 0x");
                printHex((uintptr_t) aux->prev);
                printf(". Level: ");
                printInt((uintptr_t) aux->level);
                if(aux->is_free){
                    printf("The block is free.");
                    printNewLine();
                }else{
                    printf("The block isn't free.");
                    printNewLine();
                }
                printNewLine();
                av_space+=index*BIN_POW(i + MIN_ALLOC_LOG_2);
            }
        }
    }
    printf("Available space: ");
    printInt(av_space);
    printf(" B.");
    printNewLine();

}

static void printBlockSize(uint8_t exp){
    char * aux;
    if(exp < KILO){
        aux = " B.";
    } else if ( exp < MEGA) {
        aux = " KB.";
        exp -= KILO;
    } else if (exp < GIGA){
        aux = " MB.";
        exp -= MEGA;
    } else {
        aux = " GB.";
        exp -= GIGA;
    }
    printInt(1 << exp);
    printNewLine();
    printf(aux);
}

static void initList(list_t *list){
    list->prev = list;
    list->next = list;
}

static void pushList(list_t *list, list_t *entry){
    list_t *prev = list->prev;
    entry->prev = prev;
    entry->next = list;
    prev->next = entry;
    list->prev = entry;
}

static void removeList(list_t *entry){
    list_t *prev = entry->prev;
    list_t *next = entry->next;
    prev->next = next;
    next->prev = prev;
}

static list_t *popList(list_t *list){
    list_t *back = list->prev;
    if (back == list) return NULL;
    removeList(back);
    return back;
}

static int isListEmpty(list_t *list){
    return list->prev == list;
}

static void addNode(list_t* list, list_t* node , uint8_t level){
    node->is_free = 1;
    node->level = level;
    pushList(list, node);
}

static list_t * getAddress(list_t *node){
    uint8_t level = node->level;
    uintptr_t currentOffset = (uintptr_t)node - (uintptr_t)base_ptr;
    uintptr_t newOffset = currentOffset ^ BIN_POW(MIN_ALLOC_LOG_2 + level);

    return (list_t*)(newOffset + (uintptr_t)base_ptr);
}

static list_t * getBuddy(list_t *node){
    uint8_t level = node->level;
    uintptr_t mask = BIN_POW(MIN_ALLOC_LOG_2 + level);
    mask = ~mask; 

    uintptr_t currentOffset = (uintptr_t)node - (uintptr_t)base_ptr;
    uintptr_t newOffset = currentOffset & mask;  

    return (list_t*)(newOffset + (uintptr_t)base_ptr);
}

static uint8_t getBucket(uint32_t request){

    uint8_t aux = log2(request);
    if(aux < MIN_ALLOC_LOG_2)
        return 0;

    aux -= MIN_ALLOC_LOG_2;
    if(isPowerOf2(request))
        return aux;

    return aux + 1;
}

static int getFirstAvailableLevel(uint8_t minBucket){
    for(; minBucket < bucket_count && isListEmpty(&buckets[minBucket]); minBucket++);
    if((minBucket < bucket_count)){
        return minBucket;
    }
    return -1;
}

static uint8_t isPowerOf2(uint32_t num) {
      return num && !(num & (num - 1));
}

#endif
