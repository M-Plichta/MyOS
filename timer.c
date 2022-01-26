#include "timer.h"
#include "isr.h"
// #include "isr.c"

int INITIALIZED = 0;
int counter = 0;

void timer_callback( registers_t regs ) {
    counter++;
    kprintf("Counter == %d\n", counter);
}

void start_timer ( int frequency ) {
    ASSERT( frequency != 0 ); // Making sure we don't divide be zero

    // Register the callback function for the Programmable Interrupt Timer
    register_interrupt_handler(IRQ0, &timer_callback);

    // PIT runs at roughly 1.193182 MHz
    int divisor = 1193182 / frequency;

    outb (0x43, 0x36);          // Mode/Commad register
    _set_time_freq ( divisor ); // Setting timer freq
}

void stop_timer  (  ) {
    register_interrupt_handler(IRQ0, NULL);
}

void _set_time_freq( int divisor ) {
    outb(0x40, (uint8_t) divisor & 0xFF);
    outb(0x40, (uint8_t) ((divisor&0xFF00) >> 8));
}

int get_timer() {
    return counter;
}

// void start_timer( int frequency ) {
//     ASSERT(  )
// }

// void set_pit_count(unsigned count) {
// 	// Disable interrupts
// 	asm ("cli\n");
 
// 	// Set low byte
// 	outb(0x40,count&0xFF);		// Low byte
// 	outb(0x40,(count&0xFF00)>>8);	// High byte
// 	return;
// }