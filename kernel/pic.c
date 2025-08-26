#include "stdtypes.h"
#include "pic.h"
#include "io.h"

void init_pic(uint8_t vector_start_1, uint8_t vector_start_2) {
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, vector_start_1);                 
	io_wait();
	outb(PIC2_DATA, vector_start_2);                
	io_wait();
	outb(PIC1_DATA, 1 << CASCADE_IRQ);       
	io_wait();
	outb(PIC2_DATA, 2);                      
	io_wait();
	
	outb(PIC1_DATA, ICW4_8086);       
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	// Unmask both PICs.
	outb(PIC1_DATA, 0);
	outb(PIC2_DATA, 0);
}

// sets a new mask for the PIC.
void reset_mask_pic1(uint8_t mask) {
	outb(PIC1_DATA, mask);
}
