struct Window {
    int x;
    int y;
    int width;
    int height;
};

void draw_desktop() {
    clear_screen();
    color_rect(0, 0, VGA_WIDTH, VGA_HEIGHT, color(LIGHT_GRAY, WHITE));
}

void update_desktop() {
    draw_desktop();
    redraw_mouse();
    swap_buffers();
}

void init_desktop() {
    proc_create((void*)update_desktop);
}