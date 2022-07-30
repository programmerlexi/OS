#include "shutdown.h"

void power_shutdown() {
    clear_screen();
    print_string("Shutting down...\n\r");
    asm("cli");
    outw((uint16_t)0x604, (uint16_t)0x2000);
    asm("hlt");
}