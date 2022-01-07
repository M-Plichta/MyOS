#include "globals.h"

inline void
outb (uint16_t port, uint8_t n) {
   asm volatile (
                  "outb %0, %1;\n\t"
                :                       // Outputs
                : "a" (n), "Nd" (port)  // Inputs
                );
}

inline void koutb (uint16_t port, uint8_t val) {
  asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

inline uint8_t kinb (uint16_t port) {
  uint8_t ret;
  asm volatile ( "inb %1, %0"
                  : "=a"(ret)
                  : "Nd"(port) );
  return ret;
}

inline uint16_t kinw (uint16_t port) {
  uint16_t ret;
  asm volatile ( "inw %1, %0"
                  : "=a"(ret)
                  : "Nd"(port) );
  return ret;
}