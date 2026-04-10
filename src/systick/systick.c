#include "systick/systick.h"
#include "config.h"

volatile uint32_t global_tick_counter = 0;

void SysTick_Handler(void) 
{
    global_tick_counter++;

    SCB->ICSR |= ICSR_PENDSVSET;
}

void systick_init(void)
{
    *((volatile uint32_t *)0xE000ED20) |= (0xFFu << 16);
    *((volatile uint32_t *)0xE000ED20) |= (0xFEu << 24);
    
    SYSTICK->CSR = 0;
    SYSTICK->RVR = (CLOCK_HZ / TICK_HZ) - 1;
    SYSTICK->CVR = 0;

    SYSTICK->CSR = CSR_ENABLE | CSR_TICKINT | CSR_CLKSOURCE;
}