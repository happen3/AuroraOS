#pragma once
#include "stdtypes.h"

#define KERNEL_HEAP_START 0x9FD00
#define KERNEL_HEAP_SIZE  0x10000
#define PAGE_SIZE         4096
#define TOTAL_PAGES       16


typedef struct {
    uint8_t status; // 1 = FREE, 0 = ALLOCATED
    void* ptr; // start
} page_t;

void* kmalloc();
void pages_init(void* memory_start);
void kfree(void* ptr);

extern page_t pages[16];