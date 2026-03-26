#include "uart/pl011.h"

void hcf(void)
{
    for (;;)
    {
        __asm__ volatile("wfi");
    }
}

void main(void)
{
    uart_init();
    uart_puts("\nHello from TinyAbyss!\n");
    
    hcf();
}