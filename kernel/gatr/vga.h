#pragma once
#include <stdint.h>
#include "../utils/util.h"
#include "../memory/memory.h"
#include "../memory/heap.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define GRAPHICS_ADDRESS 0xA0000
#define GRAPHICS_MAX 0xF9FF
#define GRAPHICS_MAX_WIDTH 320
#define GRAPHICS_MAX_HEIGHT 200

/* Attribute Controller Registers */
#define	GRAPHICS_AC_INDEX 0x3C0
#define	GRAPHICS_AC_READ 0x3C1
#define	GRAPHICS_AC_WRITE 0x3C0

/*
Miscellaneous Output
*/
#define	GRAPHICS_MISC_READ 0x3CC
#define	GRAPHICS_MISC_WRITE 0x3C2

/* Sequencer Registers */
#define GRAPHICS_SEQ_INDEX 0x3C4
#define GRAPHICS_SEQ_DATA 0x3C5

/* GRAPHICS Color Palette Registers */
#define	GRAPHICS_DAC_READ_INDEX 0x3C7
#define	GRAPHICS_DAC_WRITE_INDEX 0x3C8
#define	GRAPHICS_DAC_DATA 0x3C9

/* Graphics Controller Registers */
#define GRAPHICS_GC_INDEX 0x3CE
#define GRAPHICS_GC_DATA 0x3CF

/* CRT Controller Registers */
#define GRAPHICS_CRTC_INDEX 0x3D4
#define GRAPHICS_CRTC_DATA 0x3D5

/* General Control and Status Registers */
#define	GRAPHICS_INSTAT_READ 0x3DA

enum Color {
    BLACK, //0x0
    BLUE,  //0x1
    GREEN, //0x2
    CYAN,  //0x3
    RED,   //0x4
    MAGENTA,//0x5
    BROWN,//0x6
    LIGHT_GRAY,//0x7
    DARK_GRAY,//0x8
    LIGHT_BLUE,//0x9
    LIGHT_GREEN,//0xa
    LIGHT_CYAN,//0xb
    LIGHT_RED,//0xc
    LIGHT_MAGENTA,//0xd
    YELLOW,//0xe
    WHITE//0xf
};
uint64_t vga_pos = 0;
void clear_screen();
void init_vga();
void set_cursor_pos(uint64_t pos);
void putchar(char c, char colorCode, unsigned int pos);
uint8_t getchar(unsigned int pos);
void swap_buffers();
char color(char bg, char fg);
unsigned int index(unsigned int char_index);
void print_char(char c);
void print_string(const char *str);
void print_string_no_white_spaces(const char *str);
template<typename T>
const char* HexToString(T value);
void reset_vga_pos();
void print_int(int value);
void color_rect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, char color);
void switch_to_text_mode();
static unsigned get_fb_seg();
namespace vga_graphics {
    void init_graphics();
    void switch_to_graphics_mode();
    void putpixel(uint16_t x, uint16_t y, uint8_t color);
    void draw_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint8_t color);
}