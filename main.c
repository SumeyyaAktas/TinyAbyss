#include <stddef.h>
#include "uart/pl011.h"
#include "systick/systick.h"
#include "task/task.h"
#include "mutex/mutex.h"
#include "pool/pool.h"

Mutex_t uart_mutex = 
{
    .locked = 0,
    .owner = -1,
    .head = 0,
    .tail = 0
};

void task_low(void)
{
    while (1)
    {
        mutex_lock(&uart_mutex);
        uart_puts("L: holding mutex\n");

        for (volatile uint32_t i = 0; i < 5000000; i++);

        uart_puts("L: releasing mutex\n");
        mutex_unlock(&uart_mutex);
        delay_ms(1000);
    }
}

void task_med(void)
{
    while (1)
    {
        uart_puts("M: running\n");
        delay_ms(100);
    }
}

void task_high(void)
{
    delay_ms(50);

    while (1)
    {
        uart_puts("H: trying to lock\n");
        mutex_lock(&uart_mutex);

        uart_puts("H: acquired mutex\n");
        mutex_unlock(&uart_mutex);

        delay_ms(500);
    }
}

void idle_task(void)
{
    while (1)
    {
        __asm__ volatile("wfi");
    }
}

void main(void)
{
    uart_init();
    uart_puts("\nHello from DoRTOS!\n\n");

    pool_init();

    TCB_t *tcb_low = tcb_alloc();
    if (tcb_low == NULL) 
    { 
        uart_puts("Fatal: pool exhausted\n"); 
        while(1); 
    }
    
    TCB_t *tcb_med = tcb_alloc();
    if (tcb_med == NULL) 
    { 
        uart_puts("Fatal: pool exhausted\n"); 
        while(1); 
    }

    TCB_t *tcb_high = tcb_alloc();
    if (tcb_high == NULL) 
    { 
        uart_puts("Fatal: pool exhausted\n"); 
        while(1); 
    }

    TCB_t *tcb_idle = tcb_alloc();
    if (tcb_idle == NULL) 
    { 
        uart_puts("Fatal: pool exhausted\n"); 
        while(1); 
    }

    task_create(tcb_low, task_low, 1, 1);   
    task_create(tcb_med, task_med, 2, 2);   
    task_create(tcb_high, task_high, 3, 3);   
    task_create(tcb_idle, idle_task, 0, 0);

    task_register(tcb_low);
    task_register(tcb_med);
    task_register(tcb_high);
    task_register(tcb_idle);

    systick_init();

    __asm__ volatile("cpsie i");

    task_start();
}