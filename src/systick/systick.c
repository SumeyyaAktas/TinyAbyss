#include "systick/systick.h"

#define SYST_CSR_REG (*((volatile uint32_t*) SYST_CSR))
#define SYST_RVR_REG (*((volatile uint32_t*) SYST_RVR))
#define SYST_CVR_REG (*((volatile uint32_t*) SYST_CVR))

volatile uint32_t global_tick_counter = 0;

void SysTick_Handler(void) 
{
    global_tick_counter++;
}

void systick_init(void)
{
    SYST_CSR_REG = 0;
    SYST_RVR_REG = 12000;
    SYST_CVR_REG = 0;

    SYST_CSR_REG = CSR_ENABLE | CSR_TICKINT | CSR_CLKSOURCE;
}