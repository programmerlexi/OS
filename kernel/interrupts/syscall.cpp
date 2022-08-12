extern void _syscall();
#include "regs.h"
#include "idt.h"
#include "syscall.h"
#include <stdint.h>
#include "../services/proc.h"

void down(regs* r) {
    power_shutdown();
}
void fork(regs* r) {
    proc_fork(proc_list[cur_pid].loop, proc_list[cur_pid].name);
}
void exec(regs* r) {
    proc_quit(0);
    proc_create((void*)r->eax, (char*)r->ebx);
}
void exit(regs* r) {
    proc_quit(r->eax);
}
void time(regs* r) {
    read_rtc();
    r->eax = second;
    r->ebx = minute;
    r->ecx = hour;
}
void date(regs* r) {
    read_rtc();
    r->eax = day;
    r->ebx = month;
    r->ecx = year;
}
void sys_6(regs* r) {
}
void sys_7(regs* r) {
}
void sys_8(regs* r) {
}
void sys_9(regs* r) {
}
void sys_10(regs* r) {
    clear_screen();
}
void sys_11(regs* r) {
}
void sys_12(regs* r) {
}
void sys_13(regs* r) {
}
void sys_14(regs* r) {
}
void sys_15(regs* r) {
}
void sys_16(regs* r) {
}
void sys_17(regs* r) {
}

void sys_undefined(regs* r) {
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
        case 0x06: sys_6(r); break;
        case 0x07: sys_7(r); break;
        case 0x08: sys_8(r); break;
        case 0x09: sys_9(r); break;
        case 0x0A: sys_10(r); break;
        case 0x0B: sys_11(r); break;
        case 0x0C: sys_12(r); break;
        case 0x0D: sys_13(r); break;
        case 0x0E: sys_14(r); break;
        case 0x0F: sys_15(r); break;
        case 0x10: sys_16(r); break;
        case 0x11: sys_17(r); break;
        default: sys_undefined(r); break;
    }
}