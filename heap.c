#include "heap.h"

extern uint32_t end;
uint32_t free_address = &end;

/**
 * @brief Simple, pre-paging kmalloc function.
 * 
 * @param s Size of memory chunck
 * @param a if a == 1, chuck is page aligned
 * @param p physical address
 * @return uint32_t 
 */
uint32_t kmalloc(uint32_t size, uint8_t align, uint32_t * physical) {
    if (align == 1 && (free_address & 0xFFFFF000)) {
        free_address &= 0xFFFFF000;
        free_address += 0x1000;
    }

    if (physical) { // If physical != 0
        // The physical address will be stored
        *physical = free_address;
    }

    uint32_t temp = free_address;
    free_address += size;
    return temp;
}