#include "heap.h"
#include "paging.h"

// #define ADDR_TO_INDEX(a) ((a - endKernel) / 0x1000)
// #define INDEX_TO_ADDR(a) ((a * 0x1000) + endKernel)

#define ADDR_TO_INDEX(a)  (a/32)
#define ADDR_TO_OFFSET(a) (a%32)

uint32_t *frame_bitmap; // Bitmap representing frame allocation
uint32_t  nframes;      // Number of Available frames

extern uint32_t endKernel;
extern void loadPageDir  ( uint32_t );
extern void enable_paging(  );

page_directory_t * kernelDir  = 0;
page_directory_t * currentDir = 0;

// Set the corresping bit in the bitmap to used (1)
static void set_frame( uint32_t addr ) {
    int idx = addr / 32;
    kprintf("SET_FRAME IDX = %d\n", idx);
    kprintf("SET_FRAME ADDR = %d\n", addr);
    frame_bitmap[idx] |= (1 << addr);           // Update bitmap with new present frame.
}

// Set the corresping bit in the bitmap to free (0)
static void clear_frame( uint32_t addr ) {
    int idx = addr / 32;
    frame_bitmap[idx] &= ~(1 << ADDR_TO_OFFSET(addr));
}

// Returns bool. 1 used, 0 free
static uint32_t test_frame( uint32_t addr ) {
    int idx = addr / 32;
    return (frame_bitmap[idx] & (1 << ADDR_TO_OFFSET(addr)));
}

// Allocates the first unused frame and updates the bitmap
// static uint32_t first_frame() {
//     uint32_t fidx, foff = 0;
//     while (test_frame((fidx * 0x1000) + foff)) {  // Scans the fram bitmap to find first free frame
//         if (ADDR_TO_INDEX(fidx) == nframes)
//             ERROR("ERROR");
//         foff++;

//         if ( foff == 32 ) {
//             foff = 0;
//             fidx ++;
//         }
//         kprintf("GOT HERE!\n");
//     }
//     set_frame((fidx * 0x1000) + foff); // Assign as used
//     return ((fidx * 0x1000) + foff);   // returns the address of a free page frame
// }

uint32_t first_frame() {
    for (uint32_t i = 0; i < 0x80; i++) {
        ASSERT(frame_bitmap[i] != 0xFFFFFFFF);
        for (uint32_t j = 0; j < 32; j++) {
            int toTest = 0x1 << j;
            if ( !(frame_bitmap[i]&toTest) ) {
                set_frame(i*32+j);
                return i*32+j;
            }
        }
    }
}

void page_fault(registers_t regs) {
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
    
    // The error code gives us details of what happened.
    int present   = !(regs.err_code & 0x1); // Page not present
    int rw = regs.err_code & 0x2;           // Write operation?
    int us = regs.err_code & 0x4;           // Processor was in user-mode?
    int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

    // Output an error message.
    kprintf("Page fault! ( ");
    if (present) kprintf("Present ");
    if (rw) kprintf("read-only ");
    if (us) kprintf("user-mode ");
    if (reserved) kprintf("reserved ");
    kprintf(") at 0x%x\n", faulting_address);
    ERROR("Page fault");
}

page_t *get_page( uint32_t addr, page_directory_t *dir ) {
    addr /= 0x1000;
    int32_t table_idx = addr / 1024;

    kprintf("addr = %x, table_idx = %x\n", addr, table_idx);

    kprintf("Table = %x\n", &dir->tables[table_idx]->pages[addr%1024]);
    if (dir->tables[table_idx]) {
        return &dir->tables[table_idx]->pages[addr%1024];
    }

    // Create page able
    uint32_t temp;
    dir->tables[table_idx] = (page_table_t *) kmalloc_phys(sizeof(page_table_t), 1, &temp);
    memset(dir->tables[table_idx], 0, 0x1000);
    dir->tablesPhysical[table_idx] = temp | 0x7;

    kprintf("addr(MOD)1024 = %d\n", addr%1024);

    return &dir->tables[table_idx]->pages[addr%1024];
}

void init_test() {
    uint32_t  mem_end_page = 0x1000000; // Assume we have 16MB
    nframes = mem_end_page / 0x1000;    // Stores the max number of pages
    
    kprintf("nframes = %d\n", nframes);
    frame_bitmap = (uint32_t *) kmalloc( /* 4096 / 32 = */ 0x80, 0);
    memset(frame_bitmap, 0, 0x80);

    // // Page Directory
    // page_directory_t * kernelDir = (page_directory_t *) kmalloc(sizeof(page_directory_t), 1);
    // memset(kernelDir, 0, sizeof(page_directory_t));
    // currentDir = kernelDir;

    // // TODO: Implement a automatic page allocation function :) 
    // int i = 0;
    // while (i < endKernel) {
    //     alloc_frame ( get_page(i, kernelDir), 0, 0 );
    //     i += 0x1000;
    //     if (i == (0x1000 * 31))
    //         break;
    //     // if (i == 0x5000)
    //     //     break;
    // }

    // Create page able
    // uint32_t temp;
    // kernelDir->tables[0] = (page_table_t *) kmalloc_phys(sizeof(page_table_t), 1, &temp);
    // memset(kernelDir->tables[0], 0, 0x1000);
    // kernelDir->tablesPhysical[0] = temp | 0x7;

    // alloc_frame( &kernelDir->tables[0]->pages[0], 0, 0 );
    // alloc_frame( &kernelDir->tables[0]->pages[1], 0, 0 );
    // alloc_frame( &kernelDir->tables[0]->pages[2], 0, 0 );

    // uint32_t test1 = first_frame();
    // uint32_t test2 = first_frame();
    // uint32_t test3 = first_frame();

    // kprintf("test1 = %x\n", test1);
    // kprintf("test2 = %x\n", test1);
    // kprintf("test3 = %x\n", test1);

    // register_interrupt_handler(14, page_fault);
    // switch_page_directory(kernelDir);


    // uint32_t i = 0;
    // uint32_t j = 31;
    
    // for (j = 0; j < 32; j++) {
    //     set_frame(i*32+j);
    // }
    // // for (i = 0; i < 5; i++) {
    // // }

    // // Test bitmap:
    // for (int x = 0; x < 5; x++)
    //     kprintf("bitmap = %032b\n", frame_bitmap[x]);
}

/**
 * @brief Function to allocate pages.
 * 
 * @param page The page to be set
 * @param is_kenrel Should the page be user-mode
 * @param is_writeable Should the page be writeable
 */
void alloc_frame( page_t *page, int is_kenrel, int is_writeable ) {
    if (page->frame != 0) { // If frame already allocated..
        return;             // then return
    } else {
        uint32_t frame = first_frame(); // Find in next free frame
        if ( frame == (uint32_t) -1 )     // If it was the last frame ...
            ERROR("No free frames!");   // that means there are no frames left.

        page->present = 1;
        page->rw    = (is_writeable) ? 1 : 0;
        page->user  = (is_kenrel) ? 0 : 1;
        page->frame = frame;
    }
}

/**
 * @brief Function to deallocate pages.
 * 
 * @param page The page to be deallocated
 */
void free_frame( page_t *page ) {
    if ( !(page->present) ) { // If the page is not allocated ...
        return;               // return
    } else {
        clear_frame((page->frame) * 32); // Clearing the frame in the bitmap
        page->present = 0;  // Clearing the frame address
    }
}

// void switch_page_directory( page_directory_t *dir ) {
//     currentDir = dir;

//     asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
//     uint32_t cr0;
//     asm volatile("mov %%cr3, %0": "=r"(cr0));
//     cr0 |= 0x80000000;
//     asm volatile("mov %0, %%cr0":: "r"(cr0));
// }