#include "vga.h"
#include "../utils/math.h"
#include "../memory/heap.h"

namespace vga_graphics {
uint8_t* graphics_buffer;
uint8_t* graphics_buffer_back;

unsigned char g_320x200x4[] =
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

unsigned char g_640x480x16[] =
{
/* MISC */
	0xE3,
/* SEQ */
	0x03, 0x01, 0x08, 0x00, 0x06,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
	0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xEA, 0x0C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x01, 0x00, 0x0F, 0x00, 0x00
};

unsigned char g_720x480x16[] =
{
/* MISC */
	0xE7,
/* SEQ */
	0x03, 0x01, 0x08, 0x00, 0x06,
/* CRTC */
	0x6B, 0x59, 0x5A, 0x82, 0x60, 0x8D, 0x0B, 0x3E,
	0x00, 0x40, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00,
	0xEA, 0x0C, 0xDF, 0x2D, 0x08, 0xE8, 0x05, 0xE3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x01, 0x00, 0x0F, 0x00, 0x00,
};

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

static void vpokeb(unsigned off, unsigned val)
{
	pokeb(get_fb_seg(), off, val);
}

static unsigned vpeekb(unsigned off)
{
	return peekb(get_fb_seg(), off);
}

void load_graphics_registers()
{
    unsigned i;
    uint8_t* regs = driver.registers;
/* write MISCELLANEOUS reg */
	outb(GRAPHICS_MISC_WRITE, *regs);
	regs++;
/* write SEQUENCER regs */
	for(i = 0; i < 5; i++)
	{
		outb(GRAPHICS_SEQ_INDEX, i);
		outb(GRAPHICS_SEQ_DATA, *regs);
		regs++;
	}
/* unlock CRTC registers */
	outb(GRAPHICS_CRTC_INDEX, 0x03);
	outb(GRAPHICS_CRTC_DATA, inb(GRAPHICS_CRTC_DATA) | 0x80);
	outb(GRAPHICS_CRTC_INDEX, 0x11);
	outb(GRAPHICS_CRTC_DATA, inb(GRAPHICS_CRTC_DATA) & ~0x80);
/* make sure they remain unlocked */
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;
/* write CRTC regs */
	for(i = 0; i < 25; i++)
	{
		outb(GRAPHICS_CRTC_INDEX, i);
		outb(GRAPHICS_CRTC_DATA, *regs);
		regs++;
	}
/* write GRAPHICS CONTROLLER regs */
	for(i = 0; i < 9; i++)
	{
		outb(GRAPHICS_GC_INDEX, i);
		outb(GRAPHICS_GC_DATA, *regs);
		regs++;
	}
/* write ATTRIBUTE CONTROLLER regs */
	for(i = 0; i < 20; i++)
	{
		(void)inb(GRAPHICS_INSTAT_READ);
		outb(GRAPHICS_AC_INDEX, i);
		outb(GRAPHICS_AC_WRITE, *regs);
		regs++;
	}
/* lock 16-color palette and unblank display */
	(void)inb(GRAPHICS_INSTAT_READ);
	outb(GRAPHICS_AC_INDEX, 0x20);
}

static void set_plane(unsigned p)
{
	unsigned char pmask;

	p &= 3;
	pmask = 1 << p;
/* set read plane */
	outb(GRAPHICS_GC_INDEX, 4);
	outb(GRAPHICS_GC_DATA, p);
/* set write plane */
	outb(GRAPHICS_SEQ_INDEX, 2);
	outb(GRAPHICS_SEQ_DATA, pmask);
}

void clear_screen() {
    unsigned wd_in_bytes = driver.width / 8;
    if (driver.direct) {
        for (uint32_t y = 0; y < driver.height; y++) {
            uint32_t truey = (y*driver.width);
            for (uint32_t x = 0; x < driver.width; x++) {
                graphics_buffer[x+truey] = 0;
            }
        }
    } else {
        unsigned pmask = 1;
        for (int i = 0; i < 4; i++) {
            set_plane(i);
            for (uint32_t y = 0; y < driver.height; y++) {
                uint32_t truey = (y*wd_in_bytes);
                for (uint32_t x = 0; x < driver.width; x++) {
                    unsigned mask = 0x80 >> ((x & 7) * 1);
                    graphics_buffer[(x/8)+truey] &= ~mask;
                }
            }
            pmask <<= 1;
        }
    }
}

void putpixel(uint32_t x, uint32_t y, uint32_t color) {
    ((void(*)(uint32_t, uint32_t, uint32_t))(driver.putpixel))(x,y,color);
}

void g_320x200x4_putpixel(uint32_t x, uint32_t y, uint32_t color)
{
  uint32_t index = 0;
  index = driver.width * y + x;
  if(index < (driver.width*driver.height))
    graphics_buffer[index] = (uint8_t)(color&0xff);
}

void g_640x480x16_putpixel(uint32_t x, uint32_t y, uint32_t color) {
    unsigned wd_in_bytes, off, mask, p, pmask;
    uint16_t c = color & 0xffff;
	wd_in_bytes = driver.width / 8;
	off = wd_in_bytes * y + x / 8;
	x = (x & 7) * 1;
	mask = 0x80 >> x;
	pmask = 1;
	for(p = 0; p < 4; p++)
	{
		set_plane(p);
		if(pmask & c)
			graphics_buffer[off] |= mask;
		else
			graphics_buffer[off] &= ~mask;
		pmask <<= 1;
	}
}

void draw_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint32_t color) {
    for (uint64_t i = 0; i < height; i++) {
        for (uint64_t j = 0; j < width; j++) {
            putpixel(x + j, y + i, color);
        }
    }
}

void set_mode(uint64_t idx) {
    switch(idx) {
        case 0:
            driver.width = 320;
            driver.height = 200;
            driver.registers = g_320x200x4;
            driver.putpixel = g_320x200x4_putpixel;
            driver.direct = true; // Allow direct writing to the graphics buffer
            break;
        case 1:
            driver.width = 640;
            driver.height = 480;
            driver.registers = g_640x480x16;
            driver.putpixel = g_640x480x16_putpixel;
            driver.direct = false; // We use multiple planes so we need to disallow direct writing
            break;
        case 2:
            driver.width = 720;
            driver.height = 480;
            driver.registers = g_720x480x16;
            driver.putpixel = g_640x480x16_putpixel;
            driver.direct = false;
            break;
    }
    load_graphics_registers();
    graphics_buffer = (uint8_t*)(get_fb_seg()*16);
    clear_screen();
}

void init_graphics() {
    set_mode(1);
}

void switch_to_graphics_mode() {
    set_mode(1);
}
} 