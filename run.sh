#!/bin/sh

export PATH=$PATH:/usr/local/i386elfgcc/bin

nasm -f bin bootloader/boot.asm -o bins/boot.bin
nasm -f elf kernel/kernel_entry.asm -o objs/kernel_entry.o
i386-elf-g++ -ffreestanding -m32 -g -c kernel/kernel.cpp -o objs/kernel.o -mno-red-zone -Wwrite-strings
nasm bootloader/zeroes.asm -f bin -o bins/zeroes.bin

i386-elf-ld -m elf_i386 -o bins/full_kernel.bin -Ttext 0x1000 objs/kernel_entry.o objs/kernel.o --oformat binary
cat bins/boot.bin bins/full_kernel.bin bins/zeroes.bin  > images/OS.bin

qemu-system-x86_64 images/OS.bin