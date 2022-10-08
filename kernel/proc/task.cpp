// For real this time.

#include "task.h"
#include "../gatr/vga.h"
 
static Task *runningTask;
static Task mainTask;
static Task otherTask;
 
static void otherMain() {
    print_string("Hello multitasking world!\n\r"); // Not implemented here...
    yield();
}
 
void initTasking() {
    enter_debug_scope((char*)"init_tasking");
    // Get EFLAGS and CR3
    asm volatile("movl %%cr3, %%eax; movl %%eax, %0;":"=m"(mainTask.regs.cr3)::"%eax");
    asm volatile("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;":"=m"(mainTask.regs.eflags)::"%eax");
 
    createTask(&otherTask, otherMain, mainTask.regs.eflags, (uint32_t*)mainTask.regs.cr3);
    mainTask.next = &otherTask;
    otherTask.next = &mainTask;
 
    runningTask = &mainTask;
    exit_debug_scope();
}

uint64_t prev_stack = 0x300000;
void* allocateStack() {
    prev_stack = prev_stack + 0x1000;
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
    task->regs.esp = (uint32_t) allocateStack() + 0x1000; // Not implemented here
    task->next = 0;
}
 
void yield() {
    Task *last = runningTask;
    runningTask = runningTask->next;
    switchTask(&last->regs, &runningTask->regs);
}

void doIt() {
    print_string("Switching to otherTask... \n\r");
    yield();
    print_string("Returned to mainTask!\n");
}