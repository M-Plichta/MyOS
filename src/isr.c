#include "isr.h"
#include "globals.h"

isr_t interrupt_handlers[256];

// This function assigns the callback function for the corresponding interrupt ISR.
void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

// ISR handler function.
// Calls the callback function with corresponding registers.
void isr_handler(registers_t regs) {
    if (interrupt_handlers[regs.int_no] != 0) {
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(&regs);
    }
}

// IRQ handler function.
// Sends the End Of Interrupt signal to the PIC and
// calls the callback function with corresponding registers.
void irq_handler(registers_t regs) {
	PIC_sendEOI(regs.int_no);

	if (interrupt_handlers[regs.int_no] != 0) {
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(&regs);
	}
}

// Sends and end of interrupt command to the PIC
void PIC_sendEOI(uint8_t irq) {
  	// If the irq came from the Slave pic, then send EOI command to Slave
	if(irq >= 40)
		outb(0xA0, 0x20);

	outb(0x20, 0x20);
}