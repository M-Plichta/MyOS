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
   for (int i = 0; i < 10000000; i++) {
      deleteLater = i;
   }
}

char keyboard_to_ascii(uint8_t key)
{
   char* _qwertzuiop = "qwertzuiop";
   char* _asdfghjkl = "asdfghjkl";
   char* _yxcvbnm = "yxcvbnm";
   char* _num = "1234567890";

   if(key == 0x1C) return '\n';
   if(key == 0x39) return ' ';
   if(key == 0xE)  return '\r';
   if(key >= 0x2 && key <= 0xB)
      return _num[key - 0x2];
   if(key >= 0x10 && key <= 0x1C) {
      return _qwertzuiop[key - 0x10];
   } else if(key >= 0x1E && key <= 0x26) {
      return _asdfghjkl[key - 0x1E];
   } else if(key >= 0x2C && key <= 0x32) {
      return _yxcvbnm[key - 0x2C];
   }

   // return key;    // Testing only
   
   return 0;
}

void
kmain( void* mbd, unsigned int magic ) {
   vgainit  ( );

   status ("MyOS");

   init_gdt();
   init_idt();

   asm volatile ("int $0x3");
   asm volatile ("int $0x4");
   // char prevChar = ' ';
   // while (1) {
   //    char c = kinb(0x60);

   //    if (prevChar != c && (int) c > 0)
   //       kprintf("%c", keyboard_to_ascii(c));

   //    prevChar = c;
   // }

   // getbootinfo (mbd, magic);

   // abort( );
}