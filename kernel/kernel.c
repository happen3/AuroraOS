#include "stdtypes.h"
#include "kstring.h"
#include "idt.h"
#include "io.h"
#include "mem.h"
#include "pic.h"
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

char *byte_to_binary(uint8_t byte) {
	static char buf[9];
	for (int i = 0; i < 8; i++) {
		buf[i] = (byte & (1 << (7 - i)) ? '1' : '0');
	}
	buf[8] = '\0';
	return buf;
}

void sti() {
	asm volatile("sti");
}

char *int_to_binary(uint32_t integer) {
	static char buf[33];
	for (int i = 0; i < 32; i++) {
		buf[i] = (integer & (1 << (31 - i)) ? '1' : '0');
	}
	buf[32] = '\0';
	return buf;
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
}

void kmain() {
	do_init();

	clear_screen();
	kputs("AuroraOS Kernel", 0, 0, 0x0A);
	while (1) {}
}
