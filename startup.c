#include <stdint.h>

void Reset_Handler(void);

extern uint32_t _stack_top, _data_flash_start, _data_start, _data_end, _bss_start, _bss_end;
extern void main(void);

const void * const _Vectors[] __attribute__((section(".isr_vector"))) = 
{
    &_stack_top,
    Reset_Handler
};

void Reset_Handler(void)
{
    uint32_t *src = &_data_flash_start;
    uint32_t *dest = &_data_start;

    while (dest < &_data_end)
    {
        *dest++ = *src++;
    }

    uint32_t *_bss_start_p = &_bss_start;

    while (_bss_start_p < &_bss_end)
    {
        *_bss_start_p++ = 0;
    }

    main();
}