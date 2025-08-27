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