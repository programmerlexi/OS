all: run clean

cleanObjs:
	echo "Cleaning Object Files"
	rm -rf objs/*

cleanBins:
	echo "Cleaning Binaries"
	rm -rf bins/*

cleanImgs:
	echo "Cleaning Images"
	rm -rf images/*

clean: cleanObjs cleanBins

cleanAll: clean cleanImgs

setup:
	echo "Setting up enviroment"
	export PATH=$(PATH):/usr/local/i386elfgcc/bin

kernel: setup
	echo "Building Kernel"
	nasm -f elf kernel/kernel_entry.asm -o objs/kernel_entry.o
	/usr/local/i386elfgcc/bin/i386-elf-g++ -ffreestanding -m32 -g -c kernel/kernel.cpp -o objs/kernel.o -mno-red-zone -O1 -fpermissive
	/usr/local/i386elfgcc/bin/i386-elf-ld -m elf_i386 -o bins/full_kernel.bin -Ttext 0x00007ef0 objs/kernel_entry.o objs/kernel.o --oformat binary

bootloader: setup
	echo "Building Bootloader"
	nasm -f bin bootloader/boot.asm -o bins/boot.bin
	nasm -f elf bootloader/stage2/loader.asm -o objs/loader.o
	/usr/local/i386elfgcc/bin/i386-elf-g++ -ffreestanding -m16 -g -c bootloader/stage2/stage2.cpp -o objs/stage2.o -mno-red-zone -O1 -fpermissive
	/usr/local/i386elfgcc/bin/i386-elf-ld -m elf_i386 -o bins/stage2_content.bin -Ttext 0x00001000 objs/loader.o objs/stage2.o --oformat binary
	nasm -f bin bootloader/stage2/stage2_base.asm -o bins/stage2_base.bin
	dd if=bins/stage2_base.bin of=bins/stage2.bin
	dd if=bins/stage2_content.bin of=bins/stage2.bin conv=notrunc
	nasm bootloader/zeroes.asm -f bin -o bins/zeroes.bin

image: bootloader kernel setup
	echo "Generating image"
	cat bins/boot.bin bins/stage2.bin bins/full_kernel.bin bins/zeroes.bin  > images/OS.bin

iso: image
	echo "Generating ISO"
	cat images/OS.bin > images/OS.iso

run: image setup
	echo "Launching"
	qemu-system-i386 images/OS.bin -m 4G

kvm: image setup
	echo "Launching KVM"
	kvm-spice images/OS.bin
#096M



