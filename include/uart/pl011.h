#ifndef PL011_H
#define PL011_H

#include <stdint.h>

typedef volatile struct 
{
    uint32_t DR;            
    uint32_t RSR_ECR;       
    uint32_t _reserved0[4]; 
    uint32_t FR;            
    uint32_t _reserved1;    
    uint32_t ILPR;          
    uint32_t IBRD;          
    uint32_t FBRD;          
    uint32_t LCRH;          
    uint32_t CR;            
    uint32_t IFLS;          
    uint32_t IMSC;          
    uint32_t RIS;           
    uint32_t MIS;           
    uint32_t ICR;           
    uint32_t DMACR;         
} PL011_t;

#define UART0_BASE 0x4000C000U
#define UART0 ((PL011_t *) UART0_BASE)

#define FR_TXFF (1u << 5)    
#define FR_BUSY (1u << 3)  

#define LCRH_WLEN_8 (0x3u << 5)      
#define LCRH_FEN (1u << 4)                  

#define CR_RXE (1u << 9)  
#define CR_TXE (1u << 8)  
#define CR_UARTEN (1u << 0) 

#define UART_IBRD 6U
#define UART_FBRD 33U

void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *s);

#endif