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
	"Division By Zero Exception.",
	"Debug Exception.",
	"Non Maskable Interrupt Exception.",
	"Breakpoint Exception.",
	"Into Detected Overflow Exception.",
	"Out of Bounds Exception.",
	"Invalid Opcode Exception.",
	"No Coprocessor Exception.",
	"Double Fault Exception.",
	"Coprocessor Segment Overrun Exception.",
	"Bad TSS Exception.",
	"Segment Not Present Exception.",
	"Stack Fault Exception.",
	"General Protection Fault Exception.",
	"Page Fault Exception.",
	"Unknown Interrupt Exception.",
	"Coprocessor Fault Exception.",
	"Alignment Check Exception.",
	"Machine Check Exception.",
	"Reserved Exception.",
	"Reserved Exception.",
	"Reserved Exception.",
	"Reserved Exception.",
	"Reserved Exception.",
	"Reserved Exception.",
	"Reserved Exception.",
	"Reserved Exception.",
	"Reserved Exception.",
	"Reserved Exception.",
	"Reserved Exception.",
	"Reserved Exception.",
	"Reserved Exception."
};

extern "C" void _fault_handler(regs_t *r)
{
    if (r->int_no < 32)
    {
		kpanic(exception_messages[r->int_no],r);
 	}
}
