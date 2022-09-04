#include "paging.h"
unsigned int id = 0;
void idpaging(uint32_t *first_pte, int from, int size) {
   from = from & 0xfffff000; // discard bits we don't want
   for(; size>0; from += 4096, size -= 4096, first_pte++){
      *first_pte = from | 1;     // mark page present.
   }
}
void init_paging() {
    page_directory *dir = (page_directory*)current_page_directory;
    if (!dir) kpanic("Out of memory", get_regs());
    memset(dir,0,sizeof(page_directory));
    for (uint32_t i = 0; i < TABLES_PER_DIRECTORY; i++) {
        dir->entries[i] = 0x02;
    }
    page_table *table = (page_table*)paging_table;
    page_table *table3G = (page_table*)paging_table3G;
    if (!table) kpanic("Out of memory", get_regs());
    if (!table3G) kpanic("Out of memory", get_regs());
    memset(table3G,0,sizeof(page_table));
    memset(table,0,sizeof(page_table));
    for (uint32_t i = 0, frame = 0x0, virt = 0x0; i<PAGES_PER_TABLE; i++, frame+=PAGE_SIZE, virt += PAGE_SIZE) {
        pte page = 0;
        SET_ATTRIBUTE(&page,PTE_PRESENT);
        SET_ATTRIBUTE(&page,PTE_READ_WRITE);
        SET_FRAME(&page,frame);
        table3G->entries[PT_INDEX(virt)] = page;
    }
    for (uint32_t i = 0, frame = 0x6000, virt = 0xC0000000; i<PAGES_PER_TABLE; i++, frame+=PAGE_SIZE, virt += PAGE_SIZE) {
        pte page = 0;
        SET_ATTRIBUTE(&page,PTE_PRESENT);
        SET_ATTRIBUTE(&page,PTE_READ_WRITE);
        SET_FRAME(&page,frame);
        table->entries[PT_INDEX(virt)] = page;
    }
    /*pde *entry = &dir->entries[PD_INDEX(0xC0000000)];
    SET_ATTRIBUTE(entry,PDE_PRESENT);
    SET_ATTRIBUTE(entry,PDE_READ_WRITE);
    SET_FRAME(entry,(paddr)table);*/

    pde *entry2 = &dir->entries[PD_INDEX(0x00000000)];
    SET_ATTRIBUTE(entry2,PDE_PRESENT);
    SET_ATTRIBUTE(entry2,PDE_READ_WRITE);
    SET_FRAME(entry2,(paddr)table3G);

    set_page_directory(dir);
    //loadPageDirectory(current_page_directory);
    enablePaging();
}

pde *get_pd_entry(page_directory *pd, vaddr virtual_address) {
    if (pd) return &pd->entries[PD_INDEX(virtual_address)];
    return 0;
}

pte *get_pt_entry(page_table *pt, vaddr virtual_address) {
    if (pt) return &pt->entries[PT_INDEX(virtual_address)];
    return 0;
}

pte *get_page(const vaddr addr) {
    page_directory *pd = (page_directory*)current_page_directory;
    pde *entry = &pd->entries[PD_INDEX(addr)];
    page_table *table = (page_table*)PAGE_PHYS_ADDR(entry);
    pte *page = &table->entries[PT_INDEX(addr)];
    return page;
}

void *allocate_page(pte *page) {
    void *block = allocate_blocks(1);
    if (block) {
        // Map page to block
        SET_FRAME(page,(paddr)block);
        SET_ATTRIBUTE(page,PTE_PRESENT);
    } else {
        kpanic("Out of memory",get_regs());
    }
    return block;
}

bool set_page_directory(page_directory *pd) {
    if (!pd) return false;
    loadPageDirectory(pd);
    return true;
}

void flush_tlb_entry(vaddr addr) {
    asm("cli; invlpg (%0); sti" : : "r"(addr));
}

bool map_page(void *physicaladdr, void *virtualaddr) {
    page_directory *pd = (page_directory*)current_page_directory;
    pde *entry = &pd->entries[PD_INDEX((uint32_t)virtualaddr)];
    if (!TEST_ATTRIBUTE(entry,PDE_PRESENT)) {
        page_table *table = (page_table*)allocate_blocks(1);
        if (!table) kpanic("Out of memory", get_regs());
        memset(table,0,sizeof(page_table));
        pde *entry = &pd->entries[PD_INDEX((uint32_t)virtualaddr)];
        SET_ATTRIBUTE(entry,PDE_PRESENT);
        SET_ATTRIBUTE(entry,PDE_READ_WRITE);
        SET_FRAME(entry,(paddr)table);
    }
    page_table *table = (page_table*) PAGE_PHYS_ADDR(entry);
    pte *page = &table->entries[PT_INDEX((uint32_t)virtualaddr)];
    SET_FRAME(page,(paddr)physicaladdr);
    SET_ATTRIBUTE(page,PTE_PRESENT);
}

bool unmap_page(void* virtualaddr) {
    pte *page = get_page((uint32_t)virtualaddr);
    SET_FRAME(page,0);
    CLEAR_ATTRIBUTE(page,PTE_PRESENT);
}