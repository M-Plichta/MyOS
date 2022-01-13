#include "globals.h"
#include "isr.h"
#include "descriptor_tables.h"

extern void gdt_flush(uint32_t);
extern void idt_flush(uint32_t);

// Internal function prototypes.
void init_gdt();
void init_idt();

void create_gdt_descriptor( int32_t, uint32_t, uint32_t, uint16_t );
void create_idt_descriptor( uint8_t, uint32_t, uint16_t, uint8_t  );

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;
idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;

extern isr_t interrupt_handlers[];

void setup_interrupts() {
   init_gdt();
   init_idt();

   memset(&interrupt_handlers, 0, sizeof(isr_t)*256);
}

/**
 * @brief Create a GDT entry
 * 
 * @param index Index of the GDT entry.
 * @param base  Linear addres where the segment begins
 * @param limit Maximum addressable unit
 * @param flag  Bits 48-55 of Descriptor (Flags + High Limit)
 */
void
create_gdt_descriptor(int32_t index, uint32_t base, uint32_t limit, uint16_t flag) {

   gdt_entries[index].limit_low   = (limit & 0xFFFF);
   gdt_entries[index].flags_lim   = (flag >> 8);

   gdt_entries[index].access      = (flag & 0xFF);

   gdt_entries[index].base_low    = (base & 0xFFFF);
   gdt_entries[index].base_middle = (base >> 16) & 0xFF;
   gdt_entries[index].base_high   = (base >> 24) & 0xFF;
}

// Function which initializes the GDT.
void
init_gdt() {
   gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
   gdt_ptr.base  = (uint32_t)&gdt_entries;

   /**
    *  Currently there are 5 Entries in the GDT,
    *    1. NULL
    *    2. Kernel privilege code segement
    *    3. Kernel privilege data segement
    *    4. User privilege code segement
    *    5. User privilege data segement
    */
   create_gdt_descriptor(0, 0, 0, 0);
   create_gdt_descriptor(1, 0, 0x000FFFFF, (GDT_CODE_PL0));
   create_gdt_descriptor(2, 0, 0x000FFFFF, (GDT_DATA_PL0));
   create_gdt_descriptor(3, 0, 0x000FFFFF, (GDT_CODE_PL3));
   create_gdt_descriptor(4, 0, 0x000FFFFF, (GDT_DATA_PL3));

   // Calls external function, for implementation, see GDTLoader.asm
   gdt_flush((uint32_t)&gdt_ptr);
}

/**
 * @brief Create a IDT entry
 * 
 * @param num   Index of the IDT entry
 * @param base  Address of entry point to the ISR
 * @param sel   Pointer to a valid code segment in the GDT
 * @param flags Various flags (Present bit, Privilege Level and Gate Type)
 */
void
create_idt_descriptor(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
   idt_entries[num].base_lo = base & 0xFFFF;
   idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

   idt_entries[num].sel     = sel;
   idt_entries[num].always0 = 0;

   // Remember to change the privilage level to 3 when using user-mode.
   idt_entries[num].flags   = flags;
}

// Sets up IDT entries for all of the 32 CPU-Dedicated Interrupts.
void
init_idt() {
   idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
   idt_ptr.base  = (uint32_t)&idt_entries;

   memset(&idt_entries, 0, sizeof(idt_entry_t)*256);
   PIC_remap(0x20, 0x28);

   // ISRs
   create_idt_descriptor( 0, (uint32_t)isr0 , 0x08, IDT_INT32_PL0);
   create_idt_descriptor( 1, (uint32_t)isr1 , 0x08, IDT_INT32_PL0);
   create_idt_descriptor( 2, (uint32_t)isr2 , 0x08, IDT_INT32_PL0);
   create_idt_descriptor( 3, (uint32_t)isr3 , 0x08, IDT_INT32_PL0);
   create_idt_descriptor( 4, (uint32_t)isr4 , 0x08, IDT_INT32_PL0);
   create_idt_descriptor( 5, (uint32_t)isr5 , 0x08, IDT_INT32_PL0);
   create_idt_descriptor( 6, (uint32_t)isr6 , 0x08, IDT_INT32_PL0);
   create_idt_descriptor( 7, (uint32_t)isr7 , 0x08, IDT_INT32_PL0);
   create_idt_descriptor( 8, (uint32_t)isr8 , 0x08, IDT_INT32_PL0);
   create_idt_descriptor( 9, (uint32_t)isr9 , 0x08, IDT_INT32_PL0);
   create_idt_descriptor(10, (uint32_t)isr10, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(11, (uint32_t)isr11, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(12, (uint32_t)isr12, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(13, (uint32_t)isr13, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(14, (uint32_t)isr14, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(15, (uint32_t)isr15, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(16, (uint32_t)isr16, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(17, (uint32_t)isr17, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(18, (uint32_t)isr18, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(19, (uint32_t)isr19, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(20, (uint32_t)isr20, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(21, (uint32_t)isr21, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(22, (uint32_t)isr22, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(23, (uint32_t)isr23, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(24, (uint32_t)isr24, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(25, (uint32_t)isr25, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(26, (uint32_t)isr26, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(27, (uint32_t)isr27, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(28, (uint32_t)isr28, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(29, (uint32_t)isr29, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(30, (uint32_t)isr30, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(31, (uint32_t)isr31, 0x08, IDT_INT32_PL0);

   // IRQs
   create_idt_descriptor(32, (uint32_t)irq0 , 0x08, IDT_INT32_PL0);
   create_idt_descriptor(33, (uint32_t)irq1 , 0x08, IDT_INT32_PL0);   
   create_idt_descriptor(34, (uint32_t)irq2 , 0x08, IDT_INT32_PL0);
   create_idt_descriptor(35, (uint32_t)irq3 , 0x08, IDT_INT32_PL0);
   create_idt_descriptor(36, (uint32_t)irq4 , 0x08, IDT_INT32_PL0);
   create_idt_descriptor(37, (uint32_t)irq5 , 0x08, IDT_INT32_PL0);
   create_idt_descriptor(38, (uint32_t)irq6 , 0x08, IDT_INT32_PL0);
   create_idt_descriptor(39, (uint32_t)irq7 , 0x08, IDT_INT32_PL0);
   create_idt_descriptor(40, (uint32_t)irq8 , 0x08, IDT_INT32_PL0);
   create_idt_descriptor(41, (uint32_t)irq9 , 0x08, IDT_INT32_PL0);
   create_idt_descriptor(42, (uint32_t)irq10, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(43, (uint32_t)irq11, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(44, (uint32_t)irq12, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(45, (uint32_t)irq13, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(46, (uint32_t)irq14, 0x08, IDT_INT32_PL0);
   create_idt_descriptor(47, (uint32_t)irq15, 0x08, IDT_INT32_PL0);

   idt_flush((uint32_t)&idt_ptr);
}

void
PIC_remap(uint8_t offset1, uint8_t offset2) {
   uint8_t mask1, mask2;

   mask1 = inb(PIC1_DATA);
   mask2 = inb(PIC2_DATA);

   outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);  // Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, offset2);  // Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 0b100);    // Tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 0b10);     // Tell Slave PIC its cascade identity (0000 0010)
	io_wait();
 
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
 
	outb(PIC1_DATA, mask1);   // restore saved masks.
	outb(PIC2_DATA, mask2);
}