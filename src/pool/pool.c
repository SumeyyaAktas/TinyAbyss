#include <stddef.h>
#include "pool/pool.h"

static Pool tcb_pool_header;                    
static Chunk tcb_pool_memory[TCB_POOL_SIZE];     
static Pool *tcb_pool = NULL;

void pool_init(void)
{
    tcb_pool = &tcb_pool_header;
    tcb_pool->chunk_arr  = tcb_pool_memory;
    tcb_pool->free_chunk = tcb_pool_memory;

    for (size_t i = 0; i < TCB_POOL_SIZE - 1; i++)
    {
        tcb_pool_memory[i].next = &tcb_pool_memory[i + 1];
    }

    tcb_pool_memory[TCB_POOL_SIZE - 1].next = NULL;
}

void* pool_alloc(Pool *self)
{
    if (self == NULL || self->free_chunk == NULL)
    {
        return NULL;
    }

    Chunk* first_block = self->free_chunk;
    self->free_chunk = first_block->next;
    return first_block;
}

void pool_free(Pool *self, void* ptr)
{
    if (self == NULL || ptr == NULL)
    {
        return;
    }

    Chunk* freed = ptr;
    freed->next = self->free_chunk;
    self->free_chunk = freed;
}

TCB_t *tcb_alloc(void)
{
    __asm__ volatile("cpsid i");         
    TCB_t *tcb = (TCB_t *)pool_alloc(tcb_pool);
    __asm__ volatile("cpsie i");         
    return tcb;
}

void tcb_free(TCB_t *tcb)
{
    __asm__ volatile("cpsid i");
    pool_free(tcb_pool, tcb);
    __asm__ volatile("cpsie i");
}