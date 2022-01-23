#ifndef HEAP_H
#define HEAP_H

#include "globals.h"

// uint32_t kmalloc( uint32_t s, uint8_t a, uint32_t *p );

uint32_t kmalloc( uint32_t size, int align );
uint32_t kmalloc_phys( uint32_t size, int align, uint32_t *phys );

#endif