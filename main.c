#include "uart/pl011.h"
#include "systick/systick.h"

void print_uint32(uint32_t n)
{
    if (n < 10)
    {
        uart_putc('0' + n);
        return;
    }

    print_uint32(n / 10);
    uart_putc('0' + (n % 10));
}

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

    systick_init();
    __asm__ volatile("cpsie i");

    uint32_t last_tick = 0;

    while (1)
    {
        if (global_tick_counter != last_tick)
        {
            last_tick = global_tick_counter;

            uart_puts("Tick: ");
            print_uint32(last_tick); 
            uart_puts("\n");
        }
    }

    hcf();
}