#include "vga.h"
#include "../utils/math.h"
#include "../memory/heap.h"

namespace vga_graphics {
uint8_t* graphics_buffer;
uint8_t* graphics_buffer_back;

static uint8_t seq_data[5] = {0x03, 0x01, 0x0F, 0x00, 0x0E};
static uint8_t crtc_data[25] = {0x5F, 0x4F, 0x50, 0x82, 
                              0x54, 0x80, 0xBF, 0x1F,
                              0x00, 0x41, 0x00, 0x00, 
                              0x00, 0x00, 0x00, 0x00,
                              0x9C, 0x0E, 0x8F, 0x28,	
                              0x40, 0x96, 0xB9, 0xA3,
                              0xFF};

static uint8_t gc_data[9] = {0x00, 0x00, 0x00, 0x00, 
                          0x00, 0x40, 0x05, 0x0F,
                          0xFF};

static uint8_t ac_data[21] = {0x00, 0x01, 0x02, 0x03,
                            0x04, 0x05, 0x06, 0x07,
                            0x08, 0x09, 0x0A, 0x0B,
                            0x0C, 0x0D, 0x0E, 0x0F,
                            0x41, 0x00, 0x0F, 0x00,
                            0x00};

void set_misc()
{
  outb(GRAPHICS_MISC_WRITE, 0x63);
}

void set_seq()
{
  // write sequence data to index of 0-4
  for(uint8_t index = 0; index < 5; index++){
    // first set index
    outb(GRAPHICS_SEQ_INDEX, index);
    // write data at that index
    outb(GRAPHICS_SEQ_DATA, seq_data[index]);
  }
}

void set_crtc()
{
  // write crtc data to index of 0-24
  for(uint8_t index = 0; index < 25; index++){
    outb(GRAPHICS_CRTC_INDEX, index);
	  outb(GRAPHICS_CRTC_DATA, crtc_data[index]);
  }
}

void set_gc()
{
  // write gc data to index of 0-8
  for(uint8_t index = 0; index < 9; index++){
    outb(GRAPHICS_GC_INDEX, index);
    outb(GRAPHICS_GC_DATA, gc_data[index]);
  }
}

void set_ac()
{
  uint8_t d;
  // write ac data to index of 0-20
  for(uint8_t index = 0; index < 21; index++){
    outb(GRAPHICS_AC_INDEX, index);
    outb(GRAPHICS_AC_WRITE, ac_data[index]);
  }
  d = inb(GRAPHICS_INSTAT_READ);
  outb(GRAPHICS_AC_INDEX, d | 0x20);
}

void clear_screen() {
    for (int i = 0; i < GRAPHICS_MAX; i++) {
        graphics_buffer_back[i] = 0;
    }
}

void putpixel(uint16_t x, uint16_t y, uint8_t color)
{
  uint32_t index = 0;
  index = 320 * y + x;
  if(index < GRAPHICS_MAX)
    graphics_buffer_back[index] = color;
}

void setpixel(uint16_t index, uint8_t color)
{
  if(index < GRAPHICS_MAX)
    graphics_buffer_back[index] = color;
}

void draw_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint8_t color) {
    for (uint64_t i = 0; i < height; i++) {
        for (uint64_t j = 0; j < width; j++) {
            putpixel(x + j, y + i, color);
        }
    }
}

void swap() {
    for (uint32_t i = 0; i < GRAPHICS_MAX; i++) {
        graphics_buffer[i] = graphics_buffer_back[i];
    }
}

void init_graphics() {
    set_misc();
    set_seq();
    set_crtc();
    set_gc();
    set_ac();
    graphics_buffer_back = (uint8_t*)malloc(GRAPHICS_MAX);
    graphics_buffer = (uint8_t*)GRAPHICS_ADDRESS;
    clear_screen();
    swap();
}

void switch_to_graphics_mode() {
    set_misc();
    set_seq();
    set_crtc();
    set_gc();
    set_ac();
}
} 