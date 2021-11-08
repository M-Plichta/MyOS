#include "globals.h"
#include "multiboot.h"

void
abort( ) {
   kprintf ( "\n**** ABORT! ****\n");
   kprintf ( "Processor halted\n");
   asm ( "hlt;\n\t" );
}

void
kmain( void* mbd, unsigned int magic ) {
   vgainit  ( );

   status ("MyOS");

   getbootinfo (mbd, magic);

   abort( );
}
