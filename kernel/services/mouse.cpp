#include "mouse.h"
#include "pic.h"

uint8_t mouse_cycle = 0;
char mouse_packet[4];
bool mouse_packet_ready = false;
void handle_ps2_mouse_data(uint8_t data) {
    switch (mouse_cycle) {
        case 0:
            if (mouse_packet_ready) break;
            if (data & 0b00001000 == 0) break; // something went wrong with the data transfer
            mouse_packet[0] = data;
            mouse_cycle++;
            return;
            break;
        case 1:
            if (mouse_packet_ready) break;
            mouse_packet[1] = data;
            mouse_cycle++;
            return;
            break;
        case 2:
            if (mouse_packet_ready) break;
            mouse_packet[2] = data;
            mouse_packet_ready = true;
            mouse_cycle = 0;
            return;
            break;
    }
    //print_string("Something went wrong with the mouse packet.\n\r");
}

Point move_mouse(uint8_t xNegative, uint8_t yNegative, uint8_t xOverflow, uint8_t yOverflow) {
    Point mouse_vector = {
        .x = (int)mouse_packet[1],
        .y = (int)mouse_packet[2]
    };

    if (xNegative) {
        if (xOverflow) mouse_pos.x += 255;
    }
    if (yNegative) {
        if (yOverflow) mouse_pos.y += 255;
    }
    if (!xNegative) {
        if (xOverflow) mouse_pos.x -= 255;
    }
    if (!yNegative) {
        if (yOverflow) mouse_pos.y -= 255;
    }
    mouse_vector.x /= 4;
    mouse_vector.y /= 4;
    return mouse_vector;
}

Point repos_mouse(Point mouse_vector) {
    mouse_pos.x += mouse_vector.x;
    mouse_pos.y -= mouse_vector.y;
    if (mouse_pos.x < 0) mouse_pos.x = 0;
    if (mouse_pos.y < 0) mouse_pos.y = 0;
    if (mouse_pos.x > VGA_WIDTH-1) mouse_pos.x = VGA_WIDTH-1;
    if (mouse_pos.y > VGA_HEIGHT-1) mouse_pos.y = VGA_HEIGHT-1;
    putchar(old_char, color(0x0,0xF), index(mouse_pos_old.x + (mouse_pos_old.y*VGA_WIDTH)));
    old_char = getchar(index(mouse_pos.x + (mouse_pos.y*VGA_WIDTH)));
    putchar('A', color(0x0, pressed_buttons.left ? GREEN : (pressed_buttons.right ? RED : (pressed_buttons.middle ? BLUE : WHITE))), index(mouse_pos.x + (mouse_pos.y*VGA_WIDTH)));
    //vga_graphics::draw_rect(mouse_pos.x-1, mouse_pos.y-1, 3, 3, color(0x0, pressed_buttons.left ? GREEN : (pressed_buttons.right ? RED : (pressed_buttons.middle ? BLUE : WHITE))));
    //vga_graphics::draw_rect(mouse_pos.x-1, mouse_pos.y-1, 3, 3, color(0x0, pressed_buttons.left ? GREEN : (pressed_buttons.right ? RED : (pressed_buttons.middle ? BLUE : WHITE))));
    swap_buffers();
    return mouse_pos;
}

void redraw_mouse() {
    putchar('A', color(0x0, pressed_buttons.left ? GREEN : (pressed_buttons.right ? RED : (pressed_buttons.middle ? BLUE : WHITE))), index(mouse_pos.x + (mouse_pos.y*VGA_WIDTH)));
}

void mouse_click_handler(bool mouse_l, bool mouse_r, bool mouse_m) {
    if (mouse_l) {
        if (!mouse_clicked) {
            mouse_start_click = true;
            mouse_clicked = true;
        } else {
            mouse_start_click = false;
        }
    } else {
        if (mouse_clicked) {
            mouse_stop_click = true;
            mouse_clicked = false;
        } else {
            mouse_stop_click = false;
        }
    }

    pressed_buttons.left = mouse_l;
    pressed_buttons.right = mouse_r;
    pressed_buttons.middle = mouse_m;
}

void mouse_after_move(bool force) {
    if (mouse_pos.x == mouse_pos_old.x && mouse_pos.y == mouse_pos_old.y && !force) return;
    if (mouse_clicked) {
        color_rect(mouse_pos_start_click.x, mouse_pos_start_click.y, mouse_pos_old.x - mouse_pos_start_click.x + 1, mouse_pos_old.y - mouse_pos_start_click.y + 1, color(0x0,0xF));
        color_rect(mouse_pos_start_click.x, mouse_pos_start_click.y, mouse_pos.x - mouse_pos_start_click.x + 1, mouse_pos.y - mouse_pos_start_click.y + 1, color(GREEN,GREEN));
    } else if (mouse_stop_click) {
        color_rect(mouse_pos_start_click.x, mouse_pos_start_click.y, mouse_pos_old.x - mouse_pos_start_click.x + 1, mouse_pos_old.y - mouse_pos_start_click.y + 1, color(0x0,0xF));
    }
}

void process_mouse_packet() {
    if (!mouse_packet_ready) return;
    //print_string("Processing mouse packet...\n\r");
    mouse_pos_old.x = mouse_pos.x;
    mouse_pos_old.y = mouse_pos.y;
    
    mouse_packet_ready = false;
    uint8_t xNegative, yNegative, xOverflow, yOverflow;
    xNegative = mouse_packet[0] & PS2XSIGN;
    yNegative = mouse_packet[0] & PS2YSIGN;
    xOverflow = mouse_packet[0] & PS2XOVERFLOW;
    yOverflow = mouse_packet[0] & PS2YOVERFLOW;
    
    mouse_click_handler(mouse_packet[0] & PS2LMB, mouse_packet[0] & PS2RMB, mouse_packet[0] & PS2MMB);

    if (mouse_start_click) {
        mouse_pos_start_click.x = mouse_pos.x;
        mouse_pos_start_click.y = mouse_pos.y;
    }

    Point mouse_vector = move_mouse(xNegative, yNegative, xOverflow, yOverflow);
    repos_mouse(mouse_vector);
    mouse_after_move(mouse_stop_click);
}

void mouse_handler(struct regs *r) {
    uint8_t mouseData = mouse_read();//inb(0x60);
    handle_ps2_mouse_data(mouseData);
    //print_string("m");
}

void mouse_wait() {
    uint64_t timeout = 100000;
    while (timeout--) {
        if (inb(0x64) & 0b10) {
            return;
        }
    }
}

void mouse_wait_input() {
    uint64_t timeout = 100000;
    while (timeout--) {
        if (inb(0x64) & 0b1) {
            return;
        }
    }
}

void mouse_write(uint8_t val) {
    mouse_wait();
    outb(0x64, 0xD4);
    mouse_wait();
    outb(0x60, val);
}

uint8_t mouse_read() {
    mouse_wait_input();
    return inb(0x60);
}

void enable_mouse() {
    print_string("Setting up mouse...              \r");
    outb(0x64, 0xA8);
    mouse_wait();
    outb(0x64, 0x20);
    print_string("Enabling mouse...                \r");
    mouse_wait_input();
    uint8_t status = inb(0x60);
    status |= 0b10;
    mouse_wait();
    outb(0x64, 0x60);
    mouse_wait();
    outb(0x60, status);
    mouse_write(0xF6);
    mouse_read();
    mouse_write(0xF4);
    mouse_read();
}

void mouse_install() {
    print_string("Installing mouse handler...               \r");
    irq_install_handler(0xC, mouse_handler);
    print_string("Initializing mouse position...         \r");
    mouse_pos.x = (int)VGA_WIDTH/2;
    mouse_pos.y = (int)VGA_HEIGHT/2;
    mouse_pos_old.x = (int)VGA_WIDTH/2;
    mouse_pos_old.y = (int)VGA_HEIGHT/2;
    //putchar('A', color(0x0,0xF), index(mouse_pos.x + (mouse_pos.y*VGA_WIDTH)));
}