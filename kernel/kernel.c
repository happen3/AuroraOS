#define uint16_t short
#define TEXT_MODE_MAX_COLUMNS 80
#define TEXT_MODE_MAX_ROWS 25

volatile uint16_t* video = (volatile uint16_t*)0xB8000;

void kputc(const char chr, int col, int row) {
	int vramaddr = (row * TEXT_MODE_MAX_COLUMNS) + col;
	video[vramaddr] = (0x0E << 8) | chr;
}

void kmain(void) {
	kputc('P', 0, 0);
	while (1) {}
}
