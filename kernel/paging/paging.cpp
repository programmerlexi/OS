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
    if (!table) kpanic("Out of memory", get_regs());
    memset(table,0,sizeof(page_table));
    for (uint32_t i = 0, frame = 0x0, virt = 0x0; i<PAGES_PER_TABLE; i++, frame+=PAGE_SIZE, virt += PAGE_SIZE) {
        pte page = 0;
        SET_ATTRIBUTE(&page,PTE_PRESENT);
        SET_ATTRIBUTE(&page,PTE_READ_WRITE);
        SET_FRAME(&page,frame);
        table->entries[PT_INDEX(virt)] = page;
    }
    pde entry = 0;
    SET_ATTRIBUTE(&entry,PDE_PRESENT);
    SET_ATTRIBUTE(&entry,PDE_READ_WRITE);
    SET_FRAME(&entry,(paddr)table);
    dir->entries[0] = entry;

    page_table *table2 = (page_table*)paging_table2;
    if (!table2) kpanic("Out of memory", get_regs());
    memset(table2,0,sizeof(page_table));
    for (uint32_t i = 0, frame = 0x400000, virt = 0x400000; i<PAGES_PER_TABLE; i++, frame+=PAGE_SIZE, virt += PAGE_SIZE) {
        pte page = 0;
        SET_ATTRIBUTE(&page,PTE_PRESENT);
        SET_ATTRIBUTE(&page,PTE_READ_WRITE);
        SET_FRAME(&page,frame);
        table2->entries[PT_INDEX(virt)] = page;
    }
    pde entry2 = 0;
    SET_ATTRIBUTE(&entry2,PDE_PRESENT);
    SET_ATTRIBUTE(&entry2,PDE_READ_WRITE);
    SET_FRAME(&entry2,(paddr)table2);
    dir->entries[1] = entry2;

    page_table *table3 = (page_table*)paging_table3;
    if (!table3) kpanic("Out of memory", get_regs());
    memset(table3,0,sizeof(page_table));
    for (uint32_t i = 0, frame = 0x800000, virt = 0x800000; i<PAGES_PER_TABLE; i++, frame+=PAGE_SIZE, virt += PAGE_SIZE) {
        pte page = 0;
        SET_ATTRIBUTE(&page,PTE_PRESENT);
        SET_ATTRIBUTE(&page,PTE_READ_WRITE);
        SET_FRAME(&page,frame);
        table3->entries[PT_INDEX(virt)] = page;
    }
    pde entry3 = 0;
    SET_ATTRIBUTE(&entry3,PDE_PRESENT);
    SET_ATTRIBUTE(&entry3,PDE_READ_WRITE);
    SET_FRAME(&entry3,(paddr)table3);
    dir->entries[2] = entry3;

    page_table *table4 = (page_table*)paging_table4;
    if (!table4) kpanic("Out of memory", get_regs());
    memset(table4,0,sizeof(page_table));
    for (uint32_t i = 0, frame = 0xC00000, virt = 0xC00000; i<PAGES_PER_TABLE; i++, frame+=PAGE_SIZE, virt += PAGE_SIZE) {
        pte page = 0;
        SET_ATTRIBUTE(&page,PTE_PRESENT);
        SET_ATTRIBUTE(&page,PTE_READ_WRITE);
        SET_FRAME(&page,frame);
        table4->entries[PT_INDEX(virt)] = page;
    }
    pde entry4 = 0;
    SET_ATTRIBUTE(&entry4,PDE_PRESENT);
    SET_ATTRIBUTE(&entry4,PDE_READ_WRITE);
    SET_FRAME(&entry4,(paddr)table4);
    dir->entries[3] = entry4;

    set_page_directory(dir);
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
    void *block = malloc(1024*4);
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
    loadPageDirectory(pd->entries);
    return true;
}

void flush_tlb_entry(vaddr addr) {
    asm("cli; invlpg (%0); sti" : : "r"(addr));
}

bool map_page(void *physicaladdr, void *virtualaddr) {
    page_directory *pd = (page_directory*)current_page_directory;
    pde *entry = &pd->entries[PD_INDEX((uint32_t)virtualaddr)];
    if (!TEST_ATTRIBUTE(entry,PDE_PRESENT)) {
        page_table *table = (page_table*)malloc(1024*4);
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
    return true;
}

bool unmap_page(void* virtualaddr) {
    pte *page = get_page((uint32_t)virtualaddr);
    SET_FRAME(page,0);
    CLEAR_ATTRIBUTE(page,PTE_PRESENT);
    return true;
}