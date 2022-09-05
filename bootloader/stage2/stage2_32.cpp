#define PAGES_PER_TABLE 1024
#define TABLES_PER_DIRECTORY 1024
#define PAGE_SIZE 4096

#define PD_INDEX(addr) ((addr>>22)&0x3FF)
#define PT_INDEX(addr) ((addr>>12)&0x3FF)
#define PAGE_PHYS_ADDR(dir_entry) ((*dir_entry)&~0xFFF)
#define SET_ATTRIBUTE(entry,attr) (*entry |= attr)
#define CLEAR_ATTRIBUTE(entry,attr) (*entry &= ~attr)
#define TEST_ATTRIBUTE(entry,attr) (*entry & attr)
#define SET_FRAME(entry,addr) (*entry = (*entry & ~0x7FFFF000) | (addr & 0x7FFFF000))

extern "C" void loadPageDirectory(unsigned int*);
extern "C" void enablePaging();

unsigned int paging_directory[1024] __attribute__((aligned(4096)));
unsigned int first_page_table[1024] __attribute__((aligned(4096)));
unsigned int kernel_page_table[1024] __attribute__((aligned(4096)));

typedef unsigned int uint32_t;
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
} page_table;

typedef struct {
    pde entries[TABLES_PER_DIRECTORY];
} page_directory;

extern "C" void boot_bits32() {
    int i;
    for(i = 0; i < 1024; i++)
    {
        SET_ATTRIBUTE(&paging_directory[i], PDE_READ_WRITE);
    }

    for(uint32_t i = 0, frame = 0x0, virt = 0x0; i<PAGES_PER_TABLE; i++, frame+=PAGE_SIZE, virt += PAGE_SIZE)
    {
        pte page = 0;
        SET_ATTRIBUTE(&page,PTE_PRESENT);
        SET_ATTRIBUTE(&page,PTE_READ_WRITE);
        SET_FRAME(&page,frame);
        first_page_table[PT_INDEX(virt)] = page;
    }

    for (uint32_t i = 0, frame = 0x6000, virt = 0xC0000000; i<10; i++, frame+=PAGE_SIZE, virt += PAGE_SIZE) {
        pte page = 0;
        SET_ATTRIBUTE(&page,PTE_PRESENT);
        SET_ATTRIBUTE(&page,PTE_READ_WRITE);
        SET_FRAME(&page,frame);
        kernel_page_table[PT_INDEX(virt)] = page;
    }
    
    for (uint32_t i = 0, virt = 0x6000; i < 10; virt += PAGE_SIZE, i++) {
        page_directory *pd = (page_directory*)paging_directory;
        pde *entry = &pd->entries[PD_INDEX(virt)];
        page_table *table = (page_table*)PAGE_PHYS_ADDR(entry);
        pte *page = &table->entries[PT_INDEX(virt)];
        SET_FRAME(page,0);
        CLEAR_ATTRIBUTE(page,PTE_PRESENT);
    }
    pde *entry = &paging_directory[PD_INDEX(0x00000000)]; // This should be entry 0
    SET_ATTRIBUTE(entry,PDE_PRESENT);
    SET_ATTRIBUTE(entry,PDE_READ_WRITE);
    SET_FRAME(entry,((paddr)first_page_table));

    pde *kernel_entry = &paging_directory[PD_INDEX(0xC0000000)];
    SET_ATTRIBUTE(kernel_entry,PDE_PRESENT);
    SET_ATTRIBUTE(kernel_entry,PDE_READ_WRITE);
    SET_FRAME(kernel_entry,((paddr)kernel_page_table));

    loadPageDirectory(paging_directory);
    enablePaging();
}