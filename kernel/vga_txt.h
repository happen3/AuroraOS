#pragma once
#include "stdtypes.h"
#define TEXT_MODE_MAX_COLUMNS 80
#define TEXT_MODE_MAX_ROWS 25

void clear_screen();
void kputc(const char chr, int col, int row, uint8_t attr);

void kputs(const char* str, int col, int row, uint8_t attr);

void kputs_err(const char* str);