#include "isr.h"
#include "../services/debug.h"

void isrs_install()
{
	print_string("Setting IDT Gate 0...\r");
	idt_set_gate(0, (unsigned)_isr0, 0x08, 0x8E);
	print_string("Setting IDT Gate 1...\r");
	idt_set_gate(1, (unsigned)_isr1, 0x08, 0x8E);
	print_string("Setting IDT Gate 2...\r");
	idt_set_gate(2, (unsigned)_isr2, 0x08, 0x8E);
	print_string("Setting IDT Gate 3...\r");
	idt_set_gate(3, (unsigned)_isr3, 0x08, 0x8E);
	print_string("Setting IDT Gate 4...\r");
	idt_set_gate(4, (unsigned)_isr4, 0x08, 0x8E);
	print_string("Setting IDT Gate 5...\r");
	idt_set_gate(5, (unsigned)_isr5, 0x08, 0x8E);
	print_string("Setting IDT Gate 6...\r");
	idt_set_gate(6, (unsigned)_isr6, 0x08, 0x8E);
	print_string("Setting IDT Gate 7...\r");
	idt_set_gate(7, (unsigned)_isr7, 0x08, 0x8E);
	print_string("Setting IDT Gate 8...\r");
	idt_set_gate(8, (unsigned)_isr8, 0x08, 0x8E);
	print_string("Setting IDT Gate 9...\r");
	idt_set_gate(9, (unsigned)_isr9, 0x08, 0x8E);
	print_string("Setting IDT Gate 10...\r");
	idt_set_gate(10, (unsigned)_isr10, 0x08, 0x8E);
	print_string("Setting IDT Gate 11...\r");
	idt_set_gate(11, (unsigned)_isr11, 0x08, 0x8E);
	print_string("Setting IDT Gate 12...\r");
	idt_set_gate(12, (unsigned)_isr12, 0x08, 0x8E);
	print_string("Setting IDT Gate 13...\r");
	idt_set_gate(13, (unsigned)_isr13, 0x08, 0x8E);
	print_string("Setting IDT Gate 14...\r");
	idt_set_gate(14, (unsigned)_isr14, 0x08, 0x8E);
	print_string("Setting IDT Gate 15...\r");
	idt_set_gate(15, (unsigned)_isr15, 0x08, 0x8E);
	print_string("Setting IDT Gate 16...\r");
	idt_set_gate(16, (unsigned)_isr16, 0x08, 0x8E);
	print_string("Setting IDT Gate 17...\r");
	idt_set_gate(17, (unsigned)_isr17, 0x08, 0x8E);
	print_string("Setting IDT Gate 18...\r");
	idt_set_gate(18, (unsigned)_isr18, 0x08, 0x8E);
	print_string("Setting reserved gates...\r");
	idt_set_gate(19, (unsigned)_isr19, 0x08, 0x8E);
	idt_set_gate(20, (unsigned)_isr20, 0x08, 0x8E);
	idt_set_gate(21, (unsigned)_isr21, 0x08, 0x8E);
	idt_set_gate(22, (unsigned)_isr22, 0x08, 0x8E);
	idt_set_gate(23, (unsigned)_isr23, 0x08, 0x8E);
	idt_set_gate(24, (unsigned)_isr24, 0x08, 0x8E);
	idt_set_gate(25, (unsigned)_isr25, 0x08, 0x8E);
	idt_set_gate(26, (unsigned)_isr26, 0x08, 0x8E);
	idt_set_gate(27, (unsigned)_isr27, 0x08, 0x8E);
	idt_set_gate(28, (unsigned)_isr28, 0x08, 0x8E);
	idt_set_gate(29, (unsigned)_isr29, 0x08, 0x8E);
	idt_set_gate(30, (unsigned)_isr30, 0x08, 0x8E);
	idt_set_gate(31, (unsigned)_isr31, 0x08, 0x8E);

}


const char* exception_messages[] = 
{
	"Division By Zero Exeption.",
	"Debug Exeption.",
	"Non Maskable Interrupt Exeption.",
	"Breakpoint Exeption.",
	"Into Detected Overflow Exeption.",
	"Out of Bounds Exeption.",
	"Invalid Opcode Exeption.",
	"No Coprocessor Exeption.",
	"Double Fault Exeption.",
	"Coprocessor Segment Overrun Exeption.",
	"Bad TSS Exeption.",
	"Segment Not Present Exeption.",
	"Stack Fault Exeption.",
	"General Protection Fault Exeption.",
	"Page Fault Exeption.",
	"Unknown Interrupt Exeption.",
	"Coprocessor Fault Exeption.",
	"Alignment Check Exeption.",
	"Machine Check Exeption.",
	"Reserved Exeption.",
	"Reserved Exeption.",
	"Reserved Exeption.",
	"Reserved Exeption.",
	"Reserved Exeption.",
	"Reserved Exeption.",
	"Reserved Exeption.",
	"Reserved Exeption.",
	"Reserved Exeption.",
	"Reserved Exeption.",
	"Reserved Exeption.",
	"Reserved Exeption.",
	"Reserved Exeption."
};

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

void panic_dump_regs(int idx, regs* r) {
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
}

void panic_color(char color, int idx) {
	((char*)0xB8000)[idx*2+1] = color;
}

void panic_bitmap(int idx, int* bitmap, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		char pix = bitmap[i];
		int x = idx+i*80+cols;
		for (int j = 0; j < cols; j++) {
			if (pix&1) {
				panic_color(0xff,x-j);
			}
			pix >>= 1;
		}
	}
}

int smiley[5] = {
	0b10000001,
	0b00000000,
	0b01111110,
	0b10000001
};

extern "C" void _fault_handler(struct regs *r)
{
    if (r->int_no < 32)
    {
		asm("cli");
		panic_clear();
		panic_print("Error: ", 20);
		panic_print(exception_messages[r->int_no], 27);
		panic_dump_regs(100, r);
		panic_bitmap(325,smiley,4,8);
		panic_print(" If you encounter this error",1200+80*6);
		panic_print(" and you did not modify the code,",1280+80*6);
		panic_print(" please report it to the author.",1360+80*6);
		panic_print("   The System will now freeze ",522);
		panic_print("      and you will have to    ",602);
		panic_print("       reboot it manually.    ",682);
		panic_print("Debug Scope: ", 1120+80*2+45);
		panic_print(get_debug_location(),1120+80*2+45+13);
		panic_cursor_off();
		asm("hlt");
		for (;;);
 	}
}
