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
	/usr/local/i386elfgcc/bin/i386-elf-ld -m elf_i386 -o bins/full_kernel.bin -Ttext 0x00007EF0 objs/kernel_entry.o objs/kernel.o --oformat binary

bootloader: setup
	echo "Building Bootloader"
	nasm -f bin bootloader/boot.asm -o bins/boot.bin
	nasm bootloader/zeroes.asm -f bin -o bins/zeroes.bin

image: bootloader kernel setup
	echo "Generating image"
#dd if=/dev/zero of=images/OS.img bs=512 count=2880
#mkfs.fat -F 12 -n "OS" images/OS.img
#dd if=bins/boot.bin of=images/OS.img conv=notrunc
#mcopy -i images/OS.img bins/full_kernel.bin "::kernel.bin"
	cat bins/boot.bin bins/full_kernel.bin bins/zeroes.bin  > images/OS.bin

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



