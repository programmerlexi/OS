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



extern "C" void _fault_handler(struct regs *r)
{
    if (r->int_no < 32)
    {
		asm("cli");
		if (is_system()) {
			clear_screen();
			print_string("An Error occured and the System stopped working.\n\r");
			print_string("Error: ");
			print_string(exception_messages[r->int_no]);
			print_string("\n\r");
			print_string("Error code: ");
			print_string(num_to_char(r->err_code));
			print_string("\n\r");
			print_string("Interrupt number: ");
			print_string(num_to_char(r->int_no));
			print_string("\n\r");
			print_string("Error Location: ");
			print_string(get_debug_location());
			swap_buffers();
		} else if (is_usermode()) {
			print_string(proc_name());
			print_string(": ");
			print_string(exception_messages[r->int_no]);
			print_string("\n\r");
			swap_buffers();    
		}
		if (strcmp(exception_messages[r->int_no], "Page Fault Exeption.")) {
			switch (r->err_code&1) {
				case 0:
					print_string("Supervisor process");
					if (is_usermode()) {
						print_string(" (");
						print_string(proc_name());
						print_string(")");
					}
					print_string(" tried to ");
					break;
				case 1:
					print_string("User process tried to ");
					break;
			}
			switch ((r->err_code>>1)&1) {
				case 0:
					print_string("read from ");
					break;
				case 1:
					print_string("write to ");
					break;
			}
			switch ((r->err_code>>2)&1) {
				case 0:
					print_string("non-present page\n\r");
					break;
				case 1:
					print_string("present page\n\r");
					break;
			}
			swap_buffers();
			disable_cursor();
		}
		for (;;);
    }
}
