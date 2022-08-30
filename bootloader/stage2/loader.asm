bits 16
KERNEL_LOCATION equ 0x00007ef0 ; we dont need stage 1 anyway
stage2:
    xor ax, ax                          
    mov es, ax
    mov ds, ax
    mov bp, 0x7c00 ; Does the stack work with out a base address? YES IT DOES!
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
    mov dh, 66

    mov ah, 0x02
    mov al, dh 
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x06
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
    mov ah, 0x0
    mov al, 0x14
    int 0x10
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


stage2_string: db "Reached Stage 2!", 0xD, 0xA, 0
memory: db "Running memory detection!", 0xD, 0xA, 0
finished_memory: db "Finished memory detection!", 0xD, 0xA, 0
failed_disk: db "Failed to load Kernel!", 0
failed_mem: db "Failed to detect memory!", 0
a20_not_supported:
a20_activation_failed: db "A20 failed!", 0
a20_enabled: db "A20 enabled!", 0xD, 0xA, 0
good: db "Success!", 0xD, 0xA, 0


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
start_protected_mode:
    ; reset the segments
    mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
    
    ; set up the stack
	mov ebp, 0xf0000 ; maximum stack size
	mov esp, ebp
    
    ; print an "A" to indicate that we have reached Protected Mode
    mov ah, 0x0f ; bg=black fg=white
    mov al, 'A'
    mov [0xb8000], ax
    jmp KERNEL_LOCATION
    jmp $