#ifndef PAGING_H
#define PAGING_H

#include "globals.h"
#include "isr.h"

typedef struct page {
    uint32_t present    : 1;    // Page present in memory
    uint32_t rw         : 1;    // r if clear, r+w is set
    uint32_t user       : 1;    // Supervisor level if clear
    uint32_t accessed   : 1;    // Has been accessed
    uint32_t dirty      : 1;    // Has been written to
    uint32_t unused     : 7;    // Unused
    uint32_t frame      : 20;   // Address of frame
} page_t;

typedef struct page_table {
    page_t pages[1024];
} page_table_t;

typedef struct  page_directory
{
    page_table_t *  tables[1024];         // Logical addresses of page tables (for r/w operations)
    uint32_t        tablesPhysical[1024]; // Physical addresses of page tables (for CPU instructions)
    uint32_t        physicalAddr;         // Pointer to tablesPhysical
} page_directory_t;

void init_paging();
void init_test();

#endif