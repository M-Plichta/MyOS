#include "globals.h"
#include "isr.h"

void isr_handler(registers_t regs) {
    kprintf("recieved interrupt: %d\n", regs.int_no);
}