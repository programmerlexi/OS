#include "bki.h"
#include "gatr/vga.h"
#include "gatr/bitmap.h"
#include "utils/util.h"
#include "utils/math.h"
#include "utils/random.h"

bool system = true;
bool usermode = false;
bool is_system() {
    return system;
}
bool is_usermode() {
    return usermode;
}

#include "memory/memory.h"
#include "memory/heap.h"
#include "memory/sysmem.h"
#include "paging/allocator.h"
#include "paging/paging.h"

#include "services/proc.h"
#include "services/debug.h"

#include "power/shutdown.h"

extern "C" { // This is so we can import asm stuff from kernel_entry.asm
#include "interrupts/irq.h"
#include "interrupts/isr.h"
#include "interrupts/idt.h"
#include "interrupts/syscall.h"
#include "services/timer.h"
#include "services/keyboard.h"
#include "services/scancodeTranslator.h"
#include "services/mouse.h"
#include "ring3/umode.h"
}
#include "disk/disk.h"
#include "ring3/tss.h"
#include "segmentation/gdt.h"

#include "sound/pcspeaker.h"
#include "services/rtc.h"
#include "fs/fs.h"
#include "fs/devfs/devfs.h"
#include "fs/rootfs/rootfs.h"
#include "proc/task.h"

void kernel_test();

void panic_char(char c, int idx)
{
	((char*)0xB8000)[idx*2] = c;
	((char*)0xB8000)[idx*2+1] = 0x9f; // White on Ligth blue
}

void panic_print(const char* message, int idx)
{
	int i = 0;
	while (message[i] != '\0')
	{
		panic_char(message[i], idx+i);
		i++;
	}
}

void panic_clear() {
	for (int j = 0; j < 25; j++) {
		for (int i = 0; i < 80; i++) {
			panic_char('\0', i+j*80);
		}
	}
}

void panic_cursor_off() {
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void panic_dump_regs(int idx, regs_t* r) {
	panic_print("Register Dump:", idx);
	panic_print("EAX:", idx+84);
	panic_print(HexToString(r->eax), idx+88);
	panic_print("EBX:", idx+164);
	panic_print(HexToString(r->ebx), idx+168);
	panic_print("ECX:", idx+244);
	panic_print(HexToString(r->ecx), idx+248);
	panic_print("EDX:", idx+324);
	panic_print(HexToString(r->edx), idx+328);

	panic_print("ESP:", idx+484);
	panic_print(HexToString(r->esp), idx+488);
	panic_print("EBP:", idx+564);
	panic_print(HexToString(r->ebp), idx+568);

	panic_print("EIP:", idx+724);
	panic_print(HexToString(r->eip), idx+728);

    panic_print("ERROR CODE: ", idx+804);
    panic_print(HexToString(r->err_code), idx+815);
}

void panic_color(char color, int idx) {
	((char*)0xB8000)[idx*2+1] = color;
}

void panic_bitmap(int idx, int* bitmap, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		int pix = bitmap[i];
		int x = idx+i*80+cols;
		for (int j = 0; j < cols; j++) {
			if (pix&1) {
				panic_color(0xff,x-j);
			} else {
                panic_color(0x99,x-j);
            }
			pix >>= 1;
		}
	}
}

int smiley[4] = {
	0b10000001,
	0b00000000,
	0b01111110,
	0b10000001
};
int smiley_width = 8;
int smiley_heigth = 4;
void kpanic(const char* message,regs_t* r) {
    asm("cli");
    switch_to_text_mode();
	panic_clear();
	panic_print("Error: ", 20);
	panic_print(message, 27);
	panic_dump_regs(100, r);
	panic_bitmap(325,smiley,smiley_heigth,smiley_width);
	panic_print(" If you encounter this error",1200+80*6);
	panic_print(" and you did not modify the code,",1280+80*6);
	panic_print(" please report it to the author.",1360+80*6);
	panic_print("  The System will reboot  ",522);
	panic_print("     in a few moments.    ",602);
	//panic_print("       reboot it manually.    ",682);
	panic_print("Debug Scope: ", 1120+80*2+45);
	panic_print(get_debug_location(),1120+80*2+45+13);
	panic_cursor_off();
    for (uint32_t i = 0; i < 0x4fffffff;i++) {
        asm("nop");
    } // kpanic is useful for debugging too
    power_restart();
    asm("hlt");
	for (;;);
}

void test_vga() {
    clear_screen();
    print_string("VGA Test\n\r");
}

void test_memory() {
    void* t1 = malloc(0x10);
    void* t2 = malloc(0x10);
    void* t3 = malloc(0x10);

    const char* str = "0";

    print_string("\n\rt1: ");
    str = HexToString((uint64_t)t1);
    print_string(str);

    print_string("\n\rt2: ");
    str = HexToString((uint64_t)t2);
    print_string(str);

    print_string("\n\rt3: ");
    str = HexToString((uint64_t)t3);
    print_string(str);

    free(t1);
    free(t2);
    free(t3);

    void* t4 = malloc(0x60);
    
    print_string("\n\rt4: ");
    str = HexToString((uint64_t)t4);
    print_string(str);

    free(t4);

    print_string("\n\n\r");
}

void test_color() {
    for (int i=0; i<2000; i++) {
        putchar(' ', color(i&0xf,i&0xf), index(i));
    }
}

void kernel_loop() {
    print_string("Loop!\n\r");
}

void test_proc() {
    print_string("Testing proc\n\r");
}

extern "C" {
    void test_user_function() {
        for (;;);
    }
}

int robot_bmp[8] = {
    0b1111111111111,
    0b1000000000001,
    0b1011100011101,
    0b1011100011101,
    0b1000000000001,
    0b1011111111101,
    0b1000000000001,
    0b1111111111111
};

uint64_t zones[100];
void memory_self_test() {
    void* ptr = malloc(1);
    int addr = (int)ptr;
    free(ptr);
    ptr = malloc(1);
    if (((int)ptr)!=addr) {
        regs_t* r = get_regs();
        r->eax = addr;
        r->ebx = (int)ptr;
        r->ecx = sizeof(heap_segment_header);
        r->edx = count_segments();
        r->esp = (int)first_free_segment;
        memcpy(smiley,robot_bmp,8*sizeof(int));
        smiley_heigth = 8;
        smiley_width = 13;
        kpanic("Memory freeing failed",r);
    }
    free(ptr);
    for (int i = 0; i < 100; i++) {
        zones[i] = (uint64_t)malloc(1);
    }
    for (int i = 0; i < 100; i++) {
        if (!(i % 2)) {
            free((void*)zones[i]);
        }
    }
    for (int i = 0; i < 100; i++) {
        if ((i & 1)) {
            free((void*)zones[i]);
        }
    }
    ptr = malloc(100);
    if (((int)ptr)!=addr) {
        regs_t* r = get_regs();
        r->eax = (int)ptr;
        memcpy(smiley,robot_bmp,8*sizeof(int));
        smiley_heigth = 8;
        smiley_width = 13;
        kpanic("Memory combination failed",r);
    }
    free(ptr);
}

void timer_self_test() {
    timer_ticks = 0;
    int i = 0xffffff;
    while (timer_ticks == 0) {
        asm("nop");
        i--;
        if (i <= 0) {
            regs_t* r = get_regs();
            r->eax = timer_ticks;
            memcpy(smiley,robot_bmp,8*sizeof(int));
            smiley_heigth = 8;
            smiley_width = 13;
            kpanic("Timer is not responding after 0xfffffff probes",r);
        }
    }
}

#include "cpuid.cpp"
#include "kernel_shell.cpp"

void verify_kernel();

void kernel_init() {
    enter_debug_scope((char*)"kernel_init");
    init_gdt(); // Initialize the GDT
    init_heap(0x100000, 0x300000);
    memory_self_test(); // Run memory self-test.
    init_vga(); // Initialize VGA
    vga_graphics::init_graphics();
    if (get_boot_info()->splash_screen) {
        /*vga_graphics::draw_rect(124,74,52,52,LIGHT_BLUE);
        for (int i = 0; i < 2; i++) {
            vga_graphics::draw_rect(124+((i+1)*16)+(i*2),74,2,52,BLACK);
            vga_graphics::draw_rect(124,74+((i+1)*16)+(i*2),52,2,BLACK);
        }*/
        vga_graphics::draw_rect(274,194,92,92,0b1001);
        for (int i = 0; i < 2; i++) {
            vga_graphics::draw_rect(274+((i+1)*28)+(i*4),194,4,92,BLACK);
            vga_graphics::draw_rect(274,194+((i+1)*28)+(i*4),92,4,BLACK);
        }
    } else {
        switch_to_text_mode();
    }
    verify_kernel();
    print_string("Creating Filesystem\n\r");
    init_fs();

    asm volatile("cli"); // Disable interrupts
    print_string("Initializing interrupts!\r");
    idt_install(); // Create the IDT
    print_string("[OK] Interrupts initialized!\n\r");
    print_string("Installing ISRs!\r");
    isrs_install(); // Install the ISRs into the IDT
    print_string("[OK] ISRs installed!     \n\r");
    print_string("Installing syscalls!\r");
    irq_install(); // Install the IRQs into the IDT
    asm volatile("sti"); // Enable interrupt
    sys_init(); // Initialize the syscall handler
    print_string("[OK] Syscalls initialized!      \n\r");
    print_string("Installing timer!\r");
    timer_install(); // Install the timer handler
    print_string("[OK] Timer installed!\n\r");
    print_string("Installing keyboard!\r");
    kb_install(); // Install the keyboard handler
    print_string("[OK] Keyboard installed!\n\r");
    print_string("Performing Timer Self Test!\n\r");
    timer_self_test();
    print_string("Identifying Master ATA.\n\r");
    master_rdy = identify_ata(0xA0);
    print_string("Identifying Slave ATA.\n\r");
    slave_rdy = identify_ata(0xB0);
    print_string("Initialization complete!\n\r");
    print_string("Welcome to the Kernel!\n\r");
    beep(); // Beep aka the sound test
    if (get_boot_info()->splash_screen) {
        switch_to_text_mode();
    }
    initTasking();
    tasking = true;
    exit_debug_scope();
}

void kernel_test() {
    enter_debug_scope((char*)"kernel_test");
    test_vga(); // Test the vga driver in general
    test_memory(); // Test the memory manager
    exit_debug_scope();
}

void kernel_main() {
    enter_debug_scope((char*)"kernel_main");
    print_time(); // Print the time
    print_string("\n\r");
    print_string("Running CPU detection!\n\r");
    cpu_detect(); // Detect the CPU
    print_string("Lower Memory: ");
    uint16_t lower_memory = get_lower_memory();
    print_string(HexToString(lower_memory));
    print_string(" KB\n\r");
    print_string("Upper Memory: ");
    uint32_t upper_memory = get_upper_memory();
    print_string(HexToString(upper_memory));
    print_string(" KB\n\r");
    print_string("Total Memory: ");
    uint32_t total_memory = get_total_memory();
    print_string(HexToString(total_memory));
    print_string(" KB\n\r");
    print_string("Disk: 0x");
    uint8_t disk = *((uint8_t*)0x5004);
    print_string(HexToString(disk));
    if (disk & 0x80) {
        print_string(" (HDD)\n\r");
    } else {
        print_string(" (Floppy or Unknown)\n\r");
    }
    print_string("Master Drive Ready: ");
    print_string(master_rdy ? "Yes\n\r" : "No\n\r");
    print_string("Slave Drive Ready: ");
    print_string(slave_rdy ? "Yes\n\r" : "No\n\r");
    shmcreat(1234,18);
    char* c = (char*)shmget(1234);
    strcpy(c,"Shared memory test",18);
    fork(terminal_init);
    exit_debug_scope();
}

extern "C" void test_syscalls();

extern "C" void main(){
    kernel_init(); // Initialize the kernel

    //kernel_test(); // Test the kernel

    kernel_main(); // Run the kernel

    test_syscalls();

    yield(WAITING);
    while (true) { // We should not reach this
        asm("hlt"); // Only do stuff when an interrupt occurs.
    }
    return;
}
#include "klib.cpp"
void verify_kernel(void) {
    print_string("Verified that the kernel is fully loaded!\n\r");
}