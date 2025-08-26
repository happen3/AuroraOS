#include "stdtypes.h"
#pragma once

#define COM1 0x3F8

void outb(uint16_t port, uint8_t data);
uint8_t inb(uint16_t port);
void io_wait();
void serial_write(const char* str);
void serial_write_char(char c);
int serial_is_transmit_ready();
void com1_init(void);