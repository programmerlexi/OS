#include "idt.h"
#include "../memory/memory.h"

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{

	idt[num].base_lo = (base & 0xFFFF);
	idt[num].base_hi = (base >> 16) & 0xFFFF;


	idt[num].sel = sel;
	idt[num].always0 = 0;
	idt[num].flags = flags;
}


void idt_install()
{
	print_string("Setting IDT Pointer...\r");
	_idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
	_idtp.base = (unsigned int)&idt;

	/* Clear out the entire IDT, initalizing it to zeros */
	print_string("Clearing IDT...\r");
	memset(&idt, 0, sizeof(struct idt_entry) * 256);

	/* Add any new ISRs to the IDT here using idt_set_gate */

	/* Points the processor's internal register to the new IDT */
	print_string("Installing IDT...\r");
	_idt_load();
}