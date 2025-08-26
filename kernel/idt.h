#pragma once
#include "stdtypes.h"

// Source: https://wiki.osdev.org/Interrupt_Descriptor_Table#Example_Code
struct InterruptDescriptor32 {
   uint16_t offset_1;        // offset bits 0..15
   uint16_t selector;        // a code segment selector in GDT or LDT
   uint8_t  zero;            // unused, set to 0
   uint8_t  type_attributes; // gate type, dpl, and p fields
                             // task gate: 0x85, trap gate: 0x8F, interrupt gate: 0x8E
   uint16_t offset_2;        // offset bits 16..31
} __attribute__((packed));

struct IDTPointer {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void write_idt_entry(int n, uint32_t handler);
void setup_idt();
void load_idt();