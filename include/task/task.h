#ifndef TASK_H
#define TASK_H

#include <stdint.h>

typedef enum 
{
    READY,
    RUNNING,
    BLOCKED
} States;

typedef struct 
{
    uint32_t *stack_pointer;
    void (*entry)(void);
    uint32_t stack[256];
    uint8_t task_id;
    States task_state;
} TCB_t;

void task_create(TCB_t *tcb, void (*entry)(void), uint8_t task_id);

#endif 