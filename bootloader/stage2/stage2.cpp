typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

void print_string(const char*);
void print_string(char*);
void print_char(char);
void print_int(uint16_t);
char* num_to_char(uint16_t);
int digit_count(uint16_t);

uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t inl(uint16_t port);

void outb(uint16_t port, uint8_t val);
void outw(uint16_t port, uint16_t val);
void outl(uint16_t port, uint32_t val);

uint8_t get_input_keycode();
char get_ascii_char(uint8_t key);

extern "C" void load_kernel_from_disk();
extern "C" void load_gdt();
extern "C" void set_protected_mode();
extern "C" void jump_to_protected_mode();
extern "C" void enableA20();

uint8_t prev_key = 0;

extern "C" void loader_c() {
    print_string("Reached loader_c!\n\r");
    print_string("You can type anything here\n\r");
    print_string("Press ENTER to boot, '.' to toggle splash (default: on) or ESC to reboot...\n\r");
    uint8_t key = 0;
    bool* splash = (bool*)0x5005;
    *splash = true;
    while ((key = get_input_keycode()) != 0x1C) {
        if (!(prev_key == key)) {
            if (key == 0x01) {
                outb(0x64,0xFE); // Tell the keyboard controller to pulse the reset line
            } else if (key == 0x34) {
                *splash = !*splash;
            }
            char c = get_ascii_char(key);
            if (c != 0) {
                print_char(c);
            }
        }
        prev_key = key;
    }
    
    print_string("Enabling A20 line...\n\r");
    enableA20();
    print_string("Loading Kernel from disk ...\n\r");
    load_kernel_from_disk();
    print_string("Loading GDT...\n\r");
    load_gdt();
    print_string("Setting Protected Mode...\n\r");
    set_protected_mode();
    jump_to_protected_mode();
}

// This just invokes the BIOS function
void print_char(char c) {
    asm(
        "mov %0, %%al;"
		"mov $0x0E, %%ah;"
		"int $0x10;"
		:
		: "r" (c)
    );
}

void print_string(char* string) {
    int i = 0;
    while (string[i]) {
        print_char(string[i]);
        i++;
    }
}

void print_string(const char *string) {
    print_string((char*)string);
}

void print_int(uint16_t byte) {
    print_string(num_to_char(byte));
}

int digit_count(uint16_t num) {
    uint16_t count = 0;
    if(num == 0)
        return 1;
    while(num > 0){
        count++;
        num = num/10;
    }
    return count;
}

char* num_to_char(uint16_t byte) {
    char* number = (char*)"   ";
    int dgcount = digit_count(byte);
    int index = dgcount - 1;
    char x;
    if(byte == 0 && dgcount == 1){
        number[0] = '0';
        number[1] = '\0';
    }else{
        while(byte != 0){
            x = byte % 10;
            number[index] = x + '0';
            index--;
            byte = byte / 10;
        }
        number[dgcount] = '\0';
    }
    return number;
}

uint8_t get_input_keycode()
{
    uint8_t keycode = 0;
    while((keycode = inb(0x60)) != 0){
        if(keycode > 0)
            return keycode;
    }
    return keycode;
}

uint8_t inb(uint16_t port) {
    uint8_t data;
    asm volatile("inb %1, %0" : "=a"(data) : "Nd"(port));
    return data;
}

uint16_t inw(uint16_t port) {
    uint16_t data;
    asm volatile("inw %1, %0" : "=a"(data) : "Nd"(port));
    return data;
}

uint32_t inl(uint16_t port) {
    uint32_t data;
    asm volatile("inl %1, %0" : "=a"(data) : "Nd"(port));
    return data;
}

void outb(uint16_t port, uint8_t data) {
    asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

void outw(uint16_t port, uint16_t data) {
    asm volatile("outw %0, %1" : : "a"(data), "Nd"(port));
}

void outl(uint16_t port, uint32_t data) {
    asm volatile("outl %0, %1" : : "a"(data), "Nd"(port));
}

#include "keycode.cpp"