#pragma once
#include <stdint.h>
#include <stddef.h>

extern "C" void loadPageDirectory(unsigned int*);
extern "C" void enablePaging();

unsigned int page_directory[1024] __attribute__((aligned(4096)));
unsigned int first_page_table[1024] __attribute__((aligned(4096)));
unsigned int second_page_table[1024] __attribute__((aligned(4096)));
unsigned int third_page_table[1024] __attribute__((aligned(4096)));
void init_paging();