#include "bki.cpp"
#include "gatr/vga_text.cpp"
//#include "gatr/vga_graphics.cpp"
#include "utils/util.cpp"
#include "utils/math.cpp"
#include "utils/random.cpp"

bool system = true;
bool usermode = false;
bool is_system() {
    return system;
}
bool is_usermode() {
    return usermode;
}

//#include "paging/paging.cpp"
#include "memory/memory.cpp"
#include "memory/heap.cpp"
#include "memory/sysmem.cpp"

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
#include "paging/paging.cpp"
#include "ring3/umode.h"
//#include "services/pic.cpp"
}
#include "ring3/tss.cpp"
#include "segmentation/gdt.cpp"

//#include "disk/disk.cpp"
#include "sound/pcspeaker.cpp"
#include "services/rtc.cpp"

void kernel_test();



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

    void* t5 = calloc(0x10);

    print_string("\n\rt5: ");
    str = HexToString((uint64_t)t5);
    print_string(str);

    t5 = realloc(t5, 0x60);

    print_string("\n\rt5 (reallocated): ");
    str = HexToString((uint64_t)t5);
    print_string(str);

    free(t5);

    print_string("\n\n\r");

    /*void* t6 = alinged_alloc(0x4000, 0x10);
    
    print_string("\n\rt6: ");
    str = HexToString((uint64_t)t6);
    print_string(str);
    print_string("\n\r");

    free(t6);*/
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
        //print_string("Welcome to usermode!\n\r");
    }
}

//#include "usermode/desktop.cpp"

#include "cpuid.cpp"
#include "kernel_shell.cpp"

void kernel_init() {
    enter_debug_scope((char*)"kernel_init");
    init_paging(); // Initialize paging
    init_gdt(); // Initialize the GDT
    init_sysmem(0x100000-sizeof(sysmem_t), 0x200000); // Initialize the system memory
    //return_to_sysmem(); // Return to system memory
    //init_heap(0x100000, 0x200000); // Initialize heap at 0x100000 with a size of 2MB
    init_vga(); // Initialize VGA
    print_string("Initializing processes!\r");
    proc_init(); // Initialize processses
    print_string("[OK] Processes initialized!\n\r");
    asm volatile("cli"); // Disable interrupts
    print_string("Initializing interrupts!\r");
    idt_install(); // Create the IDT
    print_string("[OK] Interrupts initialized!\n\r");
    print_string("Installing IRQs!\r");
    isrs_install(); // Install the ISRs into the IDT
    print_string("[OK] IRQs installed!     \n\r");
    print_string("Installing syscalls!\r");
    irq_install(); // Install the IRQs into the IDT
    sys_init(); // Initialize the syscall handler
    print_string("[OK] Syscalls initialized!      \n\r");
    print_string("Installing timer!\r");
    timer_install(); // Install the timer handler
    print_string("[OK] Timer installed!\n\r");
    print_string("Installing keyboard!\r");
    kb_install(); // Install the keyboard handler
    print_string("[OK] Keyboard installed!\n\r");
    print_string("Installing mouse!\r");
    mouse_install(); // Install the mouse handler
    enable_mouse(); // Enable the mouse
    proc_create((void*)process_mouse_packet,"mouse"); // Create a process to handle the mouse packets
    print_string("[OK] Mouse installed!\n\r");
    asm volatile("sti"); // Enable interrupt
    //reset_vga_pos(); // Reset the VGA cursor position
    print_string("Initialization complete!\n\r");
    print_string("Welcome to the Kernel!\n\r");
    exit_debug_scope();
}

void kernel_test() {
    enter_debug_scope((char*)"kernel_test");
    //test_color(); // Test the vga colors

    test_vga(); // Test the vga driver in general
    
    test_memory(); // Test the memory manager
    exit_debug_scope();
}

void kernel_main() {
    enter_debug_scope((char*)"kernel_main");
    /*while(1) {
        terminal_loop(); // Run the terminal
    }*/
    //print_time(); // Print the time
    //print_string("\n\r");
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
    //print_string("\n\r");
    terminal_init(); // Initialize the terminal
    proc_create((void*)terminal_loop,"terminal"); // Create a process to run the terminal
    exit_debug_scope();
}

extern "C" void main(){
    kernel_init(); // Initialize the kernel

    //kernel_test(); // Test the kernel

    kernel_main(); // Run the kernel

    return;
}
