#include "allocator.h"

void* allocate_blocks(int blocks) {
    int in_follow = 0;
    for (unsigned long i = 0; i < 1048576; i++) {
        if (buddy0[i]==0) {
            in_follow++;
            if (in_follow == blocks) {
                int j = 0;
                for (j = i; j > (i-blocks); j--)
                    buddy0[j] = 1;
                return (void*)(j*(4*1024));
            }
        } else {
            in_follow = 0;
        }
    }
    return (void*)0;
}