#ifndef __GLOBALS__
#define __GLOBALS__

#define BREAK(); asm volatile ( "xchg %bx, %bx;" ); /* Bochs magic break code */
#define NULL 0

// Error macro
#define ERROR(err_message) \
    do {                   \
        kprintf("ERROR at %s:%d, function %s():\n    %s",__FILE__, __LINE__, __FUNCTION__, err_message); \
        abort();           \
    } while (0)

// Assert macro
#define ASSERT(x)                           \
    do {                                    \
        if (!(x)) {                         \
            ERROR("Assertion failed: " #x); \
        }                                   \
    } while (0)

//
//	======================== TYPES ========================
//
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned long      uint32_t;
typedef unsigned long long uint64_t;

typedef unsigned long      size_t;

typedef char      int8_t;
typedef short     int16_t;
typedef long      int32_t;
typedef long long int64_t;

//
//	=================== PUBLIC FUNCTIONS ===================
//

// globals.c
void memset( uint8_t * , uint8_t, uint32_t );

// kernel.c
void kmain    ( void *, unsigned int );
void abort    ( void );

// io.c
void    outb    ( uint16_t, uint8_t  );
uint8_t inb     ( uint16_t );
void    io_wait ( );

// multiboot.c
void getbootinfo (void *, unsigned int );

// printk.c
char * itoa   ( int    );
int  strlen   ( char * );
void kprintf  ( char *, ... );
void clear    ( void );

// vga.c
void putchar  ( char   );
void vgainit  ( void   );
void status   ( char * );

#endif //__GLOBALS__
