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

