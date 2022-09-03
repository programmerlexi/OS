#include "vga.h"
char* vgaBackBuffer;
char* vgaBuffer;

unsigned char g_80x25_text[] =
{
/* MISC */
	0x67,
/* SEQ */
	0x03, 0x00, 0x03, 0x00, 0x02,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F,
	0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x50,
	0x9C, 0x0E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x0C, 0x00, 0x0F, 0x08, 0x00
};

unsigned char g_90x30_text[] {
/* MISC */
	0xE7,
/* SEQ */
	0x03, 0x01, 0x03, 0x00, 0x02,
/* CRTC */
	0x6B, 0x59, 0x5A, 0x82, 0x60, 0x8D, 0x0B, 0x3E,
	0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
	0xEA, 0x0C, 0xDF, 0x2D, 0x10, 0xE8, 0x05, 0xA3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x0C, 0x00, 0x0F, 0x08, 0x00,
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

static void vmemwr(unsigned dst_off, unsigned char *src, unsigned count)
{
	_vmemwr(get_fb_seg(), dst_off, src, count);
}

static void write_font(unsigned char *buf, unsigned font_height)
{
	unsigned char seq2, seq4, gc4, gc5, gc6;
	unsigned i;

/* save registers
set_plane() modifies GC 4 and SEQ 2, so save them as well */
	outb(GRAPHICS_SEQ_INDEX, 2);
	seq2 = inb(GRAPHICS_SEQ_DATA);

	outb(GRAPHICS_SEQ_INDEX, 4);
	seq4 = inb(GRAPHICS_SEQ_DATA);
/* turn off even-odd addressing (set flat addressing)
assume: chain-4 addressing already off */
	outb(GRAPHICS_SEQ_DATA, seq4 | 0x04);

	outb(GRAPHICS_GC_INDEX, 4);
	gc4 = inb(GRAPHICS_GC_DATA);

	outb(GRAPHICS_GC_INDEX, 5);
	gc5 = inb(GRAPHICS_GC_DATA);
/* turn off even-odd addressing */
	outb(GRAPHICS_GC_DATA, gc5 & ~0x10);

	outb(GRAPHICS_GC_INDEX, 6);
	gc6 = inb(GRAPHICS_GC_DATA);
/* turn off even-odd addressing */
	outb(GRAPHICS_GC_DATA, gc6 & ~0x02);
/* write font to plane P4 */
	set_plane(2);
/* write font 0 */
	for(i = 0; i < 256; i++)
	{
		vmemwr(16384u * 0 + i * 32, buf, font_height);
		buf += font_height;
	}
#if 0
/* write font 1 */
	for(i = 0; i < 256; i++)
	{
		vmemwr(16384u * 1 + i * 32, buf, font_height);
		buf += font_height;
	}
#endif
/* restore registers */
	outb(GRAPHICS_SEQ_INDEX, 2);
	outb(GRAPHICS_SEQ_DATA, seq2);
	outb(GRAPHICS_SEQ_INDEX, 4);
	outb(GRAPHICS_SEQ_DATA, seq4);
	outb(GRAPHICS_GC_INDEX, 4);
	outb(GRAPHICS_GC_DATA, gc4);
	outb(GRAPHICS_GC_INDEX, 5);
	outb(GRAPHICS_GC_DATA, gc5);
	outb(GRAPHICS_GC_INDEX, 6);
	outb(GRAPHICS_GC_DATA, gc6);
}

void load_text_registers() {
    unsigned i;
    uint8_t* regs = g_80x25_text;
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

    write_font(g_8x16_font,16);

    /*pokew(0x40, 0x4A, 80);	// columns on screen
	pokew(0x40, 0x4C, 80 * 25 * 2); // framebuffer size
	pokew(0x40, 0x50, 0);		// cursor pos'n
	pokeb(0x40, 0x60, 16 - 1);	// cursor shape
	pokeb(0x40, 0x61, 16 - 2);
	pokeb(0x40, 0x84, 25 - 1);	// rows on screen - 1
	pokeb(0x40, 0x85, 16);		// char height */
}

void init_vga() {
    //vgaBackBuffer = (char*)malloc(4800*sizeof(char));
	switch_to_text_mode();
    /*vgaBuffer = (char*)0xb8000;
    clear_screen();*/
}

void set_cursor_pos(uint64_t pos) {
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
    vga_pos = pos;
}

void putchar(char c, char colorCode, unsigned int pos) {
    *(char*)(vgaBuffer + pos) = c;
    *(char*)(vgaBuffer + pos + 1) = colorCode;
    /**(char*)(0xb8000 + pos) = c;
    *(char*)(0xb8000 + pos + 1) = colorCode;*/
}

uint8_t getchar(unsigned int pos) {
    return *(char*)(vgaBuffer + pos);
}

void swap_buffers() {
    //memcpy(vgaBuffer, (uint8_t*)vgaBackBuffer, VGA_WIDTH*VGA_HEIGHT*2);
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
	if (c_reroute) {
		vga_graphics::scroll_down(2);
		vga_pos -= (int)vga_pos % c_width;
		return;
	}
    memcpy((uint8_t*)vgaBuffer, (uint8_t*)(vgaBuffer + VGA_WIDTH*2), (VGA_HEIGHT-1)*(VGA_WIDTH*2));
    memset((uint8_t*)(vgaBuffer+((VGA_HEIGHT-1)*(VGA_WIDTH*2))), 0, VGA_WIDTH*2);
    vga_pos -= (int)vga_pos % c_width;
}

void clear_screen() {
	if (c_reroute) {
		vga_graphics::clear_screen();
		set_cursor_pos(0);
		return;
	}
    for (int i = 0; i < c_width*c_height; i++) {
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
                idx += c_width;
                break;
            case 0:
                break;
            case '\r':
                idx -= idx % c_width;
                break;
            case '\b':
                idx -= 1;
				if (c_reroute) {
                	vga_graphics::draw_rect(idx%c_width*8,idx/c_width*16,8,16,0);
				} else {
					putchar(' ', color(0x0, 0xF), index(idx));
				}
                break;
            case '\t':
                idx += 4;
                break;
            default:
				if (c_reroute) {
					print_bitmap_char(c, idx%c_width*8,idx/c_width*16, 0xf);
				} else {
                	putchar(c, color(0x0, 0xF), index(idx));
				}
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
	if (c_reroute) {
		print_bitmap_string((char*)charPtr, idx%c_width*8,idx/c_width*16,0xf);
	}
    while (*charPtr != 0)
    {
        
        switch (*charPtr)
        {
            case '\n':
                idx += c_width;
                break;
            case '\r':
                idx -= idx % c_width;
                break;
            case '\b':
                idx -= 1;
                break;
            case '\t':
                idx += 4;
                break;
            default:
				if (!c_reroute) putchar(*charPtr, color(0x0, 0xF), index(idx));
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
                if (!c_reroute) putchar(*charPtr, color(0x0, 0xF), index(idx));
				else print_bitmap_char(*charPtr,idx%c_width*8,idx/c_width*16,0xf);
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
    if (size >= (i*2+1)) {
        temp = ((*ptr & 0xF0) >> 4);
        hexToStringOutput[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : 48);
    }
    if (size >= (i*2)) {
        temp = ((*ptr & 0x0F));
        hexToStringOutput[size - (i * 2 + 0)] = temp + (temp > 9 ? 55 : 48);
    }
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
            *(char*)(vgaBuffer + index(x + j + (y + i) * 80) + 1) = colorCode; 
        }
    }
}

void switch_to_text_mode() {
    load_text_registers();
	c_width = 80;
	c_height = 25;
	c_reroute = false;
    vgaBuffer = (char*)(get_fb_seg()*16);
    clear_screen();
    reset_vga_pos();
}