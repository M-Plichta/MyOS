#include "heap.h"
#include "paging.h"

extern uint32_t end;
uint32_t endKernel = &end;

// Allocates memory from the end of the kernel.
// Align determines if the memory is to be aligned to 4k or not
uint32_t kmalloc( uint32_t size, int align ) {
    if (align == 1 && (endKernel & 0x00000FFF)) {
        endKernel &= 0xFFFFF000;
        endKernel += 0x1000;
    }
    uint32_t temp = endKernel;
    endKernel += size;
    return temp;
}

// Same as kmalloc, except also returns the physical address.
// Usefull when setting up paging
uint32_t kmalloc_phys( uint32_t size, int align, uint32_t *phys) {
    if (align == 1 && (endKernel & 0x00000FFF)) {
        endKernel &= 0xFFFFF000;
        endKernel += 0x1000;
    }

    if (phys)
        *phys = endKernel;

    uint32_t temp = endKernel;
    endKernel += size;
    return temp;
}