#include "gatr/vga.h"
#include "services/proc.h"
#include "services/keyboard.h"
#include "shell/env.cpp"

void terminal_loop();

typedef struct program_info {
    char* name;
    char* description;
    void (*func)();
} program_info;

program_info* programs;
uint32_t program_count;

void add_program(program_info* info) {
    programs[program_count] = *info;
    program_count++;
}

void build_program(void (*func)(), char* name, char* description) {
    program_info info;
    info.func = func;
    info.name = name;
    info.description = description;
    add_program(&info);
}

#include "shell/programs/builtins.cpp"

void load_programs() {
    program_count = 0;
    programs = (program_info*)malloc(sizeof(program_info) * 10);
    build_program(clear,"clear","clears the screen");
    build_program(test,"test","tests the kernel");
    build_program(cpu_info,"cpuinfo","prints CPU information");
}

int sub_pid;
bool started = false;
bool printed = false;
bool logged = false;
bool enter_pressed_before = false;

void terminal_init() {
    clear_screen();
    set_cursor_pos(0);
    print_string("Welcome to the kernel shell!\n\r");
    programs = (program_info*)malloc(sizeof(program_info) * 10);
    load_programs();
    if (strcmp(USER,"kernel")) {
        print_string("You are logged in as the kernel. This is not recommended, please login!\n\r");
        print_string("Login: ");
        set_input(true);
        clear_input();
    } else {
        print_string("You are logged in as ");
        print_string(USER);
        print_string(".\n\r");
        logged = true;
    }
    printed = false;
    enter_pressed_before = false;
    started = false;
}

char* usr;
int current_pos = 0;

void terminal_loop() {
    enter_debug_scope((char*)"terminal_loop");
    if (!logged && kpressed[0x1c]) {
        usr = get_input();
        set_input(false);
        if (strcmp(usr, "root")) {
            print_string("\n\rYou are logged in as root.\n\r");
            USER = (char*)"root";
            logged = true;
        } else {
            clear_screen();
            print_string("Login failed.\n\r");
            print_string("Login: ");
            clear_input();
            set_input(true);
            return;
        }
    } else {
        if (!logged) {
            return;
        }
    }
    if (started) {
        if (proc_list[sub_pid].state == PROC_DEAD) {
            started = false;
            printed = false;
            switch (proc_list[sub_pid].exit_code) {
                case 0:
                    break;
                case 1:
                    print_string("Program exited with exit code 1 (Not Implemented)\n\r");
                    break;
                default:
                    print_string("Program exited with non-zero exit code\n\r");
                    break;
            }
        } else {
            return;
        }
    }
    if (printed) {
        if (kpressed[0x1c] && !enter_pressed_before) {
            char* input = get_input();
            clear_input();
            print_string("\n\r");
            set_input(false);
            if (strcmp(input, "help")) {
                enter_debug_scope((char*)"help");
                started = true;
                sub_pid = proc_fork((void*)help,"help");
                exit_debug_scope();
            } else if (strcmp(input, "user")) {
                enter_debug_scope((char*)"user");
                started = true;
                sub_pid = proc_fork((void*)user,"user");
                exit_debug_scope();
            } else if (strcmp(input, "shutdown")) {
                enter_debug_scope((char*)"shutdown");
                started = true;
                sub_pid = proc_fork((void*)shutdown,"shutdown");
                exit_debug_scope();
            } else if (strcmp(input, "exit")) {
                enter_debug_scope((char*)"exit");
                print_string("Goodbye!\n\r");
                set_input(true);
                proc_quit(0);
                exit_debug_scope();
                return;
            } else if (strlen(input) > 0) {
                bool found = false;
                for (int i = 0; i < 10; i++) {
                    if (strcmp(input, programs[i].name)) {
                        enter_debug_scope((char*)"program");
                        found = true;
                        started = true;
                        sub_pid = proc_fork((void*)programs[i].func,(const char*)programs[i].name);
                        exit_debug_scope();
                        break;
                    }
                }
                if (!found) {
                    print_string("Unknown command.\n\r");
                    print_string("Type 'help' for a list of commands.\n\r");
                    printed = false;
                }
            } else {
                printed = false;
            }
            enter_pressed_before = true;
        } else if (!kpressed[0x1c]) {
            enter_pressed_before = false;
        } else {
            return;
        }
    } else {
        print_string("$ ");
        printed = true;
        set_input(true);
        clear_input();
    }
    exit_debug_scope();
}