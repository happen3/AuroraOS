#include "stdtypes.h"
#include "kstring.h"
#include "idt.h"

#define TEXT_MODE_MAX_COLUMNS 80
#define TEXT_MODE_MAX_ROWS 25
volatile uint16_t* video = (volatile uint16_t*)0xB8000;

// TODO: look into using startup scripts
void _start(void) {
	kmain();
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

void kmain() {
	kputs("AuroraOS Kernel", 0, 0);
	setup_idt();
	load_idt();
	asm volatile("int $0x00");
	while (1) {}
}
