extern void _syscall();
#include "regs.h"
#include "idt.h"
#include "syscall.h"
#include <stdint.h>
#include "../services/proc.h"

regs_t* disregard_regs(regs_t* r) {
    return r;
} // Just error handling. Make a call to this function with the regs pointer that you dont want to use.

void down(regs_t* r) {
    disregard_regs(r);
    power_shutdown();
}
void fork(regs_t* r) {
    fork((void(*)())r->ebx);
}
void exec(regs_t* r) {
    fork((void(*)())r->ebx);
    quit();
}
void exit(regs_t* r) {
    quit();
}
void time(regs_t* r) {
    read_rtc();
    r->eax = second;
    r->ebx = minute;
    r->ecx = hour;
}
void date(regs_t* r) {
    read_rtc();
    r->eax = day;
    r->ebx = month;
    r->ecx = year;
}
void sys_clear(regs_t* r) {
    disregard_regs(r);
    clear_screen();
}
void sys_undefined(regs_t* r) {
    disregard_regs(r);
}

void sys_init(){
    print_string("Initializing syscall handlers...                \r");
    idt_set_gate(0x80, (uint32_t)_syscall, 0x1 << 3, 0x8E);
}

void syscall_c(regs* r){
    switch(r->eax) {
        case 0x00: down(r); break;
        case 0x01: fork(r); break;
        case 0x02: exec(r); break;
        case 0x03: exit(r); break;
        case 0x04: time(r); break;
        case 0x05: date(r); break;
        case 0x0A: sys_clear(r); break;
        default: sys_undefined(r); break;
    }
}