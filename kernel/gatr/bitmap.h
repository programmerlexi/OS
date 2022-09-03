#pragma once
#include "vga.h"
void draw_bitmap(uint64_t x, uint64_t y, uint32_t color, uint8_t* bitmap);
void print_bitmap_char(uint8_t chr,uint64_t x, uint64_t y, uint32_t color);
void print_bitmap_string(char* str,uint64_t x, uint64_t y, uint32_t color);
void print_bitmap_string_on_grid(char* str); // Default color