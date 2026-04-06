#include <stdint.h>

void Reset_Handler(void);
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

extern uint32_t _stack_top, _data_flash_start, _data_start, _data_end, _bss_start, _bss_end;
extern void main(void);

const void * const _Vectors[] __attribute__((section(".isr_vector"))) =
{
    &_stack_top,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0,
    0,
    0,
    0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,
};

void NMI_Handler(void) { while (1); }
void HardFault_Handler(void) { while (1); }
void MemManage_Handler(void) { while (1); }
void BusFault_Handler(void) { while (1); }
void UsageFault_Handler(void) { while (1); }
void SVC_Handler(void) { while (1); }
void DebugMon_Handler(void) { while (1); }

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