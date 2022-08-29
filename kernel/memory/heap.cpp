#include "heap.h"
#include "memory.h"

//heap_segment_header* first_free_segment;

void init_heap(uint64_t heapAddress, size_t size) {
    first_free_segment = (heap_segment_header*)heapAddress;
    first_free_segment->size = size;
    first_free_segment->next = NULL;
    first_free_segment->prev = NULL;
    first_free_segment->nextFree = NULL;
    first_free_segment->prevFree = NULL;
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

void combine_all_free_segments(int depth) {
    if (depth == 0) {
        return;
    }
    heap_segment_header* next = first_free_segment;
    while (next->prev != NULL) {
        next = next->prev;
    }
    while (next != NULL) {
        combine_free_segments(next, next->next);
        next = next->next;
    }
    combine_all_free_segments(depth-1);
}

void* malloc(size_t size) {
    /*size_t remainder = size % 8;
    size -= remainder;
    if (remainder != 0) {
        size += 8;
    }*/ // screw performance this breaks the allocator
    combine_all_free_segments(4);
    heap_segment_header* current = first_free_segment;
    while (true) {
        if (current->size >= size) {
            if (current->size > size + sizeof(heap_segment_header)) {
                heap_segment_header* new_segment = (heap_segment_header*)((uint64_t)current + sizeof(heap_segment_header) + size);
                new_segment->free = true;
                new_segment->size = current->size - size - sizeof(heap_segment_header);
                new_segment->next = current->next;
                new_segment->prev = current;
                new_segment->nextFree = current->nextFree;
                new_segment->prevFree = current->prevFree;

                current->next = new_segment;
                current->nextFree = new_segment;
                current->size = size;
            }
            if (current == first_free_segment) {
                first_free_segment = current->next;
            }
            current->free = false;
            if (current->prevFree != NULL) {
                current->prevFree->nextFree = current->nextFree;
            }
            if (current->nextFree != NULL) {
                current->nextFree->prevFree = current->prevFree;
            }
            if (current->prev != NULL) {
                current->prev->nextFree = current->nextFree;
            }
            if (current->next != NULL) {
                current->next->prevFree = current->prevFree;
            }
            return current+sizeof(heap_segment_header);
        }
        if (current->nextFree == NULL) {
            return NULL;
        }
        current = current->nextFree;
    }
    return NULL;
}

void* calloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        return NULL;
    }
    memset(ptr, 0, size);
    return ptr;
}

void* calloc(uint64_t num, size_t size) {
    return calloc(num*size);
}

void* realloc(void* ptr, size_t newSize) {
    heap_segment_header* old = (heap_segment_header*)ptr - sizeof(heap_segment_header);
    if (old->free) {
        return NULL;
    }
    uint64_t smallerSize = newSize;
    if (old->size < newSize) {
        smallerSize = old->size;
    }
    void* newPtr = malloc(newSize);
    memcpy(newPtr, ptr, smallerSize);
    free(ptr);
    return newPtr;
}

void combine_free_segments(heap_segment_header* a, heap_segment_header* b) {
    if (a == NULL || b == NULL) {
        return;
    }
    if (!a->free || !b->free) {
        return;
    }
    if (a < b) {
        a->size += b->size + sizeof(heap_segment_header);
        a->next = b->next;
        a->nextFree = b->nextFree;
        b->next->prev = a;
        b->next->prevFree = a;
        b->nextFree->prevFree = a;
    } else {
        b->size += a->size + sizeof(heap_segment_header);
        b->next = a->next;
        b->nextFree = a->nextFree;
        a->next->prev = b;
        a->next->prevFree = b;
        a->nextFree->prevFree = b;
    }
}

void free(void* ptr) {
    heap_segment_header* current;
    /*alinged_heap_segment_header* ahsh;
    if (ahsh->isAlinged) {
        current = (heap_segment_header*)(ahsh->segment_header_address);
    } else {
        current = (heap_segment_header*)((uint64_t)ptr - sizeof(heap_segment_header));
    }*/
    current = (heap_segment_header*)(((uint64_t)ptr) - sizeof(heap_segment_header));
    while (first_free_segment->prev && first_free_segment->prev->free) {
        first_free_segment = first_free_segment->prev;
    }
    current->free = true;
    if (current < first_free_segment) {
        first_free_segment = current;
    }
    if (current->nextFree != NULL) {
        if (current->nextFree->prevFree < current) {
            current->nextFree->prevFree = current;
        }
    }
    if (current->prevFree != NULL) {
        if (current->prevFree->nextFree > current) {
            current->prevFree->nextFree = current;
        }
    }
    if (current->next != NULL) {
        current->next->prev = current;
        if (current->next->free) {
            combine_free_segments(current, current->next);
        }
    }
    if (current->prev != NULL) {
        current->prev->next = current;
        if (current->prev->free) {
            combine_free_segments(current, current->prev);
        }
    }
}



/*void* alinged_alloc(uint64_t alingment, size_t size) {
    uint64_t alingment_remainder = alingment % 8;
    alingment -= alingment_remainder;
    if (alingment_remainder != 0) {
        alingment += 8;
    }
    size_t size_remainder = size % 8;
    size -= size_remainder;
    if (size_remainder != 0) {
        size += 8;
    }
    uint64_t totalSize = size + alingment;
    void* result = malloc(totalSize);
    uint64_t address = (uint64_t)result;

    uint64_t remainder = address % alingment;
    address -= remainder;
    if (remainder != 0) {
        address += alingment;

        alinged_heap_segment_header* alhs = (alinged_heap_segment_header*)address- sizeof(heap_segment_header);
        alhs->isAlinged = true;
        alhs->segment_header_address = (uint64_t)result - sizeof(heap_segment_header);
    }
    return (void*)address;
}*/