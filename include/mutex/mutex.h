#ifndef MUTEX_H
#define MUTEX_H

#include <stdint.h>
#include "config.h"

struct TCB;
typedef struct TCB TCB_t;

typedef struct Mutex
{
    volatile uint8_t locked;
    int8_t owner;

    TCB_t *wait_queue[MAX_TASKS];
    uint8_t head;
    uint8_t tail;
} Mutex_t;

void mutex_unlock(Mutex_t *m);
void mutex_lock(Mutex_t *m);

#endif