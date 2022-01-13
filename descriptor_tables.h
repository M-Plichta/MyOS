#include "globals.h"

// Access byte flags    
// bits<7-4> of the access byte
#define SEG_PRES(x)      ((x) << 0x07)          // Present
#define SEG_PRIV(x)     (((x) &  0x03) << 0x05) // Set privilege level (0 - 3)
#define SEG_DESCTYPE(x)  ((x) << 0x04)          // Descriptor type (0 for system, 1 for code/data)

// Access byte characteristics
// bits<3-0>, the remaining bits of the access byte
#define SEG_DATA_RD        0x00 // Read-Only
#define SEG_DATA_RDA       0x01 // Read-Only, accessed
#define SEG_DATA_RDWR      0x02 // Read/Write
#define SEG_DATA_RDWRA     0x03 // Read/Write, accessed
#define SEG_DATA_RDEXPD    0x04 // Read-Only, expand-down
#define SEG_DATA_RDEXPDA   0x05 // Read-Only, expand-down, accessed
#define SEG_DATA_RDWREXPD  0x06 // Read/Write, expand-down
#define SEG_DATA_RDWREXPDA 0x07 // Read/Write, expand-down, accessed
#define SEG_CODE_EX        0x08 // Execute-Only
#define SEG_CODE_EXA       0x09 // Execute-Only, accessed
#define SEG_CODE_EXRD      0x0A // Execute/Read
#define SEG_CODE_EXRDA     0x0B // Execute/Read, accessed
#define SEG_CODE_EXC       0x0C // Execute-Only, conforming
#define SEG_CODE_EXCA      0x0D // Execute-Only, conforming, accessed
#define SEG_CODE_EXRDC     0x0E // Execute/Read, conforming
#define SEG_CODE_EXRDCA    0x0F // Execute/Read, conforming, accessed

// Descriptor flags
#define SEG_GRAN(x)      ((x) << 0x0F) // Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)
#define SEG_SIZE(x)      ((x) << 0x0E) // Size (0 for 16-bit, 1 for 32)
#define SEG_LONG(x)      ((x) << 0x0D) // Long mode
#define SEG_SAVL(x)      ((x) << 0x0C) // Available for system use

// Limit
#define LIMIT 0xF00

#define GDT_CODE_PL0 SEG_GRAN(1) | SEG_SIZE(1) | SEG_LONG(0)      | SEG_SAVL(0) |\
                     SEG_PRES(1) | SEG_PRIV(0) | SEG_DESCTYPE(1)  |              \
                     LIMIT       | SEG_CODE_EXRD

#define GDT_DATA_PL0 SEG_GRAN(1) | SEG_SIZE(1) | SEG_LONG(0)     | SEG_SAVL(0) |\
                     SEG_PRES(1) | SEG_PRIV(0) | SEG_DESCTYPE(1) |              \
                     LIMIT       | SEG_DATA_RDWR

#define GDT_CODE_PL3 SEG_GRAN(1) | SEG_SIZE(1) | SEG_LONG(0)     | SEG_SAVL(0) |\
                     SEG_PRES(1) | SEG_PRIV(3) | SEG_DESCTYPE(1) |              \
                     LIMIT       | SEG_CODE_EXRD

#define GDT_DATA_PL3 SEG_GRAN(1) | SEG_SIZE(1) | SEG_LONG(0)     | SEG_SAVL(0) |\
                     SEG_PRES(1) | SEG_PRIV(3) | SEG_DESCTYPE(1) |              \
                     LIMIT       | SEG_DATA_RDWR

// This structure contains the value of one GDT entry.
struct gdt_entry_struct
{
   uint16_t limit_low;           // The lower 16 bits of the limit.
   uint16_t base_low;            // The lower 16 bits of the base.
   uint8_t  base_middle;         // The next 8 bits of the base.
   uint8_t  access;              // Access flags, determine what ring this segment can be used in.
   uint8_t  flags_lim;
   uint8_t  base_high;           // The last 8 bits of the base.
} __attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;

// Special pointer used to point to the GDT.
// This is what will be later loaded using lgdt
struct gdt_ptr_struct
{
   uint16_t limit;               // The upper 16 bits of all selector limits.
   uint32_t base;                // The address of the first gdt_entry_t struct.
}
 __attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;


/**
 * @brief Interrupt Descriptor table section
 * 
 * Following section is used to setting up and filling the IDT.
 * Each define constant represents a section of the Gate Descriptor.
 */
#define GATE_PRES(x)    ((x) << 7)        // Present bit
#define GATE_DPL(x)   (((x) & 0x3) << 6)  // CPU Privilate level
#define GATE_ZERO       ((0) << 4)        // Always 0

// Gate types:
#define GATE_TYPE_TASK  (0x5)   // Task Gate (Offset value is used and shoudl be 0)
#define GATE_TYPE_IN16  (0x6)   // 16-bit Interrupt Gate
#define GATE_TYPE_TR16  (0x7)   // 16-bit Trap Gate
#define GATE_TYPE_IN32  (0xE)   // 32-bit Interrupt
#define GATE_TYPE_TR32  (0xF)   // 32-bit Trap Gate

#define IDT_INT32_PL0 GATE_PRES(1) | GATE_DPL(0) | GATE_ZERO | GATE_TYPE_IN32

// A struct contains the value of on IDT entry.
struct idt_entry_struct
{
   uint16_t base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
   uint16_t sel;                 // Kernel segment selector.
   uint8_t  always0;             // This must always be zero.
   uint8_t  flags;               // More flags. See documentation.
   uint16_t base_hi;             // The upper 16 bits of the address to jump to.
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
struct idt_ptr_struct
{
   uint16_t limit;
   uint32_t base;                // The address of the first element in our idt_entry_t array.
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

// The interrupt handler is found in isr.c
// Stubs for the 32 CPU-dedicated interrupts.
extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

// Stubs for the 15 8259 PIC IQRs 
extern void irq0 ();
extern void irq1 ();
extern void irq2 ();
extern void irq3 ();
extern void irq4 ();
extern void irq5 ();
extern void irq6 ();
extern void irq7 ();
extern void irq8 ();
extern void irq9 ();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

// Programmable Interrupt Controller Constants:

#define PIC1         0x20
#define PIC2         0xA0
#define PIC1_COMMAND PIC1        // Master PIC command port
#define PIC1_DATA   (PIC1 + 1)   // Master PIC data port
#define PIC2_COMMAND PIC2        // Slave  PIC command port
#define PIC2_DATA   (PIC2 + 1)   // Slave  PIC data port


// Initialization Command Words
#define ICW1_ICW4       0x01
#define ICW1_SINGLE     0x02  // Single (Cascade) mode
#define ICW1_INTERVAL4  0x04  // Call address interval 4
#define ICW1_LEVEL      0x08  // Level triggered mode
#define ICW1_INIT       0x10  // Initialization

#define ICW4_8086       0x01  // 8086 mode
#define ICW4_AUTO       0x02  // Auto EOI
#define ICW4_BUF_SLAVE  0x08  // Buffered mode - Slave
#define ICW4_BUF_MASTER 0x0C  // Buffered mode - Master
#define ICW4_SFNM       0x10  // Special fully nested