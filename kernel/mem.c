#include "io.h"
#include "stdtypes.h"
#include "mem.h"

static uint8_t *heap_ptr = (uint8_t*)KERNEL_HEAP_START;
static uint8_t *heap_end = (uint8_t*)(KERNEL_HEAP_START + KERNEL_HEAP_SIZE);
page_t pages[TOTAL_PAGES];

void pages_init(void* memory_start) {
    for (int i = 0; i < TOTAL_PAGES; i++) {
        pages[i].status = 1;
        pages[i].ptr = (uint8_t*)memory_start + i * PAGE_SIZE;
    }
    serial_write("[aurpag] initialization finished\n");
}

void* kmalloc() {
    int free_pages_c = 0;
    int free_pages_indexes[TOTAL_PAGES];
    for (int i = 0; i < TOTAL_PAGES; i++) {
        if (pages[i].status == 1) {
            free_pages_indexes[free_pages_c] = i;
            free_pages_c++;
        }
    }
    if (free_pages_c == 0) {
        // Out of memory
        serial_write("[aurpag] no kernel pages left\n");
        return (nullptr_t)0;
    }
    void* ptr = pages[free_pages_indexes[0]].ptr;
    pages[free_pages_indexes[0]].status = 0;
    
    serial_write("[aurpag] allocated new page\n");
    return ptr;
}

void kfree(void* ptr) {
    for (int i = 0; i < TOTAL_PAGES; i++) {
        if (pages[i].ptr == ptr) {
            pages[i].status = 1;
            serial_write("[aurpag] freed page\n");
            break;
        }
    }
}