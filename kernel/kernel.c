#include "stdtypes.h"
#include "kstring.h"
#include "idt.h"
#include "io.h"
#include "mem.h"
#include "pic.h"
#include "util.h"
#include "vga_txt.h"
static volatile uint32_t ticks = 0;

void kmain();

// TODO: look into using startup scripts
void _start(void) {
	kmain();
}

void timer_tick() {
	if (ticks > 18) {
		ticks = 0;
	}
	ticks++;
}

void sti() {
	asm volatile("sti");
}

void do_init() {
	setup_idt();
	load_idt();
	com1_init();
	serial_write("[kernel] early init complete\n");
	init_pic(0x30, 0x38);
	io_wait();
	sti();
	reset_mask_pic1(0b11111110); // Allow IRQ 1 at INT 0x30
	pages_init((uint32_t*)KERNEL_HEAP_START);
	kmalloc(); // register a page for exceptions
}

void kmain() {
	do_init();
	
	clear_screen();
	kputs("AuroraOS Kernel", 0, 0, 0x0A);
	while (1) {}
}
