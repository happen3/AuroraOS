#include "idt.h"
#include "stdtypes.h"

struct InterruptDescriptor32 idt[256];
struct IDTPointer idt_ptr;

void write_idt_entry(int n, uint32_t handler) {
    idt[n].offset_1 = handler & 0xFFFF;
    idt[n].offset_2 = (handler >> 16) & 0xFFFF;
    idt[n].selector = 0x08;
    idt[n].type_attributes = 0x8E;
    idt[n].zero = 0;
}

extern void default_isr_handler();
extern void protection_error_handler();
extern void invalid_op_handler();
extern void divide_by_zero_handler();
extern void double_fault_handler();
extern void overflow_handler();

void setup_idt() {
    for (int i = 0; i != 256; i++) {
        write_idt_entry(i, (uint32_t)default_isr_handler);
    }
    write_idt_entry(0x00, (uint32_t)divide_by_zero_handler);
    write_idt_entry(0x02, (uint32_t)overflow_handler);
    write_idt_entry(0x06, (uint32_t)invalid_op_handler);
    write_idt_entry(0x08, (uint32_t)double_fault_handler);
    write_idt_entry(0x0D, (uint32_t)protection_error_handler);
}

void load_idt() {
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base  = (uint32_t)&idt;
    asm volatile("lidtl (%0)" : : "r"(&idt_ptr));
}
