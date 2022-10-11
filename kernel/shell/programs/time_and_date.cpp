#include "../../interrupts/regs.h"
#include "../../gatr/vga.h"
void time() {
    read_rtc();
    print_string(num_to_char(hour));
    print_string(":");
    print_string(num_to_char(minute));
    print_string(":");
    print_string(num_to_char(second));
    print_string("\n\r");
}
void date() {
    read_rtc();
    print_string(num_to_char(day));
    print_string("/");
    print_string(num_to_char(month));
    print_string("/");
    print_string(num_to_char(year));
    print_string("\n\r");
}