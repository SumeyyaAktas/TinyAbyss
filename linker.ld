ENTRY(Reset_Handler)

MEMORY 
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS
{
    .text : 
    {
        *(.isr_vector)
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
    } > FLASH

    _data_flash_start = LOADADDR(.data);
    
    .data : 
    {
        _data_start = .;
        *(.data)
        *(.data*)
        _data_end = .;
    } > RAM AT> FLASH
    
    .bss : 
    {
        _bss_start = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        _bss_end = .;
    } > RAM

    _stack_top = ORIGIN(RAM) + LENGTH(RAM);
}