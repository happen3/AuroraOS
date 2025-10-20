#include "io.h"
#include "stdtypes.h"
#include "util.h"
#include "ps2.h"
extern uint32_t volatile ticks;

PS2_STATUS_BYTE ps2_get_status_byte() {
    PS2_STATUS_BYTE psb;
    uint8_t sb = inb(PS2_COMMAND_STATUS_PORT);
    psb.outbuf_status = byte_to_bit(sb, 0);
    psb.inbuf_status = byte_to_bit(sb, 1);
    psb.sysflag = byte_to_bit(sb, 2);
    psb.command_or_data = byte_to_bit(sb, 3);
    
    // Match error types
    if (byte_to_bit(sb, 6)) psb.error = 1; // Time-out
    else if (byte_to_bit(sb, 7)) psb.error = 2; // Parity
    else psb.error = 0; // No error
    return psb; 
}

PS2_RESPONSE ps2_create_response_packet(uint8_t response[8], uint8_t command) {
    PS2_RESPONSE psr;
    copybuf(response, psr.response, 8);
    psr.packet_metadata.command = command;
    psr.packet_metadata.tick_of_creation = ticks;
    return psr;
}

void ps2_wait_for_inbuf(uint32_t timeout_in_ticks) {
    if (timeout_in_ticks == 0) {
        uint8_t wait_ticks = 2;
    } else {
        uint8_t wait_ticks = timeout_in_ticks;
    }

    PS2_STATUS_BYTE status_byte;
    uint8_t target_ticks = (ticks + timeout_in_ticks) % 18;
    while (1) {
        PS2_STATUS_BYTE psb = ps2_get_status_byte();

        if (ticks >= target_ticks) break;
        if (psb.inbuf_status) break;
    }
}

void ps2_wait_for_outbuf(uint32_t timeout_in_ticks) {
    if (timeout_in_ticks == 0) {
        uint8_t wait_ticks = 2;
    } else {
        uint8_t wait_ticks = timeout_in_ticks;
    }

    PS2_STATUS_BYTE status_byte;
    uint8_t target_ticks = (ticks + timeout_in_ticks) % 18;
    while (1) {
        PS2_STATUS_BYTE psb = ps2_get_status_byte();

        if (ticks >= target_ticks) break;
        if (psb.outbuf_status) break;
    }
}


void ps2_send_command_discard(uint8_t command) {
    outb(PS2_COMMAND_STATUS_PORT, command);
}

PS2_RESPONSE ps2_send_command_wait_for_result(uint8_t command) {
    outb(PS2_COMMAND_STATUS_PORT, command);
    ps2_wait_for_outbuf(0);
    uint8_t response[8];
    response[0] = inb(PS2_DATA_PORT);
    PS2_RESPONSE psr = ps2_create_response_packet(response, command);
    return psr;
}

PS2_CONF_DATA ps2_get_conf_byte() {
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

uint8_t ps2_encode_config_byte(PS2_CONF_DATA conf_data) {
    uint8_t cb_raw = 0;
    cb_raw = set_bit_in_byte(conf_data.ps2_port1_int, 0, cb_raw);
    cb_raw = set_bit_in_byte(conf_data.ps2_port2_int, 1, cb_raw);
    cb_raw = set_bit_in_byte(conf_data.system_flag, 2, cb_raw);
    cb_raw = set_bit_in_byte(0, 3, cb_raw);
    cb_raw = set_bit_in_byte(conf_data.clock_port1, 4, cb_raw);
    cb_raw = set_bit_in_byte(conf_data.clock_port2, 5, cb_raw);
    cb_raw = set_bit_in_byte(conf_data.translation_port1, 6, cb_raw);
    cb_raw = set_bit_in_byte(0, 7, cb_raw);
    return cb_raw;
}