extern "C" void loadPageDirectory(unsigned int*);
extern "C" void enablePaging();

typedef unsigned int uint32_t;

void bpanic(const char* str) {
    int i = 0;
    while (*str) {
        *((char*)(0xb8000+i)) = *str;
        *((char*)(0xb8000+i+1)) = 0x04;
        i+=2;
        str++;
    }
    for(;;);
}

void print_pm_string(const char* str) {
    int i = 0;
    while (*str) {
        *((char*)(0xb8000+i)) = *str;
        *((char*)(0xb8000+i+1)) = 0x0e;
        i+=2;
        str++;
    }
}

extern "C" void boot_bits32() {
    print_pm_string("Reached 32-bit");
}