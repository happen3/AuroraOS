#pragma once
#include "stdtypes.h"
#define EXTRACT_BITS(val,start,cnt) (((val) >> (start)) & ((1U << (cnt)) - 1))

uint8_t byte_to_bit(uint8_t byte, uint8_t idx);
char *byte_to_binary(uint8_t byte);
char *int_to_binary(uint32_t integer);
char *int_to_hex(uint32_t integer);
char *int_to_str(uint32_t integer);
void copybuf(uint8_t *buf, uint8_t *newbuf, uint64_t size);
uint8_t set_bit_in_byte(uint8_t state, uint8_t idx, uint8_t byte);