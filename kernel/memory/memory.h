#pragma once
#include <stddef.h>
#include <stdint.h>

void memset(void* dst, uint64_t val, size_t size);
void memcpy(void* dst, void* src, size_t size);
uint16_t get_lower_memory();
uint32_t get_upper_memory();
uint32_t get_total_memory();