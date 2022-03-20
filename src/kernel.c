#include "globals.h"
#include "multiboot.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"

static int busy_wait_counter = 0;

void
abort( ) {
   kprintf ( "\n**** ABORT! ****\n");
   kprintf ( "Processor halted\n");
   asm ( "hlt;\n\t" );
}

void busy_wait() {
   for (int i = 0; i < 100000000; i++) {
      busy_wait_counter = i;
   }
}

void
kmain( void* mbd, unsigned int magic ) {
   vgainit( );
   status ("MyOS");

   // GDT and IDT setup function
   setup_interrupts();

   /**
    * @brief Testing interrupts:
    * 
    * Here, I show how the Programmable Interrupt Timer can be used
    * to measure the time taken between the start and stop functions.
    * 
    * The code below measures how long the busy_wait() function lasts.
    */
   start_timer( 2000 );
   busy_wait();
   stop_timer();
   kprintf("Total time taken: %d\n", get_timer());

   while(1) {  } // Stops the main thread from terminating
}