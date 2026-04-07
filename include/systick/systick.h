#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

typedef struct
{
    volatile uint32_t CSR;  
    volatile uint32_t RVR;  
    volatile uint32_t CVR;  
} SysTick_t;

typedef struct
{
    volatile uint32_t RESERVED0;
    volatile uint32_t ICSR;
} SCB_t;

#define SYSTICK ((SysTick_t *) 0xE000E010)
#define SCB ((SCB_t *) 0xE000ED00)

#define CSR_ENABLE (1 << 0)
#define CSR_TICKINT (1 << 1)
#define CSR_CLKSOURCE (1 << 2)

#define ICSR_PENDSVSET (1 << 28)

extern volatile uint32_t global_tick_counter;

void systick_init(void);

#endif