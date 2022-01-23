#include "keyboard.h"
#include "isr.h"

static char scanToAscii[100] = "\
??1234567890-=\r\
 qwertyuiop[]\n?\
asdfghjkl;'#?\\z\
xcvbnm,./??? ???\
";
// Stores the scancode to ascii layout
// Most keys are not implemented, and therefore are represented as '?'

char keyboard_to_ascii(uint8_t key) {
   return *(scanToAscii+key);
}

static void keyboard_callback(registers_t regs) {
   char c = inb(0x60);
   if ((int) c > 0)     // Ignore key release
      kprintf("%c", keyboard_to_ascii(c));
}

void init_keyboard() {
   register_interrupt_handler(IRQ1, &keyboard_callback);
}