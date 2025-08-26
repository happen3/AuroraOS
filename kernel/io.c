#include "io.h"
#include "stdtypes.h"

void outb(uint16_t port, uint8_t data) {
    asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void io_wait() {
    asm volatile ("inb $0x80, %%al" : : : "al");
}

void com1_init(void) {
    outb(COM1 + 1, 0x00);    // Disable interrupts
    outb(COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(COM1 + 0, 0x01);    // Divisor low (115200 / 115200 = 1)
    outb(COM1 + 1, 0x00);    // Divisor high
    outb(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7);    // Enable FIFO, clear them, 14-byte threshold
    outb(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int serial_is_transmit_ready() {
    return inb(COM1 + 5) & 0x20;
}

void serial_write_char(char c) {
    while (!serial_is_transmit_ready());
    outb(COM1, c);
}

void serial_write(const char* str) {
    while (*str) {
        if (*str == '\n') serial_write_char('\r');  // Convert LF → CRLF
        serial_write_char(*str++);
    }
}