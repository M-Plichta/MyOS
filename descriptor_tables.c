#include "globals.h"
#include "descriptor_tables.h"

extern void gdt_flush(uint32_t);
extern void idt_flush(uint32_t);

// Internal function prototypes.
void init_gdt();
void init_idt();

void create_gdt_descriptor(int32_t,uint32_t,uint32_t,uint16_t);
void create_idt_descriptor(uint8_t, uint32_t, uint16_t, uint8_t);

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;
idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;

void
create_gdt_descriptor(int32_t index, uint32_t base, uint32_t limit, uint16_t flag) {

   gdt_entries[index].limit_low   = (limit & 0xFFFF);
   gdt_entries[index].flags_lim   = (flag >> 8);

   gdt_entries[index].access      = (flag & 0xFF);

   gdt_entries[index].base_low    = (base & 0xFFFF);
   gdt_entries[index].base_middle = (base >> 16) & 0xFF;
   gdt_entries[index].base_high   = (base >> 24) & 0xFF;
}

void
init_gdt() {
   gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
   gdt_ptr.base  = (uint32_t)&gdt_entries;

   create_gdt_descriptor(0, 0, 0, 0);
   create_gdt_descriptor(1, 0, 0x000FFFFF, (GDT_CODE_PL0));
   create_gdt_descriptor(2, 0, 0x000FFFFF, (GDT_DATA_PL0));
   create_gdt_descriptor(3, 0, 0x000FFFFF, (GDT_CODE_PL3));
   create_gdt_descriptor(4, 0, 0x000FFFFF, (GDT_DATA_PL3));

   gdt_flush((uint32_t)&gdt_ptr);
}

void
init_idt() {
   idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
   idt_ptr.base  = (uint32_t)&idt_entries;

   memset(&idt_entries, 0, sizeof(idt_entry_t)*256);

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

   idt_flush((uint32_t)&idt_ptr);
}

void create_idt_descriptor(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
   idt_entries[num].base_lo = base & 0xFFFF;
   idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

   idt_entries[num].sel     = sel;
   idt_entries[num].always0 = 0;

   // Remember to change the privilage level to 3 when using user-mode.
   idt_entries[num].flags   = flags;

   // kprintf("IDT%2d = %04x|%02x|%02x|%04x|%04x\n", num,
   //                                      idt_entries[num].base_hi,
   //                                      idt_entries[num].flags,
   //                                      idt_entries[num].always0,
   //                                      idt_entries[num].sel,
   //                                      idt_entries[num].base_lo);
}