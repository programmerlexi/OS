#pragma once
#include <stddef.h>
#include <stdint.h>
typedef struct sysmem_segment {
    uint32_t size;
    sysmem_segment* next;
    sysmem_segment* prev;
} sysmem_t;
void init_sysmem(uint32_t start, uint32_t size);