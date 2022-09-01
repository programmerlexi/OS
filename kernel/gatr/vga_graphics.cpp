#include "vga.h"
#include "../utils/math.h"
#include "../memory/heap.h"

namespace vga_graphics {
uint8_t* graphics_buffer;
uint8_t* graphics_buffer_back;

unsigned char g_720x480x16[] =
{
/* MISC */
	0x63,
/* SEQ */
	0x03, 0x01, 0x0F, 0x00, 0x0E,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 
    0x54, 0x80, 0xBF, 0x1F,
    0x00, 0x41, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00,
    0x9C, 0x0E, 0x8F, 0x28,	
    0x40, 0x96, 0xB9, 0xA3,
    0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 
    0x00, 0x40, 0x05, 0x0F,
    0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F,
    0x41, 0x00, 0x0F, 0x00,
    0x00,
};

void load_graphics_registers()
{
    outb(GRAPHICS_MISC_WRITE, g_720x480x16[0]);

    int i = 1;
    int in = i;
    // write sequence data to index of 0-4
    for(uint8_t index = 0; index < 5; index++){
        // first set index
        outb(GRAPHICS_SEQ_INDEX, index);
        // write data at that index
        outb(GRAPHICS_SEQ_DATA, g_720x480x16[in+index]);
        i++;
    }
    in = i;

    // write crtc data to index of 0-24
    for(uint8_t index = 0; index < 25; index++){
        outb(GRAPHICS_CRTC_INDEX, index);
        outb(GRAPHICS_CRTC_DATA, g_720x480x16[in+index]);
        i++;
    }
    in = i;

    // write gc data to index of 0-8
    for(uint8_t index = 0; index < 9; index++){
        outb(GRAPHICS_GC_INDEX, index);
        outb(GRAPHICS_GC_DATA, g_720x480x16[in+index]);
        i++;
    }
    in = i;

    uint8_t d;
    // write ac data to index of 0-20
    for(uint8_t index = 0; index < 21; index++){
        outb(GRAPHICS_AC_INDEX, index);
        outb(GRAPHICS_AC_WRITE, g_720x480x16[in+index]);
    }
    d = inb(GRAPHICS_INSTAT_READ);
    outb(GRAPHICS_AC_INDEX, d | 0x20);
    
}

void clear_screen() {
    for (uint32_t i = 0; i < GRAPHICS_MAX; i++) {
        graphics_buffer[i] = 0x00;
    }
}

void putpixel(uint16_t x, uint16_t y, uint8_t color)
{
  uint32_t index = 0;
  index = GRAPHICS_MAX_WIDTH * y + x;
  if(index < GRAPHICS_MAX)
    graphics_buffer[index] = color;
}

void setpixel(uint16_t index, uint8_t color)
{
  if(index < GRAPHICS_MAX)
    graphics_buffer[index] = color;
}

void draw_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint8_t color) {
    for (uint64_t i = 0; i < height; i++) {
        for (uint64_t j = 0; j < width; j++) {
            putpixel(x + j, y + i, color);
        }
    }
}

static unsigned get_fb_seg(void)
{
	unsigned seg;

	outb(GRAPHICS_GC_INDEX, 6);
	seg = inb(GRAPHICS_GC_DATA);
	seg >>= 2;
	seg &= 3;
	switch(seg)
	{
	case 0:
	case 1:
		seg = 0xA000;
		break;
	case 2:
		seg = 0xB000;
		break;
	case 3:
		seg = 0xB800;
		break;
	}
	return seg;
}

void init_graphics() {
    load_graphics_registers();
    graphics_buffer = (uint8_t*)0xA0000;
    clear_screen();
}

void switch_to_graphics_mode() {
    load_graphics_registers();
}
} 