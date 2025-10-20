#pragma once
#include "stdtypes.h"
#include "ps2.h"
extern volatile uint32_t ticks;

PS2_CONF_DATA kbd_get_conf_byte();
void kbd_begin_initialization();