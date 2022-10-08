#include "bki.cpp"
#include "gatr/vga_text.cpp"
#include "gatr/vga_graphics.cpp"
#include "gatr/bitmap.cpp"
#include "utils/util.cpp"
#include "utils/math.cpp"
#include "utils/random.cpp"
#include "interrupts/regs.cpp"

#include "memory/memory.cpp"
#include "memory/heap.cpp"
#include "memory/sysmem.cpp"
#include "paging/allocator.cpp"
#include "paging/paging.cpp"

#include "services/proc.cpp"
#include "services/debug.cpp"

#include "power/shutdown.cpp"

extern "C" { // This is so we can import asm stuff from kernel_entry.asm
#include "interrupts/irq.cpp"
#include "interrupts/isr.cpp"
#include "interrupts/idt.cpp"
#include "interrupts/syscall.cpp"
#include "services/timer.cpp"
#include "services/keyboard.cpp"
#include "services/scancodeTranslator.cpp"
#include "services/mouse.cpp"
}
#include "disk/disk.cpp"
#include "ring3/tss.cpp"
#include "segmentation/gdt.cpp"

#include "sound/pcspeaker.cpp"
#include "services/rtc.cpp"
#include "fs/devfs/devfs.cpp"
#include "fs/rootfs/rootfs.cpp"
#include "fs/fs.cpp"

#include "proc/task.cpp"