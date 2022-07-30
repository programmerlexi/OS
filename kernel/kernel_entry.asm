section .text
[bits 32]
[extern main]
call main
jmp $

%include "kernel/asm/gdt.asm"
%include "kernel/asm/interrupts.asm"
%include "kernel/asm/paging.asm"
%include "kernel/ring3/tss.asm"
%include "kernel/ring3/umode.asm"