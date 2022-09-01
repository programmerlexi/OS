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
    enter_debug_scope((char*)"Clearing");
    clear_screen();
    enter_debug_scope((char*)"Exiting");
    proc_quit(0);
    exit_debug_scope();
}

void crash() {
    *((uint8_t*)0xffffffff) = 1;
}

void read_sector_one() {
    print_string("Reading first sector\n\r");
    uint16_t* buf = (uint16_t*)LBA28_read_sectors(0xA0,0,1);
    for (uint32_t i = 0; i < 256; i++) {
        print_string(HexToString(buf[i]));
        print_string(" ");
    }
    free(buf);
    proc_quit(0);
}

void reboot() {
    power_restart();
}

void memory() {
    heap_segment_header* segment = first_free_segment;
    while(1) {
        /*if (segment->free) {
            print_string("F");
        } else {
            print_string("U");
        }*/
        print_string(HexToString((uint64_t)segment));
        print_string(" ");
        print_string(HexToString((uint64_t)segment->size));
        print_string(" ");
        if (segment->free) {
            print_string("Free");
        } else {
            print_string("Used");
        }
        print_string(" ");
        print_string(HexToString((uint64_t)segment->next));
        print_string(" ");
        print_string(HexToString((uint64_t)segment->prev));
        print_string("\n\r");
        segment = segment->next;
        if (segment == NULL) break;
    }
    int i = 0;
    segment = first_free_segment;
    while (1) {
        if (segment->free) i++;
        segment = segment->next;
        if (segment == NULL) break;
    }
    print_string(HexToString((uint64_t)i));
    print_string(" segment(s) of ");
    print_string(HexToString((uint64_t)count_segments()));
    print_string(" segment(s) are free\n\r");
    //print_string("\n\r");
    proc_quit(0);
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