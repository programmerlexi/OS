#include "keyboard.h"
#include "scancodeTranslator.h"
#include "pic.h"
void keyboard_handler(struct regs *r)
{
    disregard_regs(r);
    unsigned char scancode;

    scancode = inb(DATA_PORT);

    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
    if (scancode & 0x80)
    {
    	switch(scancode){
    		case 0xaa: shift_pressed = false; break;
    	}
        kpressed[scancode - 0x80] = 0;

    }
    else
    {
        if (input_enabled) {
    	    switch(scancode){
        		case 0x4b: set_cursor_pos(vga_pos-1); break;
        		case 0x4d: set_cursor_pos(vga_pos + 1); break;
    		    case 0x48: set_cursor_pos(vga_pos - 80); break;
    	    	case 0x50: set_cursor_pos(vga_pos + 80); break;
        		case 0x2a: shift_pressed = true; break;
        		case 0x3a: caps_lock = !caps_lock; break;
    		    default: 
                    print_char(QWERTYKeyboard::Translate(scancode, false)); //, color(0x0, 0xF), index(vga_pos));
                    char c = QWERTYKeyboard::Translate(scancode, false);
                    if (c == '\b') {
                        inputPos--;
                        if (inputPos < 0) {
                            inputPos = 0;
                        }
                        inputList[inputPos] = '\0';
                    } else {
                        inputList[inputPos] = QWERTYKeyboard::Translate(scancode, false);
                        inputPos++;
                    }
                    break;
    	    }
        }
        kpressed[scancode] = 1;
    }
}

void kb_install()
{
    print_string("Installing keyboard...          \r");
    irq_install_handler(1, keyboard_handler);
	print_string("Disabling keyboard...                 \r");
    input_enabled = false;
}

void clear_input(){
    for(int i = 0; i < 128; i++){
        inputList[i] = 0;
    }
    inputPos = 0;
}

char* get_input(){
    char* input = (char*)malloc(inputPos);
    for(int i = 0; i < inputPos; i++){
        input[i] = inputList[i];
    }
    return input;
}

void set_input(bool enabled){
    input_enabled = enabled;
}