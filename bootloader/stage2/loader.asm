bits 16
KERNEL_LOCATION equ 0x00006000
MAPPED_KERNEL_LOCATION equ 0xC0000000
stage2:
    xor ax, ax                          
    mov es, ax
    mov ds, ax
    mov bp, 0x1000 ; Does the stack work with out a base address? YES IT DOES!
    mov sp, bp

    mov ah, 0x0e
    mov bx, stage2_string
    call print_string

    mov bx, memory
    call print_string

    call memory_detection
    call upper_memory_detection

    mov bx, finished_memory
    call print_string

    [extern loader_c]
    call loader_c

[global load_kernel_from_disk]
load_kernel_from_disk:
    mov bx, KERNEL_LOCATION
    mov dh, 90

    mov ah, 0x02
    mov al, dh 
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x0a
    mov dl, [0x5004]
    int 0x13
    jnc after_load
    mov ah, 0x0e
    mov bx, failed_disk
    call print_string
    jmp $
after_load:
    mov ah, 0x0e
    mov bx, good
    call print_string
    ret
[global load_gdt]
load_gdt:
    cli
    lgdt [GDT_descriptor]
    ret
[global set_protected_mode]
set_protected_mode:
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    ret
[global jump_to_protected_mode]
jump_to_protected_mode:
    jmp CODE_SEG:start_protected_mode
    jmp $
memory_detection:
    clc
    int 0x12
    jc failed_memory
after_memory_detection:
    mov [0x5000], ax
    ret
upper_memory_detection:
    ;detect upper memory.
    mov ax, 0xE801
    int 0x15
    jc failed_memory
after_upper_memory_detection:
    xor dx, dx
    mov cx, 1024
    div cx
    mov cx, ax
    xor dx, dx
    mov ax, bx
    mov bx, 64
    mul bx
    mov bx, 1024
    div bx
    add cx, ax
    mov ax, cx
    mov [0x5002], ax
    ret
failed_memory:
    mov ah, 0x0e
    mov bx, failed_mem
    call print_string
    jmp $



print_string:
    mov al, [bx]
    cmp al, 0
    je end
    int 0x10
    inc bx
    jmp print_string
end:
    ret
[global enableA20]
check_a20:
    pushf
    push ds
    push es
    push di
    push si
 
    cli
 
    xor ax, ax ; ax = 0
    mov es, ax
 
    not ax ; ax = 0xFFFF
    mov ds, ax
 
    mov di, 0x0500
    mov si, 0x0510
 
    mov al, byte [es:di]
    push ax
 
    mov al, byte [ds:si]
    push ax
 
    mov byte [es:di], 0x00
    mov byte [ds:si], 0xFF
 
    cmp byte [es:di], 0xFF
 
    pop ax
    mov byte [ds:si], al
 
    pop ax
    mov byte [es:di], al
 
    mov ax, 0
    je check_a20__exit
 
    mov ax, 1
 
check_a20__exit:
    pop si
    pop di
    pop es
    pop ds
    popf
 
    ret

enableA20:
    call A20check
a20_bios_func:
    mov     ax,2403h                ;--- A20-Gate Support ---
    int     15h
    jb      A20ns                  ;INT 15h is not supported
    cmp     ah,0
    jnz     A20ns                  ;INT 15h is not supported
    
    mov     ax,2402h                ;--- A20-Gate Status ---
    int     15h
    jb      A20failed              ;couldn't get status
    cmp     ah,0
    jnz     A20failed              ;couldn't get status
    
    cmp     al,1
    jz      A20done                 ;A20 is already activated
    
    mov     ax,2401h                ;--- A20-Gate Activate ---
    int     15h
    jb      A20failed              ;couldn't activate the gate
    cmp     ah,0
    jnz     A20failed              ;couldn't activate the gate
 
a20_activated:                  ;go on
    call A20check
a20_kbc:
    cli
 
    call    a20wait
    mov     al,0xAD
    out     0x64,al
 
    call    a20wait
    mov     al,0xD0
    out     0x64,al
 
    call    a20wait2
    in      al,0x60
    push    eax
 
    call    a20wait
    mov     al,0xD1
    out     0x64,al
 
    call    a20wait
    pop     eax
    or      al,2
    out     0x60,al
 
    call    a20wait
    mov     al,0xAE
    out     0x64,al
 
    call    a20wait
    sti
    jmp after_kbc
 
a20wait:
        in      al,0x64
        test    al,2
        jnz     a20wait
        ret
 
 
a20wait2:
        in      al,0x64
        test    al,1
        jz      a20wait2
        ret
after_kbc:
    call A20check
fastA20:
    in al, 0x92
    test al, 2
    jnz after
    or al, 2
    and al, 0xFE
    out 0x92, al
after:
    mov bx, 0xffff
chk_loop:
    call A20check
    sub bx, 1
    jnz chk_loop
    jmp A20failed
A20check:
    call check_a20
    test ax,ax
    jz .end  
.succes:
    call A20done
    pop ax
.end:
    ret
A20done:
    mov ah, 0x0e
    mov bx, a20_enabled
    call print_string
    ret
A20failed:
    mov ah, 0x0e
    mov bx, a20_activation_failed
    call print_string
    jmp $
A20ns:
    mov ah, 0x0e
    mov bx, a20_not_supported
    call print_string
    jmp $

[global EnableSSE]
EnableSSE:
    mov eax, 0x1
    cpuid
    test edx, 1<<25
    jz .noSSE
    mov eax, cr0
    and ax, 0xFFFB		;clear coprocessor emulation CR0.EM
    or ax, 0x2			;set coprocessor monitoring  CR0.MP
    mov cr0, eax
    mov eax, cr4
    or ax, 3 << 9		;set CR4.OSFXSR and CR4.OSXMMEXCPT at the same time
    mov cr4, eax
    ret
.noSSE:
    mov ah, 0x0e
    mov bx, sse_unavailable
    call print_string
    jmp $

[global enable_unreal]
enable_unreal:
    cli
    push ds
    lgdt [GDT_descriptor]
    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp $+2 ; avoid crashing the cpu (386 and 486)
    mov  bx, DATA_SEG
    mov  ds, bx
    and al,0xFE
    mov  cr0, eax
    pop ds
    sti
    ret

stage2_string: db "Reached Stage 2!", 0xD, 0xA, 0
memory: db "Running memory detection!", 0xD, 0xA, 0
finished_memory: db "Finished memory detection!", 0xD, 0xA, 0
failed_disk: db "Failed to load Kernel!", 0
failed_mem: db "Failed to detect memory!", 0
a20_not_supported:
a20_activation_failed: db "A20 failed!", 0
a20_enabled: db "A20 enabled!", 0xD, 0xA, 0
good: db "Success!", 0xD, 0xA, 0
sse_unavailable: db "SSE is unavailable!", 0


GDT_start:                          ; must be at the end of real mode code
    GDT_null:
        dd 0x0
        dd 0x0

    GDT_code:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10011010
        db 0b11001111
        db 0x0

    GDT_data:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010
        db 0b11001111
        db 0x0
GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start - 1
    dd GDT_start


CODE_SEG equ GDT_code - GDT_start
DATA_SEG equ GDT_data - GDT_start

[bits 32]
[global loadPageDirectory]
loadPageDirectory:
    push ebp
    mov ebp, esp
    mov eax, [esp+8]
    mov cr3, eax
    mov esp, ebp
    pop ebp
    ret

[global enablePaging]
    enablePaging:
    push ebp
    mov ebp, esp
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    mov esp, ebp
    pop ebp
    ret

start_protected_mode:
    ; reset the segments
    mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
    
    ; set up the stack
	mov ebp, 0x6000 ; maximum stack size
	mov esp, ebp
    
    [extern boot_bits32]
    call boot_bits32

    ; print a smiley to indicate that we have reached Protected Mode
    mov ax, 0x0f01
    mov [0xb8000], ax
    jmp MAPPED_KERNEL_LOCATION
    jmp $