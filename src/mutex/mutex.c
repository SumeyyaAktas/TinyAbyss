#include <stddef.h>
#include "mutex/mutex.h"
#include "task/task.h"
#include "systick/systick.h"

int mutex_enqueue(Mutex_t *m, TCB_t *tcb)
{
    uint8_t next = (m->head + 1) % MAX_TASKS;

    if (next == m->tail)
    {
        return -1;
    }

    m->wait_queue[m->head] = tcb;
    m->head = next;

    return 0;
}

TCB_t *mutex_dequeue(Mutex_t *m)
{
    if (m->head == m->tail)
    {
        return NULL;
    }

    TCB_t *tcb = m->wait_queue[m->tail];
    m->tail = (m->tail + 1) % MAX_TASKS;

    return tcb;
}

void mutex_unlock(Mutex_t *m)
{
    __asm__ volatile("cpsid i");

    TCB_t *current = task_table[current_task_index];

    if (m->owner != current->task_id) 
    {
        __asm__ volatile("cpsie i");
        return;
    }

    current->effective_priority = current->base_priority;

    TCB_t *next_waiter = mutex_dequeue(m);

    if (next_waiter != NULL)
    {
        m->owner  = next_waiter->task_id;
        m->locked = 1;
        m->holder = next_waiter;
        next_waiter->task_state  = READY;
        next_waiter->waiting_on  = NULL;
    }
    else
    {
        m->locked = 0;
        m->owner  = -1;
        m->holder = NULL;
    }

    __asm__ volatile("cpsie i");
}