#include "stdtypes.h"
#include "kstring.h"
#include "idt.h"
#include "io.h"
#include "pic.h"

#define TEXT_MODE_MAX_COLUMNS 80
#define TEXT_MODE_MAX_ROWS 25
volatile uint16_t* video = (volatile uint16_t*)0xB8000;
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

void kputc(const char chr, int col, int row, uint8_t attr) {
	int vramaddr = (row * TEXT_MODE_MAX_COLUMNS) + col;
	video[vramaddr] = (attr << 8) | chr;
}

void kputs(const char* str, int col, int row) {
	size_t str_length = strlen(str);
	for (int i = 0; i < str_length; i++) {
		kputc(str[i], col + i, row, 0x0E);
	}
}

void kputs_err(const char* str) {
	size_t str_length = strlen(str);
	for (int i = 0; i < str_length; i++) {
		kputc(str[i], 0 + i, 22, 0x04);
	}
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

void kmain() {
	kputs("AuroraOS Kernel", 0, 0);
	setup_idt();
	load_idt();
	com1_init();
	serial_write("[kernel] early init complete\n");
	init_pic(0x30, 0x38);
	io_wait();
	sti();
	reset_mask_pic1(0b11111110); // Allow IRQ 1 at INT 0x30
	while (1) {}
}
