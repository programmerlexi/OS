#include "bitmap.h"
void draw_bitmap(uint64_t x, uint64_t y, uint32_t color, uint8_t* bitmap) {
    for (uint64_t chrY = 0; chrY < 16; chrY++) {
        uint8_t row = bitmap[chrY];
        for (uint64_t chrX = 0; chrX < 8; chrX++) {
            if (row&0b10000000) vga_graphics::putpixel(chrX+x,chrY+y,color);
            else vga_graphics::putpixel(x+chrX,y+chrY,0);
            row <<= 1;
        }
    }
}

void print_bitmap_char(uint8_t chr,uint64_t x, uint64_t y, uint32_t color) {
    uint8_t* cBmp = g_8x16_font+(16*chr);
    draw_bitmap(x,y,color,cBmp);
}

void print_bitmap_string(char* str,uint64_t x, uint64_t y, uint32_t color) {
    while (*str != 0) {
        if ((x/8) >= c_width) {
            y += 16;
            x = 0;
        }
        switch (*str) {
            case '\n':
                y += 16;
                break;
            case '\r':
                x = 0;
                break;
            case '\b':
                x -= 8;
                vga_graphics::draw_rect(x,y,8,16,0);
                break;
            case '\t':
                x += 8*4;
                break;
            default:
                print_bitmap_char(*str,x,y,color);
                x += 8;
        }
        str += 1;
    }
}

void print_bitmap_string(const char* str, uint64_t x, uint64_t y, uint32_t color) {
    print_bitmap_string((char*)str,x,y,color);
}