#include "../../../gatr/vga.h"

long pads_y;
long ball_x;
long ball_y;
long ball_vx;
long ball_vy;
long score;
bool game_running = false;

void init_game() {
    pads_y = 80;
    ball_x = 155;
    ball_y = 95;
    ball_vx = 5;
    ball_vy = -2;
    score = 0;
    game_running = true;
}

void pong() {
    vga_graphics::set_mode(1);
    while (kpressed[0x1c]) asm("nop");
    while (true) {
        clear_screen();
        print_bitmap_string("Pong",160-16,50,WHITE);
        print_bitmap_string("Press ENTER to start.",160-88,130,WHITE);
        vga_graphics::swap();
        while(!kpressed[0x1c]) asm("nop");
        init_game();
        while(game_running) {
            ball_x += ball_vx;
            ball_y += ball_vy;

            if (kpressed[0x48]) pads_y -= 4;
            if (kpressed[0x50]) pads_y += 4;

            if (pads_y < 4) pads_y = 4;
            if (pads_y > 156) pads_y = 156;

            if (ball_y < 4) ball_vy = 2;
            if (ball_y > 192) ball_vy = -2;

            if (ball_x > 310 || ball_x < 6) {
                if ((ball_y+4) > pads_y) {
                    if (ball_y < (pads_y+40)) {
                        ball_vx = -ball_vx;
                        score++;
                    }
                }
            }

            if (ball_x < 0) { ball_x -= ball_vx; ball_y -= ball_vy; game_running = false; }
            if (ball_x > 320) { ball_x -= ball_vx; ball_y -= ball_vy; game_running = false; }

            clear_screen();
            vga_graphics::draw_rect(159,0,2,200,WHITE);
            vga_graphics::draw_rect(ball_x,ball_y,4,4,LIGHT_GREEN);
            vga_graphics::draw_rect(1,pads_y,5,40,WHITE);
            vga_graphics::draw_rect(314,pads_y,5,40,WHITE);
            vga_graphics::swap();
            sleep(1);
        }
        print_bitmap_string("GAME OVER", 160-36,10,LIGHT_RED);
        print_bitmap_string("Score: ", 160-56,40,WHITE);
        print_bitmap_string(HexToString(score),160-8,40,WHITE);
        vga_graphics::swap();
        sleep(5000);
    }
}