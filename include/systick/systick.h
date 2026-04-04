#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

#define SYST_CSR 0xE000E010
#define SYST_RVR 0xE000E014
#define SYST_CVR 0xE000E018

#define CSR_ENABLE (1 << 0)
#define CSR_TICKINT (1 << 1)
#define CSR_CLKSOURCE (1 << 2)

extern volatile uint32_t global_tick_counter;

void systick_init(void);

#endif