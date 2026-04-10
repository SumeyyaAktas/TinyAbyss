#ifndef POOL_H
#define POOL_H

#include "task/task.h"
#include "config.h"

#define TCB_POOL_SIZE MAX_TASKS

typedef union Chunk Chunk;
union Chunk 
{
    Chunk *next;
    TCB_t tcb;        
};

typedef struct Pool Pool;
struct Pool 
{
    Chunk* free_chunk;
    Chunk* chunk_arr; 
};

void pool_init(void);
TCB_t *tcb_alloc(void);
void tcb_free(TCB_t *tcb);

#endif