[global jump_usermode]
[extern enter_umode]
[extern set_kernel_stack]
jump_usermode:
    push esp
    call set_kernel_stack
	mov ax, 0x20 | 3 ; ring 3 data with bottom 2 bits set for ring 3
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax ; SS is handled by iret
	; set up the stack frame iret expects
	mov eax, esp
	push 0x20 | 3 ; data selector
	push eax ; current esp
	pushf ; eflags
	push 0x18 | 3 ; code selector (ring 3 code with bottom 2 bits set for ring 3)
	push enter_umode ; instruction address to return to
	iret