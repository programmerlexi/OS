typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

uint32_t pos = 0;

void bpanic(const char* str) {
    for (int i = 0; i < 2000; i+=1) {
        *((char*)(0xb8000+i*2)) = 0;
        *((char*)(0xb8000+i*2+1)) = 0x0f;
    }
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
    int i = pos;
    while (*str) {
        if (*str == '\n') i += 80*2;
        else if (*str == '\r') i -= i % (80*2);
        else {
            *((char*)(0xb8000+i)) = *str;
            *((char*)(0xb8000+i+1)) = 0x0e;
            i+=2;
        }
        str++;
    }
    pos = i;
}

void print_pm_string(char* str) {
    print_pm_string((const char*)str);
}

void clear_screen() {
    for (int i = 0, pos = 0; i < 2000; i+=1) {
        *((char*)(0xb8000+i*2)) = 0;
        *((char*)(0xb8000+i*2+1)) = 0x0f;
    }

}

uint8_t inb_32(uint16_t port) {
    uint8_t data;
    asm volatile("inb %1, %0" : "=a"(data) : "Nd"(port));
    return data;
}

void outb32(uint16_t port, uint8_t data) {
    asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

uint8_t input_keycode() {
    uint8_t keycode = 0;
    while((keycode = inb_32(0x60)) != 0){
        if(keycode > 0)
            return keycode;
    }
    return keycode;
}

extern "C" void boot_bits32() {
    outb32(0x3D4, 0xA);
    outb32(0x3D5, 0x20);
    pos = 0;
    clear_screen();
    print_pm_string("Reached 32-bit\n\r");
    print_pm_string("Let go of enter to continue.\n\r");
    while (input_keycode() == 0x1c) asm("nop");
    return;
}