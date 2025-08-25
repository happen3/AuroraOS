#define uint16_t short

volatile uint16_t* video = (volatile uint16_t*)0xB8000;

void kmain(void) {
	video[0] = (0x07 << 8) | 'H';
	while (1) {}
}
