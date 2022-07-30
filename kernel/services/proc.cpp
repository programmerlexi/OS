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
                ((void(*)())proc_list[i].loop)();
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
    proc_list[cur_pid].state = PROC_DEAD;
    proc_list[cur_pid].exit_code = exit_code;
}

int proc_fork(void* loop_func, char* name) {
    proc_create(loop_func, name);
    proc_list[newest_pid-1].parent_pid = proc_list[cur_pid].pid;
    return newest_pid-1;
}