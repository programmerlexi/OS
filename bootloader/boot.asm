[org 0x7c00]
[bits 16]
; Bootloader
main:
    STAGE2_LOCATION equ 0x00001000
    KERNEL_LOCATION equ 0x00007ef0
    mov [0x5004], dl
    xor ax, ax                          
    mov es, ax
    mov ds, ax
    mov bp, 0x7c00 ; Does the stack work with out a base address? YES IT DOES!
    mov sp, bp
    
    ;clear the screen
	mov ah, 0x00
	mov al, 0x2
	int 0x10

    mov bx, STAGE2_LOCATION
    mov dh, 7

    mov ah, 0x02
    mov al, dh 
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02
    mov dl, [0x5004]
    int 0x13

    jnc after_load_stage2
error:
    mov ah, 0x0e
    mov bx, failed_disk_stage2
    call print_string
    jmp $

after_load_stage2:
    mov ah, 0x0e
    mov bx, good
    call print_string
    
    mov ah, 0x0e
    mov bx, jumping
    call print_string

    jmp STAGE2_LOCATION

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

label: db "Hello, world!", 0
good: db "Success!", 0xD, 0xA, 0
failed_disk: db "Failed to load kernel!", 0
failed_disk_stage2: db "Failed to load Stage 2!", 0
jumping: db "Jumping to second stage!", 0xD, 0xA, 0

times 510-($-$$) db 0
dw 0xaa55