void help() {
    print_string("help - prints out a help message\n\r");
    print_string("user - prints the name of the current user.\n\r");
    print_string("shutdown - shuts down the computer.\n\r");
    print_string("exit - exits the shell.\n\r");
    for (uint32_t i = 0; i < program_count; i++) {
        print_string(programs[i].name);
        print_string(" - ");
        print_string(programs[i].description);
        print_string("\n\r");
    }
    proc_quit(0);
}

void user() {
    print_string(USER);
    print_string("\n\r");
    proc_quit(0);
}

void test() {
    kernel_test();
    proc_quit(0);
}

void shutdown() {
    power_shutdown();
    proc_quit(0);
}

void clear() {
    enter_debug_scope("Clearing");
    clear_screen();
    enter_debug_scope("Exiting");
    proc_quit(0);
    exit_debug_scope();
}

void crash() {
    *((uint8_t*)0xffffffff) = 1;
}

void cpu_info() {
    print_string("CPU Vendor: ");
    print_string(get_vendor());
    print_string("\n\r");
    print_string("Virtual Vendor: ");
    print_string(get_virtual_vendor());
    print_string("\n\r");
    print_string("CPU Model Number: ");
    print_string(num_to_char(get_model()));
    print_string("\n\r");
    print_string("CPU Family: ");
    print_string(num_to_char(get_family()));
    print_string("\n\r");
    print_string("CPU Flags: ");
    if (check_sse()!=0) {
        print_string("SSE ");
    }
    if (check_sse2()!=0) {
        print_string("SSE2 ");
    }
    if (check_sse3()!=0) {
        print_string("SSE3 ");
    }
    if (check_sse4_1()!=0) {
        print_string("SSE4.1 ");
    }
    if (check_sse4_2()!=0) {
        print_string("SSE4.2 ");
    }
    if (check_apic()!=0) {
        print_string("APIC ");
    }
    if (check_pae()!=0) {
        print_string("PAE ");
    }
    if (check_movbe()!=0) {
        print_string("MOVBE ");
    }
    if (check_popcnt()!=0) {
        print_string("POPCNT ");
    }
    if (check_tsc()!=0) {
        print_string("TSC ");
    }
    if (check_x2apic()!=0) {
        print_string("X2APIC ");
    }
    if (check_xsave()!=0) {
        print_string("XSAVE ");
    }
    if (check_fpu()!=0) {
        print_string("FPU ");
    }
    print_string("\n\r");
    proc_quit(0);
}