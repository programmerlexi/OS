#pragma once
#include <stddef.h>
#include <stdint.h>
typedef struct sysmem_segment {
    size_t size;
    sysmem_segment* next;
    sysmem_segment* prev;
} sysmem_t;
void init_sysmem(uint64_t start, size_t size);