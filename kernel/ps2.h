#pragma once
#include "stdtypes.h"
#define PS2_DATA_PORT 0x60
#define PS2_COMMAND_STATUS_PORT 0x64

typedef struct {
    uint8_t outbuf_status;
    uint8_t inbuf_status;
    uint8_t sysflag;
    uint8_t command_or_data;
    uint8_t error;
} PS2_STATUS_BYTE;

typedef struct {
    uint8_t ps2_port1_int;
    uint8_t ps2_port2_int;
    uint8_t system_flag;
    uint8_t clock_port1;
    uint8_t clock_port2;
    uint8_t translation_port1;
} PS2_CONF_DATA;

typedef struct {
    uint8_t system_reset; // WARNING!! Always pulse this line - setting it to 0 may cause machine lock up.
    uint8_t gate_a20;
    uint8_t port_2_clock;
    uint8_t port_2_output;
    uint8_t outputbuffer_full_port1;
    uint8_t outputbuffer_full_port2;
    uint8_t port_1_clock;
    uint8_t port_1_output;
} PS2_CONTROLLER_OUTPORT;

typedef enum {
    READ_CONFIGURATION_BYTE = 0x20,
    SET_CONFIGURATION_BYTE = 0x60,
    DISABLE_PS2_PORT_2 = 0xA7,
    ENABLE_PS2_PORT_2,
    TEST_PS2_PORT_2,
    TEST_PS2_CONTROLLER = 0xAA,
    TEST_PS2_PORT_PRIMARY,
    UNDOCUMENTED_DIAGNOSTIC_DUMP,
    DISABLE_PS2_PORT_PRIMARY,
    ENABLE_PS2_PORT_PRIMARY,
    READ_CONTROLLER_INPUT_PORT = 0xC0,
    READ_CONTROLLER_OUTPUT_PORT= 0xD0,
    WRITE_TO_CONTROLLER_OUTPUT_PORT,
    WRITE_TO_PS2_PORT_PRIMARY_OUTBUF,
    WRITE_TO_PS2_PORT_2_OUTBUF,
    WRITE_TO_PS2_PORT_2_INBUF,
    CPU_RESET = 0xF0,
    PS2_RESET = 0xFF

} PS2_COMMANDS;

typedef struct {
    uint8_t response[8];
    struct {
        uint8_t tick_of_creation;
        uint8_t command;
    } packet_metadata;
} PS2_RESPONSE;

// functions
PS2_STATUS_BYTE ps2_get_status_byte();
void ps2_wait_for_inbuf(uint32_t timeout_in_ticks);
void ps2_wait_for_outbuf(uint32_t timeout_in_ticks);
void ps2_send_command_discard(uint8_t command);
PS2_RESPONSE ps2_send_command_wait_for_result(uint8_t command);
PS2_CONF_DATA ps2_get_conf_byte();
uint8_t ps2_encode_config_byte(PS2_CONF_DATA conf_data);