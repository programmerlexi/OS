typedef unsigned short uint16_t;

void print_string(const char*);
void print_string(char*);
void print_char(char);
void print_int(uint16_t);
char* num_to_char(uint16_t);
int digit_count(uint16_t);

extern "C" void load_kernel_from_disk();
extern "C" void load_gdt();
extern "C" void set_protected_mode();
extern "C" void jump_to_protected_mode();
extern "C" void enableA20();

extern "C" void loader_c() {
    print_string("Reached loader_c!\n\r");
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