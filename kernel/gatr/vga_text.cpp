#include "vga.h"
char* vgaBackBuffer;

void init_vga() {
    vgaBackBuffer = (char*)malloc(4800*sizeof(char));
    clear_screen();
}

void set_cursor_pos(uint64_t pos) {
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
    vga_pos = pos;
}

void putchar(char c, char colorCode, unsigned int pos) {
    *(char*)(vgaBackBuffer + pos) = c;
    *(char*)(vgaBackBuffer + pos + 1) = colorCode;
    /**(char*)(0xb8000 + pos) = c;
    *(char*)(0xb8000 + pos + 1) = colorCode;*/
}

uint8_t getchar(unsigned int pos) {
    return *(char*)(vgaBackBuffer + pos);
}

void swap_buffers() {
    memcpy((uint8_t*)0xb8000, (uint8_t*)vgaBackBuffer, VGA_WIDTH*VGA_HEIGHT*2);
}

void disable_cursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

char color(char bg, char fg) {
    char colorCode = (bg << 4) | fg;
    return colorCode;
}

unsigned int index(unsigned int char_index) {
    return char_index * 2;
}

void scroll_down() {
    memcpy((uint8_t*)vgaBackBuffer, (uint8_t*)(vgaBackBuffer + VGA_WIDTH*2), VGA_HEIGHT*(VGA_WIDTH*2));
    memset((uint8_t*)(vgaBackBuffer-VGA_WIDTH*2), 0, VGA_WIDTH*2);
    vga_pos -= VGA_WIDTH;
}

void clear_screen() {
    for (int i = 0; i < 2000; i++) {
        putchar(' ', color(0x0, 0xF), index(i));
    }
    set_cursor_pos(0);
}

void reset_vga_pos() {
    set_cursor_pos(0);
}

void print_char(char c) {
    uint16_t idx = vga_pos;
    switch (c)
        {
            case '\n':
                idx += 80;
                break;
            case 0:
                break;
            case '\r':
                idx -= idx % 80;
                break;
            case '\b':
                idx -= 1;
                putchar(' ', color(0x0, 0xF), index(idx));
                break;
            case '\t':
                idx += 4;
                break;
            default:
                putchar(c, color(0x0, 0xF), index(idx));
                idx++;
                break;
        }
    while (idx > VGA_WIDTH*VGA_HEIGHT) {
        scroll_down();
        idx = vga_pos;
    }
    set_cursor_pos(idx);
}

void print_string(const char *str)
{
    uint8_t* charPtr = (uint8_t*)str;
    uint16_t idx = vga_pos;
    while (*charPtr != 0)
    {
        
        switch (*charPtr)
        {
            case '\n':
                idx += 80;
                break;
            case '\r':
                idx -= idx % 80;
                break;
            case '\b':
                idx -= 1;
                break;
            case '\t':
                idx += 4;
                break;
            default:
                putchar(*charPtr, color(0x0, 0xF), index(idx));
                idx++;
                break;
        }
        charPtr++;
    }
    while (idx >= VGA_WIDTH*VGA_HEIGHT) {
        scroll_down();
        idx = vga_pos;
    }
    set_cursor_pos(idx);
    swap_buffers();
}

void print_string(const char *str, int len) {
    for (int i = 0; i < len; i++) {
        print_char(str[i]);
    }
}

void print_string_no_whitespaces(const char *str)
{
    uint8_t* charPtr = (uint8_t*)str;
    uint16_t idx = vga_pos;
    while (*charPtr != 0)
    {
        switch (*charPtr)
        {
            default:
                putchar(*charPtr, color(0x0, 0xF), index(idx));
                idx++;
                break;
        }
        charPtr++;
    }
    set_cursor_pos(idx);
}

char hexToStringOutput[128];
template<typename T>
const char* HexToString(T value){
  T* valPtr = &value;
  uint8_t* ptr;
  uint8_t temp;
  uint8_t size = (sizeof(T)) * 2 - 1;
  uint8_t i;
  for (i = 0; i < size; i++){
    ptr = ((uint8_t*)valPtr + i);
    temp = ((*ptr & 0xF0) >> 4);
    hexToStringOutput[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : 48);
    temp = ((*ptr & 0x0F));
    hexToStringOutput[size - (i * 2 + 0)] = temp + (temp > 9 ? 55 : 48);
  }
  hexToStringOutput[size + 1] = 0;
  return hexToStringOutput;
}

void print_int(int value){
    print_string(HexToString(value));
}

void color_rect(int x, int y, int width, int height, char colorCode) {
    if (width < 0) {
        x += width;
        width = -width;
        width += 1;
        x -= 1;
    }
    if (height < 0) {
        y += height;
        height = -height;
        height += 1;
        y -= 1;
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            *(char*)(vgaBackBuffer + index(x + j + (y + i) * 80) + 1) = colorCode; 
        }
    }
}

void switch_to_text_mode() {
    outb(0x3D4, 0x3);
    outb(0x3D5, 0x3);
}