#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct heap_segment_header {
    size_t size;
    heap_segment_header* next;
    heap_segment_header* prev;
    bool free;
};

heap_segment_header* first_free_segment;

void init_heap(uint64_t heapAddress, size_t size);
void* malloc(size_t size);
void* calloc(size_t size);
void* calloc(uint64_t num, size_t size);
void combine_free_segments(heap_segment_header* a, heap_segment_header* b);
void free(void* ptr);
void load_heap(heap_segment_header* heap);
int count_segments();