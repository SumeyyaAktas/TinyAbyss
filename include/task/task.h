#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include "config.h"

struct Mutex;
typedef struct Mutex Mutex_t;

typedef enum 
{
    READY,
    RUNNING,
    BLOCKED
} States;

typedef struct TCB
{
    uint32_t *stack_pointer;
    void (*entry)(void);
    uint32_t stack[STACK_SIZE];
    uint8_t task_id;
    States task_state;
    uint32_t wake_tick;
    Mutex_t *waiting_on;
    uint8_t base_priority;
    uint8_t effective_priority;
} TCB_t;

extern TCB_t *task_table[MAX_TASKS];
extern volatile uint8_t task_count;
extern volatile uint8_t current_task_index;

void task_create(TCB_t *tcb, void (*entry)(void), uint8_t task_id, uint8_t priority);
void task_register(TCB_t *tcb); 
void scheduler_next(void); 
void task_start(void);
void delay(uint32_t ticks);
void delay_ms(uint32_t ms);

#endif