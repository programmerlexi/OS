#include "memory.h"

void memset(void* dst, uint64_t val, size_t count) {
    if (count <= 8) {
        uint8_t* val_ptr = (uint8_t*)&val;
        for (uint8_t* ptr = (uint8_t*)dst; ptr < (uint8_t*)((size_t)dst + count); ptr++) {
            *ptr = *val_ptr;
            val_ptr++;
        }
        return;
    }

    uint64_t proceding_bytes = count % 8;
    uint64_t newcount = count - proceding_bytes;

    for (uint64_t* ptr = (uint64_t*)dst; ptr < (uint64_t*)((size_t)dst + newcount); ptr++) {
        *ptr = val;
    }
    uint8_t* valPtr = (uint8_t*)&val;
    for (uint8_t* ptr = (uint8_t*)dst + newcount; ptr < (uint8_t*)((size_t)dst + count); ptr++) {
        *ptr = *valPtr;
        valPtr++;
    }
}

void memcpy(void* dst, void* src, size_t count) {
    if (count <= 8) {
        uint8_t* src_ptr = (uint8_t*)src;
        uint8_t* dst_ptr = (uint8_t*)dst;
        for (uint8_t* ptr = dst_ptr; ptr < (uint8_t*)((size_t)dst + count); ptr++) {
            *ptr = *src_ptr;
            src_ptr++;
        }
        return;
    }
    uint64_t proceding_bytes = count % 8;
    uint64_t newcount = count - proceding_bytes;
    uint64_t* src_ptr = (uint64_t*)src;

    for (uint64_t* ptr = (uint64_t*)dst; ptr < (uint64_t*)((size_t)dst + newcount); ptr++) {
        *ptr = *src_ptr;
        src_ptr++;
    }

    uint8_t* src_ptr_8 = (uint8_t*)src_ptr;
    for (uint8_t* ptr = (uint8_t*)dst + newcount; ptr < (uint8_t*)((size_t)dst + count); ptr++) {
        *ptr = *src_ptr_8;
        src_ptr_8++;
    }
}
uint16_t get_lower_memory() {
    uint16_t* lower_ptr = (uint16_t*)0x5000;
    return *lower_ptr;
}
uint32_t get_upper_memory() {
    uint16_t* upper_ptr = (uint16_t*)0x5002;
    return *upper_ptr * 64;
}
uint32_t get_total_memory() {
    uint32_t total = 0;
    uint16_t lower = get_lower_memory();
    uint32_t upper = get_upper_memory();
    total = ((uint32_t)lower) + upper;
    return total;
}