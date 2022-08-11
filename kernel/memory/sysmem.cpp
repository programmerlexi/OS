#include "sysmem.h"
#include "heap.h"
sysmem_t* sysmem;
void init_sysmem(uint64_t start, size_t size) {
    sysmem = (sysmem_t*)start;
    sysmem->size = size;
    sysmem->next = NULL;
    sysmem->prev = NULL;
    init_heap(start + sizeof(sysmem_t), size);
}
sysmem_t* get_sysmem_seg() {
    sysmem_t* current = sysmem;
    while (current->next != NULL) {
        current = current->next;
    }
    return current;
}
sysmem_t* get_sysmem_seg_next() {
    return get_sysmem_seg()+sizeof(sysmem_t)+get_sysmem_seg()->size;
}
sysmem_t* add_new_sysmem(uint32_t size) {
    sysmem_t* new_sysmem = get_sysmem_seg_next();
    new_sysmem->size = size;
    new_sysmem->next = NULL;
    new_sysmem->prev = get_sysmem_seg();
    heap_segment_header* old_heap = first_free_segment;
    init_heap((uint64_t)new_sysmem+sizeof(sysmem_t), size);
    load_heap(old_heap);
    return new_sysmem;
}
void return_to_sysmem() {
    load_heap((heap_segment_header*)(sysmem+sizeof(sysmem_t)));
}
void load_sysmem_seg(sysmem_t* seg) {
    load_heap((heap_segment_header*)(seg+sizeof(sysmem_t)));
}