#include "systick/systick.h"
#include "config.h"

#define SYST_CSR_REG (*((volatile uint32_t*) SYST_CSR))
#define SYST_RVR_REG (*((volatile uint32_t*) SYST_RVR))
#define SYST_CVR_REG (*((volatile uint32_t*) SYST_CVR))
#define ICSR_REG (*((volatile uint32_t*) ICSR))

volatile uint32_t global_tick_counter = 0;

void SysTick_Handler(void) 
{
    global_tick_counter++;
    
    ICSR_REG |= ICSR_PENDSVSET;
}

void systick_init(void)
{
    SYST_CSR_REG = 0;
    SYST_RVR_REG = (CLOCK_HZ / TICK_HZ) - 1;
    SYST_CVR_REG = 0;

    SYST_CSR_REG = CSR_ENABLE | CSR_TICKINT | CSR_CLKSOURCE;
}