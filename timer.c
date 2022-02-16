#include "timer.h"
#include "isr.h"

int INITIALIZED = 0;
int counter = 0;

// Callback function for when the IRQ0 is fired
void timer_callback( registers_t regs ) {
    counter++;
}

// Starts timer
void start_timer ( int frequency ) {
    ASSERT( frequency != 0 ); // Making sure we don't divide be zero

    // Register the callback function for the Programmable Interval Timer
    register_interrupt_handler(IRQ0, &timer_callback);

    // PIT runs at roughly 1.193182 MHz
    int divisor = 1193182 / frequency;

    outb (0x43, 0x36);          // Mode/Commad register
    _set_time_freq ( divisor ); // Setting timer freq
}

// Stops the timer by setting the function callback to NULL
void stop_timer  (  ) {
    register_interrupt_handler(IRQ0, NULL);
}

// Used to set the frequency of the PIT
void _set_time_freq( int divisor ) {
    outb(0x40, (uint8_t) divisor & 0xFF);
    outb(0x40, (uint8_t) ((divisor&0xFF00) >> 8));
}

// Returns current count value.
int get_timer() {
    return counter;
}