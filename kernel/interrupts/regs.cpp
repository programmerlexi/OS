#include "regs.h"
void fill_reg(struct regs *reg) {
    uint32_t eaxres, ebxres, ecxres, edxres;
    uint32_t espres, ebpres, esires;
    asm("movl\t%%eax, %0" : "=m" (eaxres));
    asm("movl\t%%ebx, %0" : "=m" (ebxres));
    asm("movl\t%%ecx, %0" : "=m" (ecxres));
    asm("movl\t%%edx, %0" : "=m" (edxres));
    asm("movl\t%%esp, %0" : "=m" (espres));
    asm("movl\t%%ebp, %0" : "=m" (ebpres));
    asm("movl\t%%esi, %0" : "=m" (esires));
    reg->eax = eaxres;
    reg->ebx = ebxres;
    reg->ecx = ecxres;
    reg->edx = edxres;
    reg->esp = espres;
    reg->ebp = ebpres;
    reg->esi = esires;
}

regs_t* get_regs() {
    regs_t* regs = (regs_t*)0x2000;
    fill_reg(regs);
    return regs;
}