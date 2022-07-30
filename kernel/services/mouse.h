#pragma once
#include "../interrupts/irq.h"
#include "../gatr/vga.h"
#include "../utils/math.h"

#define PS2LMB 0b00000001
#define PS2MMB 0b00000100
#define PS2RMB 0b00000010

#define PS2XSIGN 0b00010000
#define PS2YSIGN 0b00100000
#define PS2XOVERFLOW 0b01000000
#define PS2YOVERFLOW 0b10000000

struct mouse_buttons {
    bool left;
    bool right;
    bool middle;
};

mouse_buttons pressed_buttons;

Point mouse_pos;
Point mouse_pos_old;
Point mouse_pos_start_click;
Point mouse_pos_stop_click;

bool mouse_clicked = false;
bool mouse_start_click = false;
bool mouse_stop_click = false;

uint8_t old_char;

void mouse_handler(struct regs *r);
void mouse_install();
void handle_ps2_mouse_data(uint8_t data);
void mouse_wait();
void mouse_wait_input();
void mouse_write(uint8_t val);
uint8_t mouse_read();
void enable_mouse();
void process_mouse_packet();