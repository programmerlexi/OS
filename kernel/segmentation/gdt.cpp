#include "gdt.h"
#include "../ring3/tss.h"
#include <stdint.h>

/*GDTEntry g_GDT[] = {
    // TSS
    GDT_ENTRY(0,
              0xFFFFF,
              GDT_ACCESS_PRESENT | GDT_ACCESS_RING3 | GDT_ACCESS_DESCRIPTOR_TSS,
              GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_1B),

};*/

GDTDescriptor g_GDTDescriptor = { sizeof(g_GDT) - 1, g_GDT};

//extern void load_gdt(GDTDescriptor* descriptor, uint16_t codeSegment, uint16_t dataSegment);

void init_gdt()
{
    g_GDT[0] = GDT_ENTRY(0, 0, 0, 0);
    g_GDT[1] = GDT_ENTRY(0,
              0x100000000,
              GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_CODE_SEGMENT | GDT_ACCESS_CODE_READABLE,
              GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K);
    g_GDT[2] = GDT_ENTRY(0,
              0x100000000,
              GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_DATA_SEGMENT | GDT_ACCESS_DATA_WRITEABLE,
              GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K);
    g_GDT[3] = GDT_ENTRY(0,
                0x100000000,
                GDT_ACCESS_PRESENT | GDT_ACCESS_RING3 | GDT_ACCESS_CODE_SEGMENT | GDT_ACCESS_CODE_READABLE,
                GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_1B); // Ring 3 code segment
    g_GDT[4] = GDT_ENTRY(0,
                0x100000000,
                GDT_ACCESS_PRESENT | GDT_ACCESS_RING3 | GDT_ACCESS_DATA_SEGMENT | GDT_ACCESS_DATA_WRITEABLE,
                GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_1B); // Ring 3 data segment
    write_tss(&g_GDT[5]);
    load_gdt(&g_GDTDescriptor, GDT_CODE_SEGMENT, GDT_DATA_SEGMENT);
}