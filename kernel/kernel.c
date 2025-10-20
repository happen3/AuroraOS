#include "kernel.h"
#include "drivers/keyboard/keyboard.h"
#include "ps2.h"
#include "stdtypes.h"
#include "kstring.h"
#include "idt.h"
#include "io.h"
#include "mem.h"
#include "pic.h"
#include "util.h"
#include "vga_txt.h"
volatile uint32_t ticks = 0;
static volatile uint8_t* kernel_data;
KERNEL_INFO kernel_extended_info;

void kmain();

// TODO: look into using startup scripts
void _start(void) {
	kmain();
}

void timer_tick() {
	if (ticks > 18) {
		ticks = 0;
	}
	kernel_data[0] = ticks + 1;
	ticks++;
}

void sti() {
	asm volatile("sti");
}

void do_init() {
	setup_idt();
	load_idt();
	clear_screen();
	com1_init();
	serial_write("[kernel] early init complete\n");
	init_pic(0x30, 0x38);
	io_wait();
	reset_mask_pic1(0b11111110); // Allow IRQ 1 at INT 0x30
	pages_init((uint32_t*)KERNEL_HEAP_START);
	kmalloc(); // register a page for exceptions
	kernel_data = (uint8_t*)kmalloc(); // reserve a page for kernel data
	
	serial_write("[aurpag] free pages: ");
	serial_write(int_to_str(getfree()));
	serial_write_char('\n');
}

void init_periph() {
	serial_write("[kernel] communicating with PS/2 keyboard\n");
	kbd_begin_initialization();
	reset_mask_pic2(0b11111111);
	reset_mask_pic1(0b11111100);
	sti();
	ps2_wait_for_inbuf(0);
	outb(PS2_DATA_PORT, 0xF4);  // Enable scanning
	serial_write("[kernel] Finished communication with keyboard\n");
}


void kmain() {
	do_init();
	init_periph();
	
	//clear_screen();
	kputs("AuroraOS Kernel", 0, 0, 0x0A);
	while (1) {
	}
}
