[global jump_usermode]
[extern enter_umode]
[extern set_kernel_stack]
jump_usermode:
    cli
    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    push 0x23
    push 0x1000
    pushf
    push dword 0x1B
    push enter_umode
    mov ebx, 0x10
    iret