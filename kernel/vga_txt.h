#pragma once
#include "stdtypes.h"
#define TEXT_MODE_MAX_COLUMNS 80
#define TEXT_MODE_MAX_ROWS 25

void clear_screen();
void kputc(const char chr, int col, int row, uint8_t attr);

void kputs(const char* str, int col, int row, uint8_t attr);

void kputs_err(const char* str);
void set_cursor_pos(int x, int y);
void print_register(char reg[6], char* val, int col, int row, uint8_t unspecified);