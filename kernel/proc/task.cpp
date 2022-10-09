// For real this time.

#include "task.h"
#include "../gatr/vga.h"
 
static Task *runningTask;
static Task mainTask;
static Task otherTask;
static Task yetAnotherTask;

static void otherMain() {
    print_string("Hello multitasking world!\n\r"); // Not implemented here...
    yield();
    print_string("Back in otherTask we are again!\n\r");
    yield();
}

static void yetAnother() {
    print_string("This is quite awesome!\n\r");
    yield();
    print_string("And to anotherTask!\n\r");
    yield();
}

void initTasking() {
    enter_debug_scope((char*)"init_tasking");
    // Get EFLAGS and CR3
    asm volatile("movl %%cr3, %%eax; movl %%eax, %0;":"=m"(mainTask.regs.cr3)::"%eax");
    asm volatile("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;":"=m"(mainTask.regs.eflags)::"%eax");
 
    createTask(&otherTask, otherMain, mainTask.regs.eflags, (uint32_t*)mainTask.regs.cr3);
    createTask(&yetAnotherTask, yetAnother, mainTask.regs.eflags, (uint32_t*)mainTask.regs.cr3);
    mainTask.next = &otherTask;
    otherTask.next = &yetAnotherTask;
    yetAnotherTask.next = &mainTask;
 
    runningTask = &mainTask;
    exit_debug_scope();
}

void broken_process() {
    print_string("Broken Process\n\r");
    yield();
    for (;;);
}

uint64_t prev_stack = 0x300000;
void* allocateStack() {
    prev_stack = prev_stack + 0x1000;
    *(uint32_t*)(prev_stack - 4) = (uint32_t)&broken_process;
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
}
 
void yield() {
    Task *last = runningTask;
    runningTask = runningTask->next;
    switchTask(&last->regs, &runningTask->regs);
}

void doIt() {
    print_string("Switching to otherTask... \n\r");
    yield();
    print_string("Returned to mainTask!\n\r");
    yield();
    print_string("Welcome back!\n\r");
}