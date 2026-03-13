#include <stdint.h>

extern uint32_t _sdata, _edata, _etext, _sbss, _ebss;
void main(void);

// Reset Handler: The first code that runs on power-up
void Reset_Handler(void) {
    // 1. Copy .data section from FLASH to SRAM
    uint32_t *src = &_etext;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) *dst++ = *src++;

    // 2. Initialize .bss section to zero
    dst = &_sbss;
    while (dst < &_ebss) *dst++ = 0;

    // 3. Jump to main
    main();

    // 4. Trap if main returns
    while (1);
}

// Minimal Vector Table
__attribute__((section(".vector_table")))
const uint32_t vector_table[] = {
    0x20010000,          // Initial Stack Pointer (Top of SRAM)
    (uint32_t)Reset_Handler // Reset Vector
};
