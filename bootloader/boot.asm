[org 0x7c00]
; Bootloader
main:
    KERNEL_LOCATION equ 0x00007EF0
    mov [BOOT_DISK], dl
    xor ax, ax                          
    mov es, ax
    mov ds, ax
    mov bp, 0x7c00
    mov sp, bp
    
    call memory_detection
    call upper_memory_detection
    pusha
    mov al, [BOOT_DISK]
    mov [0x5004], al
    popa

    mov bx, KERNEL_LOCATION
    mov dh, 67

    mov ah, 0x02
    mov al, dh 
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02
    mov dl, [BOOT_DISK]
    int 0x13
    jnc after_load
    mov ah, 0x0e
    mov bx, failed
    call print_string
    jmp $
memory_detection:
    clc
    int 0x12
    jnc after_memory_detection
    mov ah, 0x0e
    mov bx, failed
    call print_string
    jmp $
after_memory_detection:
    mov [0x5000], ax
    ret
upper_memory_detection:
    ;detect upper memory.
    mov ax, 0xE801
    int 0x15
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
after_load:
    mov ah, 0x0e
    mov bx, good
    call print_string
    
    mov ah, 0x0
    mov al, 0x14
    int 0x10

    call enableA20
    
    cli
    lgdt [GDT_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEG:start_protected_mode
    jmp $

enableA20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

print_string:
    mov al, [bx]
    cmp al, 0
    je end
    int 0x10
    inc bx
    jmp print_string
end:
    ret
jmp $

BOOT_DISK: db 0
label:
    db "Hello, world!", 0

good:
    db "Sucess!", 0xD, 0xA, 0

failed:
    db "Failed to load Kernel from Disk!", 0

CODE_SEG equ GDT_code - GDT_start
DATA_SEG equ GDT_data - GDT_start

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

[bits 32]
start_protected_mode:
    mov al, 'P'
    mov ah, 0x0f
    mov [0xb8000], ax
    mov al, 'M'
    mov ah, 0x0f
    mov [0xb8002], ax
    jmp KERNEL_LOCATION
    jmp $
times 510-($-$$) db 0
dw 0xaa55