#pragma once
#include "../utils/util.h"
#include "../interrupts/regs.h"
#include "../interrupts/irq.h"
#include "../gatr/vga.h"

#define DATA_PORT 0x60 

char kpressed[128] = {0,};
bool input_enabled = false;
char inputList[128] = {0,};
int inputPos = 0;

/*unsigned char kbdus[128] =
{
    0,  0x1b, '1', '2', '3', '4', '5', '6', '7', '8',	// 9
  '9', '0', '-', '=', '\b',	// Backspace
  '\t',			// Tab
  'q', 'w', 'e', 'r',	// 19
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	// Enter key
    0,			// 29   - Control
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	// 39
 '\'', '`',   0,		// Left shift
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			// 49
  'm', ',', '.', '/',   0,				// Right shift
  '*',
    0,	// Alt
  ' ',	// Space bar
    0,	// Caps lock
    0,	// 59 - F1 key ... >
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	// < ... F10
    0,	// 69 - Num lock
    0,	// Scroll Lock
    0,	// Home key
    0,	// Up Arrow
    0,	// Page Up
  '-',
    0,	// Left Arrow
    0,
    0,	// Right Arrow
  '+',
    0,	// 79 - End key
    0,	// Down Arrow
    0,	// Page Down
    0,	// Insert Key
    0,	// Delete Key
    0,   0,   0,
    0,	// F11 Key
    0,	// F12 Key
    0,	// All other keys are undefined
};

unsigned char kbdus_shift[128] =
{
    0,  
    0x1b, // esc
    '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
    '_', '+', '\b', '\t', 'Q', 'W', 'E', 'R',    
    'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0,  // left control
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':','\"', '~',
    0,    // left shift
    '|', 'Z', 'X', 'C', 'V', 'B', 'N',
    'M', '<', '>', '?',   
    0,    // right shift
    '*',
    0,  // alt
    ' ',// space bar
    0,  // caps lock
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // f1 ... f10
    0,    // num lock
    0,    // scroll lock
    0,    // home key
    0,    // up arrow
    0,    // page up
    '-',
    0,    // left arrow
    0,
    0,    // right arrow
    '+',
    0,    // end key
    0,    // down arrow
    0,    // page down
    0,    // insert key
    0,    // delete key
    0, 0, 0,
    0,    // f11 key
    0,    // f12 key
    0,    // all other keys are undefined 
};*/

bool shift_pressed = false;
bool caps_lock = false;
bool enter_pressed = false;
void clear_input();
char* get_input();
void set_input(bool input);
void kb_install();