#include "globals.h"
#include "multiboot.h"
#include "descriptor_tables.h"

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

   // asm volatile ("int $0x3");
   // asm volatile ("int $0x4");

   // asm volatile("sti");
   // init_keyboard();
   // init_timer(50);

   // int counter = 0;
   while(1) {
      // wait();
      // kprintf("\nCOUNTER%d\n", counter++);
   } // Stops the main thread from terminating
   
   // getbootinfo (mbd, magic);

   // abort( );
}