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
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",
	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

extern "C" void _fault_handler(regs_t *r)
{
    if (r->int_no < 32)
    {
		if (handle_interrupts) {
			if (tasking) {
				if (runningTask->tid == 0) {
					kpanic(exception_messages[r->int_no],r);
				} else {
					print_int(runningTask->tid);
					print_string(": ");
					switch (r->int_no) {
						case 14:
							if ((r->err_code >> 2) & 1) print_string("User process ");
							else print_string("Supervisor process ");
							print_int(runningTask->tid);
							print_string(" tried to ");
							if ((r->err_code >> 1) & 1) print_string("write to ");
							else print_string("read from ");
							if ((r->err_code >> 0) & 1) print_string("a present page");
							else print_string("a non-present page");
							if ((r->err_code >> 5) & 1) print_string(" and violated a protection-key");
							if ((r->err_code >> 6) & 1) print_string(" during a shadow stack access");
							if ((r->err_code >> 4) & 1) print_string(" during instruction-fetch");
							print_string(".");
							break;
						default:
							print_string(exception_messages[r->int_no]);
					}
					print_string("\n\r");
					quit();
				}
			} else {
				kpanic(exception_messages[r->int_no],r);
			}
		} 
 	}
}
