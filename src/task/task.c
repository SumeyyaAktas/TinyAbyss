#include <stdint.h>
#include <stddef.h>
#include "task/task.h"
#include "mutex/mutex.h"
#include "uart/pl011.h"
#include "systick/systick.h"
#include "config.h"

TCB_t *task_table[MAX_TASKS]; 
volatile uint8_t task_count = 0;
volatile uint8_t current_task_index = 0;

void task_create(TCB_t *tcb, void (*entry)(void), uint8_t task_id, uint8_t priority)
{
    if (tcb == NULL || entry == NULL) 
    {
        return;
    }

    tcb->entry = entry;
    tcb->task_id = task_id;
    tcb->task_state = READY;
    tcb->waiting_on = NULL;
    tcb->wake_tick = UINT32_MAX;
    tcb->base_priority = priority;
    tcb->effective_priority = priority;

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
    __asm__ volatile("cpsid i");

    if (task_count < MAX_TASKS)
    {
        task_table[task_count] = tcb;
        task_count++;
    }

    __asm__ volatile("cpsie i");
}

void scheduler_next(void)
{
    uint8_t best_priority = 0;
    uint8_t best_index = current_task_index;
    uint8_t found = 0;

    for (uint8_t i = 0; i < task_count; i++)
    {
        uint8_t index = (current_task_index + 1 + i) % task_count;
        TCB_t *tcb = task_table[index];

        if (tcb->task_state == BLOCKED && global_tick_counter >= tcb->wake_tick)
        {
            tcb->task_state = READY;
        }

        if (tcb->task_state == BLOCKED && tcb->waiting_on != NULL && tcb->waiting_on->locked == 0)
        {
            tcb->task_state = READY;
            tcb->waiting_on = NULL;
        }

        if (tcb->task_state == READY)
        {
            if (!found)
            {
                found = 1;
                best_priority = tcb->effective_priority;
                best_index = index;
            }
            else if (tcb->effective_priority > best_priority)
            {
                best_priority = tcb->effective_priority;
                best_index = index;
            }
        }
    }

    if (!found)
    {
        uart_puts("FATAL: no READY task\n");
        while (1);
    }

    current_task_index = best_index;
}

void delay(uint32_t ticks)
{
    TCB_t *current = task_table[current_task_index];

    current->wake_tick = global_tick_counter + ticks;
    current->task_state = BLOCKED;

    SCB->ICSR |= ICSR_PENDSVSET;
}

void delay_ms(uint32_t ms)
{
    delay((ms * TICK_HZ) / 1000);
}