; ds = 0x7c00
org 0x7c00
bits 16

; FAT12 header
jmp short stage1
bpb_oem_identifier: db "MSWIN4.1" ; For maximum compatibility, use "MSWIN4.1"
bpb_bytes_per_sector: dw 512 ; 512 bytes per sector
bpb_sectors_per_cluster: db 1 ; 1 sector per cluster
bpb_reserved_sectors: dw 1 ; 1 reserved sector (we dont want files destroing our bootloader)
bpb_number_of_fats: db 2 ; 2 FATs (thats just the default)
bpb_root_dir_entries: dw 0x0e0 ; 0x0e0 entries in the root directory (1.44 MB)
bdb_media_descriptor_type: db 0xF0 ; Floppy
bpb_sectors_per_fat: dw 9
bpb_sectors_per_track: dw 18 ; 18 sectors per track
bpb_number_of_heads: dw 2 ; 2 heads
bpb_hidden_sectors: db 0 ; 0 hidden sectors
bpb_large_sectors: db 0 ; 0 large sectors
ebr_drive_number: db 0 ; Drive number
ebr_reserved: db 0 ; Reserved
ebr_signature: db 29h ; Signature
ebr_volume_id: db 12h, 34h, 56h, 78h ; Volume ID
ebr_volume_label: db "OS         " ; Volume label
ebr_file_system_type: db "FAT12   " ; File system type

stage1:
mov [ebr_drive_number], dl

cli
mov ax, 0  ; set up segments
mov ds, ax
mov es, ax
mov ss, ax     ; setup stack
mov bp, 0x7c00
mov sp, bp
sti

call load_next_sector ; load the first sector of the boot sector

load_next_sector:
    mov bx, stage2
    mov dh, 1
    mov ah, 0x02
    mov al, dh 
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02
    mov dl, [ebr_drive_number]
    int 0x13
    jnc .load_next_sector_done
    cli
    hlt
  .load_next_sector_done:
    ret
BOOT_DISK: db 0x00
times 510-($-$$) db 0
dw 0x0AA55
stage2: