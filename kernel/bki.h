#pragma once
#include <stdint.h>
typedef struct {
    uint16_t lower_memory_size;
    uint16_t upper_memory_size;
    uint8_t disk;
} boot_info;

boot_info *get_boot_info();