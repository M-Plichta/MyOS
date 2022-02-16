#include "keyboard.h"
#include "isr.h"
#include "terminal.h"

/**
 * Keyboard driver code.
 * 
 * This keyboard driver utilizes a 256-byte buffer.
 * There are the read and write pointers which point
 * to the index of the buffer which was last read/written to.
 */
static uint8_t wptr; // Write pointer
static uint8_t rptr; // Read  pointer

static char buffer[256]; // The 256-byte buffer

// Stores the scancode to ascii layout
// Most keys are not implemented, and therefore are represented as '?'
static char scanToAscii[128] = 
   "??1234567890-=\r "
   "qwertyuiop[]\n?as"
   "dfghjkl;'`?#zxcv"
   "bnm,./\e?? ??????"
   "??!\"?$%^&*()_+\r "
   "QWERTYUIOP{}\n?AS"
   "DFGHJKL:@??~ZXCV"
   "BNM<>?\e?? ??????";

// Flags register can be used to store special flags, e.g. ctrl or caps lock.
// For now, only one is used.
// The first bit is used to signal is Shift is being pressed.
static uint8_t flags = 0;

// Converts from scancode to ascii
char keyboard_to_ascii(uint32_t key) {
   return *(scanToAscii + ((flags & 0x1)*64) + key);
}

static void keyboard_callback(registers_t regs) {
   // // Read the key pressed from port 0x60
   int keyPressed = inb(0x60);

   // If shift key pressed
   if ( keyPressed == 54  ) {
      flags |= 0x1;  // Set the flag corresponding to shift
   } else if ( keyPressed == 182 ) { // If shift key released
      flags &= 0xFE; // Clear the flag corresponding to shift
   } else {
      // Ignore key release
      if ( keyPressed < 64 ) {
         char ascii_char = keyboard_to_ascii(keyPressed);
         // Write the key into the buffer
         *(buffer+wptr) = ascii_char;
         wptr++;

         // Display typed characters
         kprintf("%c", ascii_char);

         // Termial functions.
         addChar( ascii_char );
         if ( ascii_char == '\n' ) // When Enter is pressed, signal to the terminal
            submit();
      }
   }
}

// This function is how other applications can interface with the keyboard, and read it's input
char read_buffer() {
   // If the two pointers are equal, no unread characters.
   if ( rptr == wptr )
      return NULL;

   // Otherwise, return the character at the current read pointer
   return *(buffer+rptr++);
}

void init_keyboard() {
   wptr = 0; rptr = 0; // Initializing pointers

   register_interrupt_handler(IRQ1, &keyboard_callback);
}