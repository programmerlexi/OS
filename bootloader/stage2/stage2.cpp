void print_string(const char*);
void print_string(char*);
void print_char(char);

extern "C" void loader_c() {
    print_string("Hello World from C++\n\r");
    for(int i = 0; i < 0xfffffff; i++) {
        asm("nop");
    }
    print_string("Loading Kernel...\n\r");
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