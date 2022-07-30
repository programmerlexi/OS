#include "tss.h"
#include "../segmentation/gdt.h"
#include "../memory/memory.h"
// Note: some of the GDT entry struct field names may not match perfectly to the TSS entries.
tss_entry_t tss_entry;
 
void write_tss(GDTEntry* g) {
	// Compute the base and limit of the TSS for use in the GDT entry.
	uint32_t base = (uint32_t) &tss_entry;
	uint32_t limit = sizeof tss_entry;
 
	// Set the GDT entry for the TSS.
    *g = GDT_ENTRY(base, limit,
                   GDT_ACCESS_PRESENT | GDT_ACCESS_RING3 | GDT_ACCESS_DESCRIPTOR_TSS,
                   GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_1B);
 
	// Ensure the TSS is initially zero'd.
	memset(&tss_entry, 0, sizeof tss_entry);
 
	tss_entry.ss0  = GDT_DATA_SEGMENT;  // Set the kernel stack segment.
	//tss_entry.esp0 = (uint32_t) &kernel_stack_top;  // Set the kernel stack pointer.
	//note that CS is loaded from the IDT entry and should be the regular kernel code segment
}
 
void set_kernel_stack(uint32_t stack) { // Used when an interrupt occurs
	tss_entry.esp0 = stack;
}