#include "regs.h"
void fill_reg(struct regs *reg) {
    uint32_t eaxres, ebxres, ecxres, edxres;
    uint32_t espres, ebpres, eipres, esires;
    asm volatile("movl\t%0, %%eax" : "=m" (eaxres));
    asm volatile("movl\t%0, %%ebx" : "=m" (ebxres));
    asm volatile("movl\t%0, %%ecx" : "=m" (ecxres));
    asm volatile("movl\t%0, %%edx" : "=m" (edxres)); 
    asm volatile("movl\t%0, %%esp" : "=m" (espres));
    asm volatile("movl\t%0, %%ebp" : "=m" (ebpres));
    asm volatile("movl\t%0, %%esi" : "=m" (esires));
    asm volatile("movl\t%0, %%eip" : "=m" (eipres));
    reg->eax = eaxres;
    reg->ebx = ebxres;
    reg->ecx = ecxres;
    reg->edx = edxres;
    reg->esp = espres;
    reg->ebp = ebpres;
    reg->esi = esires;
    reg->eip = eipres;
}