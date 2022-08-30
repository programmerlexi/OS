#include "heap.h"
#include "memory.h"

//heap_segment_header* first_free_segment;

void init_heap(uint64_t heapAddress, size_t size) {
    first_free_segment = (heap_segment_header*)heapAddress;
    first_free_segment->size = size;
    first_free_segment->next = NULL;
    first_free_segment->prev = NULL;
    first_free_segment->free = true;
}

int count_segments() {
    heap_segment_header* segment = first_free_segment;
    while (segment->prev != NULL) {
        segment = segment->prev;
    }
    int i = 1;
    while (segment->next) {
        segment = segment->next;
        i++;
    }
    return i;
}

void load_heap(heap_segment_header* first) {
    first_free_segment = first;
}

heap_segment_header* get_new_segment(heap_segment_header* ptr, uint64_t offset,size_t size) {
    uint64_t addr = (uint64_t)ptr;
    addr += offset;
    heap_segment_header* segment = (heap_segment_header*)addr;
    segment->size = size;
    segment->next = ptr->next;
    segment->prev = ptr;
    segment->free = true;
    ptr->next = segment;
    return segment;
}

heap_segment_header* get_header_of_ptr(void* ptr) {
    uint64_t addr = (uint64_t)ptr;
    addr -= sizeof(heap_segment_header);
    return (heap_segment_header*)addr;
}

void* get_ptr_from_header(heap_segment_header* header) {
    uint64_t addr = (uint64_t)header;
    addr += sizeof(heap_segment_header);
    return (void*)addr;
}

void* malloc(size_t size) {
    heap_segment_header* segment = first_free_segment;
    if (segment->size == 0) {
        regs_t* r = get_regs();
        r->eax = (int)segment;
        kpanic("NULL Segment as first free segment",r);
    }
    while (1) {
        if (segment->free) {
            if (segment->size >= size) {
                if (segment->size > size+sizeof(heap_segment_header)*2) {
                    heap_segment_header* new_segment = get_new_segment(segment,size+sizeof(heap_segment_header),segment->size-size);
                }
                segment->free = false;
                return get_ptr_from_header(segment);
            }
        }
        if (segment->next == NULL) {
            return NULL;
        }
        segment = segment->next;
    }
    return NULL;
}

void* calloc(uint64_t num, size_t size) {
    uint8_t* ptr = (uint8_t*)malloc(size);
    memset(ptr, num, size);
    return ptr;
}
void* calloc(size_t size) {
    return calloc(0, size);
}

void combine_free_segments(heap_segment_header* a, heap_segment_header* b) {
    if (a==NULL || b==NULL) return;
    if (!a->free || !b->free) return;
    if (a < b) {
        if (((size_t)a->next)!=(size_t)b || ((size_t)b->prev)!=(size_t)a) return;
        a->next = b->next;
        a->size += b->size+sizeof(heap_segment_header);
    } else if (b < a) {
        if (((size_t)b->next)!=(size_t)a || ((size_t)a->prev)!=(size_t)b)return;
        b->next = a->next;
        b->size += a->size+sizeof(heap_segment_header);
    }
}

void free(void* ptr) {
    heap_segment_header* segment = get_header_of_ptr(ptr);
    segment->free = true;
    if (segment->next != NULL) combine_free_segments(segment, segment->next);
    if (segment->prev != NULL) combine_free_segments(segment->prev, segment);
}