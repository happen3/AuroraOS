#include "stdtypes.h"

char *byte_to_binary(uint8_t byte) {
	static char buf[9];
	for (int i = 0; i < 8; i++) {
		buf[i] = (byte & (1 << (7 - i)) ? '1' : '0');
	}
	buf[8] = '\0';
	return buf;
}

char *int_to_binary(uint32_t integer) {
	static char buf[33];
	for (int i = 0; i < 32; i++) {
		buf[i] = (integer & (1 << (31 - i)) ? '1' : '0');
	}
	buf[32] = '\0';
	return buf;
}

char *int_to_hex(uint32_t integer) {
    static char buf[5];
    const char hexchars[] = "0123456789ABCDEF";
    uint8_t nibble;
    uint8_t index = 0;
    buf[4] = '\0';
    for (int i = 7; i >= 0; i--) {
        nibble = (integer >> (i * 4)) & 0xF;
        buf[index] = hexchars[nibble];
        index++;
    }

    return buf;
}

char *int_to_str(uint32_t integer) {
    static char buf[11];
    buf[10] = '\0'; 
    const char chars[] = "0123456789";
    int i = 9;
    if (integer == 0) {
        buf[9] = '0';
        return &buf[9];
    }
    while (integer > 0 && i >= 0) {
        buf[i] = '0' + (integer % 10);
        integer /= 10;
        i--;
    }
    return &buf[i+1];
}

uint8_t byte_to_bit(uint8_t byte, uint8_t idx) {
    return byte >> idx & 1;
}

void copybuf(uint8_t *buf, uint8_t *newbuf, uint64_t size) {
    for (uint64_t i = 0; i <= size; i++) {
        newbuf[i] = buf[i];
    }
}

uint8_t set_bit_in_byte(uint8_t state, uint8_t idx, uint8_t byte) {
    return (byte & ~(1 << idx)) | ((state & 1) << idx);
}