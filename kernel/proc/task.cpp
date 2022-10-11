// For real this time.

#include "task.h"
#include "../gatr/vga.h"
#include "../services/debug.h"
#include "../memory/heap.h"

void fork(Task* task, void(*func)()) {
    createTask(task,func,runningTask->regs.eflags,(uint32_t*)mainTask.regs.cr3);
    task->next = runningTask->next;
    runningTask->next = task;
}

Task* fork(void(*func)()) {
    Task* t = (Task*)malloc(sizeof(Task));
    fork(t, func);
    return t;
}

void quit() {
    if (runningTask->tid == 0) kpanic("Something went wrong, the kernel is gone!",get_regs());
    runningTask->running = false;
    Task* current = runningTask;
    while (1) {
        if (current->next == runningTask) {
            current->next = runningTask->next;
            break;
        }
        current = current->next;
    }
    yield();
}

void initTasking() {
    enter_debug_scope((char*)"init_tasking");
    // Get EFLAGS and CR3
    __asm__ __volatile__("movl %%cr3, %%eax; movl %%eax, %0;":"=m"(mainTask.regs.cr3)::"%eax");
    __asm__ __volatile__("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;":"=m"(mainTask.regs.eflags)::"%eax");
    mainTask.tid = 0;

    mainTask.next = &mainTask;
 
    runningTask = &mainTask;
    exit_debug_scope();
}

void process_end() {
    quit(); // Remove the broken process
    for (;;);
}

uint64_t prev_stack = 0x300000;
void* allocateStack() {
    enter_debug_scope((char*)"stack allocation");
    prev_stack = prev_stack + 0x2000;
    *(uint32_t*)(prev_stack - 4) = (uint32_t)&process_end;
    return (void*)prev_stack;
}

void createTask(Task *task, void (*main)(), uint32_t flags, uint32_t *pagedir) {
    task->regs.eax = 0;
    task->regs.ebx = 0;
    task->regs.ecx = 0;
    task->regs.edx = 0;
    task->regs.esi = 0;
    task->regs.edi = 0;
    task->regs.eflags = flags;
    task->regs.eip = (uint32_t) main;
    task->regs.cr3 = (uint32_t) pagedir;
    task->regs.esp = (uint32_t) allocateStack()-4; // Not implemented here
    task->regs.ebp = task->regs.esp+4;
    task->next = 0;
    task->running = true;
    task->time_used = 0;
    task->state = READY;
    task->tid = newest_tid;
    newest_tid++;
}

uint64_t irq_lock_counter = 0;
void lock_scheduler() {
    asm("cli");
    irq_lock_counter++;
}

void unlock_scheduler() {
    irq_lock_counter--;
    if (irq_lock_counter == 0) {
        asm("sti");
    }
}

void yield() {
    Task *last = runningTask;
    runningTask = runningTask->next;
    while (runningTask->state != READY && runningTask->state != RUNNING) {
        runningTask = runningTask->next;
    }
    if (runningTask->state == RUNNING) return; // task already current
    last->state = READY;
    runningTask->state = RUNNING;
    switchTask(&last->regs, &runningTask->regs);
}

void schedule() {
    Task *last = runningTask;
    runningTask = runningTask->next;
    while (runningTask->state != READY && runningTask->state != RUNNING) {
        runningTask = runningTask->next;
    }
    if (runningTask->state == RUNNING) return; // task already current
    last->state = READY;
    runningTask->state = RUNNING;
    switchTask(&last->regs, &runningTask->regs);
}