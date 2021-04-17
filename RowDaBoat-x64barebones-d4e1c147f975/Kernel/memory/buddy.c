#ifndef MEMORY

#include <memory.h>

#define MIN_ALLOC_LOG_2 6                  //64 B min aloc
#define MAX_LEVELS (30 - MIN_ALLOC_LOG_2)  //1GB MB max aloc

#define BIN_POW(x) (1 << (x))

typedef struct list_t {
      uint8_t is_free;
      uint8_t level;
      struct list_t *prev, *next;
} list_t;

static list_t buckets[BUCKET_COUNT];
static size_t bucket_limit;
static uint8_t *base_ptr;

static void blockManager(Header * left, Header * right);

static list_t *base_ptr;
static uint32_t max_size;
static uint8_t bucket_count;
static list_t buckets[MAX_LEVELS];

void initializeMem(void * base,uint32_t size){
    if (base == NULL){
        return;
    }
    base_ptr=base;
    max_size=size;
    bucket_count = log2(max_size) - MIN_ALLOC_LOG_2 + 1;

    if(bucket_count < 1){
        return -1; 
    }

    if(bucket_count > MAX_LEVELS){
        bucket_count = MAX_LEVELS;
    }

    for(size_t i = 0; i < bucket_count; i++){
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
        node=getAdress(node);
        node->level++;
        buddy=getBuddy(node);
    }
    pushList(&buckets[node->level],node);
}

void memdata(){

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
