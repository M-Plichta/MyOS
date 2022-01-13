#include "keyboard.h"
#include "isr.h"

char keyboard_to_ascii(uint8_t key) {
   char* layer1 = "qwertyuiop";
   char* layer2 = "asdfghjkl";
   char* layer3 = "zxcvbnm";
   char* nums = "1234567890";

   if(key == 0x1C) return '\n';
   if(key == 0x39) return ' ';
   if(key == 0xE)  return '\r';
   if(key >= 0x2 && key <= 0xB)
      return nums[key - 0x2];
   if(key >= 0x10 && key <= 0x1C) {
      return layer1[key - 0x10];
   } else if(key >= 0x1E && key <= 0x26) {
      return layer2[key - 0x1E];
   } else if(key >= 0x2C && key <= 0x32) {
      return layer3[key - 0x2C];
   }

   // return key;    // Testing only
   
   return 0;
}

static void keyboard_callback(registers_t regs) {
    char c = inb(0x60);
    if ((int) c > 0)     // Ignore key release
        kprintf("%c", keyboard_to_ascii(c));
}

void init_keyboard() {
   register_interrupt_handler(IRQ1, &keyboard_callback);
}