#include "../../services/proc.h"
void procs() {
    /*for (int i = 0; i < 256; i++) {
        if (proc_list[i].state == PROC_DEAD) continue;
        print_string(num_to_char(proc_list[i].pid));
        print_string(" ");
        print_string(proc_list[i].name);
        print_string(" ");
        switch (proc_list[i].state) {
            case PROC_RUNNING:
                print_string("RUNNING");
                break;
            case PROC_READY:
                print_string("READY");
                break;
            case PROC_SLEEPING:
                print_string("SLEEPING");
                break;
            case PROC_WAITING:
                print_string("WAITING");
                break;
            case PROC_DEAD:
                print_string("DEAD");
                break;
        }
        print_string("\n\r");
    }*/
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
        print_string("\n\r");
        task = task->next;
    }
}