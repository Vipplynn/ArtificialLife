/* src/baremetal/linker.ld */
MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS
{
    .text : {
        KEEP(*(.vector_table)) /* Vector table must be at the start of Flash */
        *(.text*)
        *(.rodata*)
        _etext = .;
    } > FLASH

    .data : {
        _sdata = .;
        *(.data*)
        _edata = .;
    } > SRAM AT > FLASH

    .bss : {
        _sbss = .;
        *(.bss*)
        _ebss = .;
    } > SRAM
}
