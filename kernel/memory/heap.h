#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct heap_segment_header {
    size_t size;
    heap_segment_header* next;
    heap_segment_header* prev;
    heap_segment_header* nextFree;
    heap_segment_header* prevFree;
    bool free;
    void combine_forward();
    void combine_backward();
    heap_segment_header* split(size_t size);
};

struct alinged_heap_segment_header {
    uint64_t segment_header_address : 63;
    bool isAlinged : 1;
};

heap_segment_header* first_free_segment;

void init_heap(uint64_t heapAddress, size_t size);
void* malloc(size_t size);
void* realloc(void* ptr, size_t size);
void* calloc(size_t size);
void* calloc(uint64_t num, size_t size);
void combine_free_segments(heap_segment_header* a, heap_segment_header* b);
void free(void* ptr);
void* alinged_alloc(uint64_t alingment, size_t size);
void load_heap(heap_segment_header* heap);