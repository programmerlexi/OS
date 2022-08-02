; note: code is untested
global jump_usermode
extern test_user_function
jump_usermode:
	mov ax, (4 * 8) | 3 ; user data segment with RPL 3
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax ; sysexit sets SS
 
	; setup wrmsr inputs
	xor edx, edx ; not necessary; set to 0
	mov eax, 0x100008 ; SS=0x10+0x10=0x20, CS=0x8+0x10=0x18
	mov ecx, 0x174 ; MSR specifier: IA32_SYSENTER_CS
	wrmsr ; set sysexit segments
 
	; setup sysexit inputs
	mov edx, test_user_function ; to be loaded into EIP
	mov ecx, esp ; to be loaded into ESP
	sysexit