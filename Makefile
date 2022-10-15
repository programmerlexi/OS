all: run clean

cleanObjs:
	rm -rf objs/*

cleanBins:
	rm -rf bins/*

cleanImgs:
	rm -rf images/*

clean: cleanObjs cleanBins

cleanAll: clean cleanImgs

objs/kernel.o:
	/usr/local/i386elfgcc/bin/i386-elf-g++ -ffreestanding -m32 -g -c kernel/kernel.cpp -o objs/kernel.o -mno-red-zone -fno-pic -fno-builtin -O1 -fpermissive

objs/kernel_entry.o:
	nasm -f elf kernel/kernel_entry.asm -o objs/kernel_entry.o

bins/full_kernel.bin: objs/kernel_entry.o objs/kernel.o
	/usr/local/i386elfgcc/bin/i386-elf-ld -m elf_i386 -o bins/full_kernel.bin -Ttext 0x0000C000 objs/kernel_entry.o objs/kernel.o --oformat binary

kernel: bins/full_kernel.bin

bins/boot.bin:
	nasm -f bin bootloader/boot.asm -o bins/boot.bin

objs/loader.o:
	nasm -f elf bootloader/stage2/loader.asm -o objs/loader.o

objs/stage2.o:
	/usr/local/i386elfgcc/bin/i386-elf-g++ -ffreestanding -m16 -g -c bootloader/stage2/stage2.cpp -o objs/stage2.o -mno-red-zone -O1 -fpermissive -fno-pic -fno-builtin

objs/stage2_5.o:
	/usr/local/i386elfgcc/bin/i386-elf-g++ -ffreestanding -m32 -g -c bootloader/stage2/stage2_32.cpp -o objs/stage2_5.o -mno-red-zone -O1 -fpermissive -fno-pic -fno-builtin

bins/stage2_content.bin: objs/loader.o objs/stage2.o objs/stage2_5.o
	/usr/local/i386elfgcc/bin/i386-elf-ld -m elf_i386 -o bins/stage2_content.bin -Ttext 0x00001000 objs/loader.o objs/stage2.o objs/stage2_5.o --oformat binary

bins/stage2_base.bin:
	nasm -f bin bootloader/stage2/stage2_base.asm -o bins/stage2_base.bin

bins/stage2.bin: bins/stage2_base.bin bins/stage2_content.bin
	dd if=bins/stage2_base.bin of=bins/stage2.bin
	dd if=bins/stage2_content.bin of=bins/stage2.bin conv=notrunc

bins/zeroes.bin:
	nasm bootloader/zeroes.asm -f bin -o bins/zeroes.bin

bootloader: bins/boot.bin bins/stage2.bin bins/zeroes.bin

image: bootloader kernel
	cat bins/boot.bin bins/stage2.bin bins/full_kernel.bin bins/zeroes.bin  > images/OS.bin

iso: image
	cat images/OS.bin > images/OS.iso

run: image
	qemu-system-i386 images/OS.bin -m 4G -soundhw pcspk

kvm: image
	kvm-spice images/OS.bin -soundhw pcspk
#096M



