#pragma once
#include <stdint.h>
#include <stddef.h>

#include "allocator.h"
#include "../interrupts/regs.h"
#include "../memory/memory.h"

#define PAGES_PER_TABLE 1024
#define TABLES_PER_DIRECTORY 1024
#define PAGE_SIZE 4096

#define PD_INDEX(addr) ((addr>>22)&0x3FF)
#define PT_INDEX(addr) ((addr>>12)&0x3FF)
#define PAGE_PHYS_ADDR(dir_entry) ((*dir_entry)&~0xFFF)
#define SET_ATTRIBUTE(entry,attr) (*entry |= attr)
#define CLEAR_ATTRIBUTE(entry,attr) (*entry &= ~attr)
#define TEST_ATTRIBUTE(entry,attr) (*entry & attr)
#define SET_FRAME(entry,addr) (*entry = (*entry & ~0x7FFFF000) | addr)

typedef uint32_t pde;
typedef uint32_t pte;
typedef uint32_t vaddr;
typedef uint32_t paddr;

typedef enum {
    PTE_PRESENT         = 0x001,
    PTE_READ_WRITE      = 0x002,
    PTE_USER            = 0x004,
    PTE_WRITE_THROUGH   = 0x008,
    PTE_CACHE_DISABLE   = 0x010,
    PTE_ACCESSED        = 0x020,
    PTE_DIRTY           = 0x040,
    PTE_PAT             = 0x080,
    PTE_GLOBAL          = 0x100,
    PTE_FRAME           = 0x7FFFF000
} pte_flags;

typedef enum {
    PDE_PRESENT         = 0x001,
    PDE_READ_WRITE      = 0x002,
    PDE_USER            = 0x004,
    PDE_WRITE_THROUGH   = 0x008,
    PDE_CACHE_DISABLE   = 0x010,
    PDE_ACCESSED        = 0x020,
    PDE_DIRTY           = 0x040,
    PDE_PAGE_SIZE       = 0x080,
    PDE_GLOBAL          = 0x100,
    PDE_PAT             = 0x2000,
    PDE_FRAME           = 0x7FFFF000
} pde_flags;

typedef struct {
    pte entries[PAGES_PER_TABLE];
} page_table __attribute__((aligned(4096)));

typedef struct {
    pde entries[TABLES_PER_DIRECTORY];
} page_directory __attribute__((aligned(4096)));

extern "C" void loadPageDirectory(page_directory*);
extern "C" void enablePaging();

pde current_page_directory[TABLES_PER_DIRECTORY];
pte paging_table[PAGES_PER_TABLE];
pte paging_table3G[PAGES_PER_TABLE];


void init_paging();
pte *get_pt_entry(page_table *pt, vaddr virtual_address);
pde *get_pd_entry(page_directory *pd, vaddr virtual_address);
pte *get_page(const vaddr addr);
void *allocate_page(pte *page);
void *allocate_blocks(uint8_t blocks);
bool set_page_directory(page_directory *pd);
void flush_tlb_entry(vaddr addr);
bool map_page(void* physicaladdr, void* virtualaddr);
bool unmap_page(void *virtualaddr);