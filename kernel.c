#include "globals.h"
#include "multiboot.h"
#include "descriptor_tables.h"
#include "timer.h"

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

   kprintf("MyOS> ");

   status ("MyOS");
   setup_interrupts();

   init_keyboard();
   // init_paging();

   // start_timer( 6000 );   // IRQ 0 timer interrupt


   // stop_timer();

   // kprintf("Time passed = %d\n", get_timer());

   // getbootinfo (mbd, magic);
   // abort( );

   /* Avail. mem: 0-0x9fx00 + 0x100000-0x7ee0000 */


   // Code for testing paging
   // kprintf("Paging activated!\n");
   // uint32_t *ptr = (uint32_t *) 0xA0000000;
   // uint32_t do_page_fault = *ptr;

   while(1) {  } // Stops the main thread from terminating
}