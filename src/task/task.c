#include <stdint.h>
#include "task/task.h"
#include "uart/pl011.h"
#include "systick/systick.h"

#define ICSR_REG (*((volatile uint32_t*) ICSR))

TCB_t *task_table[MAX_TASKS]; 
uint8_t task_count = 0; 
uint8_t current_task_index = 0; 

void task_create(TCB_t *tcb, void (*entry)(void), uint8_t task_id)
{
    tcb->entry = entry;
    tcb->task_id = task_id;
    tcb->task_state = READY;

    uint32_t *sp = &(tcb->stack[256]);

    *(--sp) = 0x01000000;        
    *(--sp) = (uint32_t)entry;   
    *(--sp) = 0xFFFFFFFD;        
    *(--sp) = 0;                 
    *(--sp) = 0;                 
    *(--sp) = 0;                 
    *(--sp) = 0;                 
    *(--sp) = 0;                 

    *(--sp) = 0;                 
    *(--sp) = 0;                 
    *(--sp) = 0;                 
    *(--sp) = 0;                 
    *(--sp) = 0;                 
    *(--sp) = 0;                 
    *(--sp) = 0;             
    *(--sp) = 0;                

    tcb->stack_pointer = sp;     
}

void task_register(TCB_t *tcb) 
{
    if (task_count < MAX_TASKS)
    {
        task_table[task_count] = tcb;
        task_count++;
    }
}

void scheduler_next(void)
{
    for (uint8_t i = 0; i < task_count; i++)
    {
        current_task_index++;

        if (current_task_index >= task_count) 
        {
            current_task_index = 0;
        }

        TCB_t *tcb = task_table[current_task_index];

        if (tcb->task_state == BLOCKED && global_tick_counter >= tcb->wake_tick)
        {
            tcb->task_state = READY;
        }

        if (tcb->task_state == READY)
        {
            return;
        }
    }
}