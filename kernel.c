#include "globals.h"
#include "multiboot.h"
#include "descriptor_tables.h"
#include "heap.h"

static int deleteLater = 0;

void
abort( ) {
   kprintf ( "\n**** ABORT! ****\n");
   kprintf ( "Processor halted\n");
   asm ( "hlt;\n\t" );
}

void wait() {
   for (int i = 0; i < 100000000; i++) {
      deleteLater = i;
   }
}

void
kmain( void* mbd, unsigned int magic ) {
   vgainit  ( );

   status ("MyOS");
   setup_interrupts();

   asm volatile("sti");
   init_keyboard();
   // init_timer(50);   // IRQ 0 tiemr interrupt
   
   // getbootinfo (mbd, magic);
   // abort( );

   /* Avail. mem: 0-0x9fx00 + 0x100000-0x7ee0000 */

   // Page Directory
   uint32_t test = 0b11111111111111111111111111111111;
   kprintf("kprintf = %032b\n", test);
   // init_test();

   while(1) {  } // Stops the main thread from terminating
}