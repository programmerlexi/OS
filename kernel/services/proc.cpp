#include "proc.h"

bool locked = false;

void proc_lock() {
    locked = true;
}

void proc_unlock() {
    locked = false;
}

void proc_init() {
    cur_pid = 0;
    newest_pid = 0;
    //proc_create(kernel_loop);
}

void proc_cycle() {
    if (locked) return;
    if (cur_pid > newest_pid) {
        cur_pid = 0;
    }
    for (int i = 0; i < newest_pid; i++) {
        if (proc_list[i].state == PROC_DEAD) {
            if (cur_pid == i) {
                cur_pid++;
                if (cur_pid >= newest_pid) {
                    cur_pid = 0;
                    proc_cycle();
                    return;
                }
            }
            continue;
        }
        if (proc_list[i].pid == cur_pid) {
            if (proc_list[i].state == PROC_READY) {
                proc_list[i].state = PROC_RUNNING;
                proc_lock();
                char* name = proc_list[i].name;
                char* pid = num_to_char(proc_list[i].pid);
                char* debug = (char*)malloc(strlen(name) + strlen(pid) + strlen(" | "));
                strcpy(debug, name, strlen(name));
                strcat(debug, " | ", 3);
                strcat(debug, pid, strlen(pid));
                enter_debug_scope(debug);
                ((void(*)())proc_list[i].loop)();
                exit_debug_scope();
                proc_unlock();
            }
        } else {
            if (proc_list[i].state == PROC_RUNNING) {
                proc_list[i].state = PROC_READY;
            }
        }
    }
    cur_pid++;
}

const char* proc_name() {
    return proc_list[cur_pid].name;
}

void proc_create(void* loop_func, char* name) {
    proc_list[newest_pid].pid = newest_pid;
    proc_list[newest_pid].parent_pid = 0;
    proc_list[newest_pid].state = PROC_READY;
    proc_list[newest_pid].loop = loop_func;
    proc_list[newest_pid].name = name;
    newest_pid++;
}

void proc_quit(int exit_code) {
    if (cur_pid >= newest_pid) {
        return;
    }
    if (proc_list[cur_pid].parent_pid != 0) {
        proc_signal(proc_list[cur_pid].parent_pid);
    }
    proc_list[cur_pid].state = PROC_DEAD;
    proc_list[cur_pid].exit_code = exit_code;
}

int proc_fork(void* loop_func, char* name) {
    proc_create(loop_func, name);
    proc_list[newest_pid-1].parent_pid = proc_list[cur_pid].pid;
    return newest_pid-1;
}

void proc_wait() {
    if (cur_pid >= newest_pid) {
        return;
    }
    proc_list[cur_pid].state = PROC_WAITING;
    proc_cycle();
}

void proc_signal(int pid) {
    if (cur_pid >= newest_pid) {
        return;
    }
    if (proc_list[pid].state == PROC_WAITING) {
        proc_list[pid].state = PROC_READY;
    }
}