#include "globals.h"

inline void
outb ( uint16_t port, uint8_t n ) {
   asm volatile (
                  "outb %0, %1;\n\t"
                :                       // Outputs
                : "a" (n), "Nd" (port)  // Inputs
                );
}

inline uint8_t
inb ( uint16_t port ) {
  uint8_t ret;
  asm volatile ( "inb %1, %0"
                  : "=a"(ret)
                  : "Nd"(port) );
  return ret;
}

// Waits a few microseconds
inline void
io_wait ( void ) {
  outb(0x80, 0);
}