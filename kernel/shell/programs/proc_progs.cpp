#include "../../services/proc.h"
#include "../../gatr/vga.h"
void procs() {
    clear_screen();
    disable_cursor();
    while (1) {
        set_cursor_pos(0);
        Task* task = &mainTask;
        print_string(num_to_char(task->tid));
        print_string(" ");
        switch (task->state) {
            case 0:
                print_string("RUNNING");
                break;
            case 1:
                print_string("READY");
                break;
            case 2:
                print_string("SLEEPING");
                break;
            default:
                print_string("Unknown");
        }
        print_string(" ");
        print_int(task->time_used);
        task = task->next;
        print_string("\n\r");
        while (task != &mainTask) {
            print_string(num_to_char(task->tid));
            print_string(" ");
            switch (task->state) {
                case 0:
                    print_string("RUNNING");
                    break;
                case 1:
                    print_string("READY");
                    break;
                case 2:
                    print_string("SLEEPING");
                    break;
                default:
                    print_string("Unknown");
            }
            print_string(" ");
            print_int(task->time_used);
            print_string("\n\r");
            task = task->next;
        }
    }
}