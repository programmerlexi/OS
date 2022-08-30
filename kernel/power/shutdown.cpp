#include "shutdown.h"

void power_shutdown() {
    clear_screen();
    print_string("Shutting down...\n\r");
    asm("cli");
    outw((uint16_t)0x604, (uint16_t)0x2000);
    asm("hlt");
}

void power_restart() {
    uint8_t temp;
    asm ("cli");
    do
    {
        temp = inb(0x64);
        if (((temp) & (1)) != 0)
            inb(0x60);
    } while (((temp) & (1<<1)) != 0);
 
    outb(0x64, 0xFE);
    while (1) {
        asm("hlt");
    }
}