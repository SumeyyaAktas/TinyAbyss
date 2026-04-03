#include "uart/pl011.h"

void uart_init(void)
{
    UART0->CR = 0;

    while (UART0->FR & FR_BUSY);
    
    UART0->LCRH &= ~LCRH_FEN;
    UART0->IBRD = UART_IBRD;
    UART0->FBRD = UART_FBRD;
    UART0->LCRH = LCRH_WLEN_8 | LCRH_FEN;
    UART0->ICR = 0x7FFU;
    UART0->CR = CR_UARTEN | CR_TXE | CR_RXE;
}

void uart_putc(char c)
{
    while (UART0->FR & FR_TXFF);
    UART0->DR = c;

    if (c == '\n')
    {
        uart_putc('\r');
    }
}

void uart_puts(const char *s)
{
    while (*s)
    {
        uart_putc(*s++);
    }
}

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