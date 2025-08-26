#include "vga_txt.h"
#include "io.h"
#include "stdtypes.h"
#include "kstring.h"
volatile uint16_t *video = (volatile uint16_t*)0xB8000;

void set_cursor_pos(int x, int y)
{
	uint16_t pos = y * TEXT_MODE_MAX_COLUMNS + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void kputc(const char chr, int col, int row, uint8_t attr) {
	int vramaddr = (row * TEXT_MODE_MAX_COLUMNS) + col;
	video[vramaddr] = (attr << 8) | chr;
}

void kputs(const char* str, int col, int row, uint8_t attr) {
	size_t str_length = strlen(str);
    int i = 0;
	for (; i < str_length; i++) {
		kputc(str[i], col + i, row, attr);
        if (col > TEXT_MODE_MAX_COLUMNS) {
            row++;
            col = 0;
        }
	}
    set_cursor_pos(col + i, row);
}

void kputs_err(const char* str) {
	size_t str_length = strlen(str);
	for (int i = 0; i < str_length; i++) {
		kputc(str[i], 0 + i, 22, 0x04);
	}
}

void clear_screen() {
    char buf[81];
    for (int i = 0; i < 80; i++) {
        buf[i] = ' ';
    }
    buf[80] = '\0';
    for (int line = 0; line < TEXT_MODE_MAX_COLUMNS; line++) {
        kputs(buf, 0, line, 0x0F);
    }
}