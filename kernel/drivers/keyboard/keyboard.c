#include "stdtypes.h"
#include "io.h"
#include "ps2.h"
#include "keyboard.h"
#include "util.h"
#include "vga_txt.h"

// Thanks to: http://www.osdever.net/bkerndev/Docs/keyboard.htm
unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};		

PS2_CONF_DATA kbd_get_conf_byte() {
    outb(PS2_COMMAND_STATUS_PORT, READ_CONFIGURATION_BYTE);
    ps2_wait_for_inbuf(0);
    uint8_t cb = inb(PS2_DATA_PORT);
    PS2_CONF_DATA cdt;
    cdt.clock_port1 = byte_to_bit(cb, 4);
    cdt.clock_port2 = byte_to_bit(cb, 5);
    cdt.ps2_port1_int = byte_to_bit(cb, 0);
    cdt.ps2_port2_int = byte_to_bit(cb, 1);
    cdt.translation_port1 = byte_to_bit(cb, 6);
    cdt.system_flag = byte_to_bit(cb, 2);
    return cdt;
}

void kbd_begin_initialization() {
    // Disable ports
    ps2_send_command_wait_for_result(DISABLE_PS2_PORT_PRIMARY);
    ps2_send_command_wait_for_result(DISABLE_PS2_PORT_2);

    // Set Configuration Byte
    PS2_CONF_DATA confbyte = kbd_get_conf_byte();
    if (confbyte.ps2_port1_int) serial_write("IRQ1 is enabled");
    if (confbyte.ps2_port2_int) serial_write("IRQ12 is enabled");
    
    // Set Config Byte
    confbyte.clock_port1 = 1;
    uint8_t staged_cb = ps2_encode_config_byte(confbyte);

    // Commit to controller
    ps2_send_command_discard(SET_CONFIGURATION_BYTE);
    outb(PS2_DATA_PORT, staged_cb);
    
    io_wait();

    // Now test the controller and ports
    PS2_RESPONSE self_test_resp = ps2_send_command_wait_for_result(TEST_PS2_CONTROLLER);
    if (self_test_resp.response[0] != 0x55) {
        serial_write("[ps2/keyb] Controller self test is FAIL!\n");
        return;
    }

    // test the port 1
    self_test_resp = ps2_send_command_wait_for_result(TEST_PS2_PORT_PRIMARY);
    if (self_test_resp.response[0] != 0x00) {
        serial_write("[ps2/keyb] Port 1 test is FAIL!\n");
        return;
    }

    // Enable back
    ps2_send_command_wait_for_result(ENABLE_PS2_PORT_PRIMARY);
    ps2_send_command_wait_for_result(ENABLE_PS2_PORT_2);

    confbyte.ps2_port1_int = 1;
    staged_cb = ps2_encode_config_byte(confbyte);
    ps2_send_command_discard(SET_CONFIGURATION_BYTE);
    outb(PS2_DATA_PORT, staged_cb);

    // Reset
    self_test_resp = ps2_send_command_wait_for_result(PS2_RESET);
    serial_write("[ps2/keyb] Keyboard init finished successfully\n");

    // Clear output buffer
    for (int i = 0; i <= 8; i++) {
        inb(PS2_DATA_PORT);
    }
}

void ps2kbd_handler(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx) {
    while ((inb(PS2_COMMAND_STATUS_PORT) & 2) == 0 && ((ticks + 1) % 18) == ticks) {}
    uint8_t scancode = inb(0x60);
    kputc(kbdus[scancode], 0, 1, 0xE0);
}