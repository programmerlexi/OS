section .text
[bits 32]
[extern main]
call main
jmp $

proc_one:
    mov eax, 0x02
    mov ebx, proc_two
    int 0x80
    ret

proc_two:
    mov eax, 0x03
    int 0x80
    ret

[global test_syscalls]
test_syscalls:
    mov eax, 0x01
    mov ebx, proc_one
    int 0x80
    ret

%include "kernel/asm/gdt.asm"
%include "kernel/asm/interrupts.asm"
%include "kernel/asm/paging.asm"
%include "kernel/asm/task.asm"
%include "kernel/ring3/tss.asm"
%include "kernel/ring3/umode.asm"