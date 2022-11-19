// For real this time.

#include "task.h"
#include "../gatr/vga.h"
#include "../services/debug.h"
#include "../memory/heap.h"

Task* find_task(uint64_t tid) { // WARNING: if the task with the specified TID does not exist this function runs forever
    Task* task = &mainTask;
    while (task->tid != tid) {
        task = task->next;
    }
    return task;
}

void wait() {
    runningTask->locked = true;
    yield();
}

void signal(uint64_t tid) {
    Task* task = find_task(tid);
    if (task->locked) task->locked = false;
}

void shmcreat(uint64_t key, size_t size) {
    SHM* memory = &shared_mem;
    while (memory->next != NULL) {
        memory = memory->next;
    }
    SHM* new_mem = (SHM*)malloc(sizeof(SHM));
    memory->next = new_mem;
    new_mem->key = key;
    new_mem->ptr = malloc(size);
}

void* shmget(uint64_t key) {
    SHM* memory = &shared_mem;
    while (memory->key != key && memory->next != NULL) {
        memory = memory->next;
    }
    if (memory == NULL) return NULL;
    return memory->ptr;
}

void shmdel(uint64_t key) {
    SHM* memory = &shared_mem;
    SHM* prev = NULL;
    while (memory->key != key && memory->next != NULL) {
        prev = memory;
        memory = memory->next;
    }
    if (memory == NULL) return;
    free(memory->ptr);
    prev->next = memory->next;
    free(memory);
}

void fork(Task* task, void(*func)()) {
    createTask(task,func,runningTask->regs.eflags,(uint32_t*)runningTask->regs.cr3);
    task->next = runningTask->next;
    runningTask->next = task;
    task->ptid = runningTask->tid;
}

Task* fork(void(*func)()) {
    Task* t = (Task*)malloc(sizeof(Task));
    fork(t, func);
    t->onHeap = true;
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
    if (runningTask->onHeap) {
        free(runningTask);
    }
    yield();
}

void idle() {
    while (1) {
        asm("nop");
    }
}

void initTasking() {
    enter_debug_scope((char*)"init_tasking");
    // Get EFLAGS and CR3
    __asm__ __volatile__("movl %%cr3, %%eax; movl %%eax, %0;":"=m"(mainTask.regs.cr3)::"%eax");
    __asm__ __volatile__("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;":"=m"(mainTask.regs.eflags)::"%eax");
    mainTask.tid = 0;

    mainTask.next = &mainTask;
    mainTask.quantum = 20; // Kernel gets extra quantum
    mainTask.current_quantum = 20; 
    runningTask = &mainTask;

    Task* idle_task = fork(idle);
    idle_task->quantum = 2; // Idle gets 2 ms quantum

    shared_mem.key = 0x1337c0de;

    exit_debug_scope();
}

void process_end() {
    quit();
    for (;;);
}

uint64_t prev_stack = 0x400000;
void* allocateStack() {
    enter_debug_scope((char*)"stack allocation");
    prev_stack = prev_stack + 0x1000;
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
    task->regs.esp = (uint32_t) allocateStack()-4;
    task->regs.ebp = task->regs.esp+4;
    task->next = (Task*)0;
    task->running = true;
    task->time_used = 0;
    task->state = READY;
    task->tid = newest_tid;
    task->quantum = 10;
    task->current_quantum = 0;
    task->locked = false;
    task->onHeap = false;
    newest_tid++;
}

void lock_scheduler() {
    irq_lock_counter = 1;
}

void unlock_scheduler() {
    irq_lock_counter = 0;
}

uint64_t start_time = 0;
void yield() {
    yield(READY);
}

void yield(TaskState ts) {
    Task *last = runningTask;
    last->time_used += timer_ticks - start_time;
    start_time = timer_ticks;
    runningTask = runningTask->next;
    while ((runningTask->state != READY && runningTask->state != RUNNING)  || runningTask->locked) {
        runningTask = runningTask->next;
    }
    runningTask->current_quantum = runningTask->quantum;
    if (runningTask->state == RUNNING) return; // task already current
    last->state = ts;
    runningTask->state = RUNNING;
    switchTask(&last->regs, &runningTask->regs);
}

void schedule() {
    Task *last = runningTask;
    last->time_used += timer_ticks - start_time;
    start_time = timer_ticks;
    runningTask = runningTask->next;
    while ((runningTask->state != READY && runningTask->state != RUNNING) || runningTask->locked) {
        runningTask = runningTask->next;
    }
    runningTask->current_quantum = runningTask->quantum;
    if (runningTask->state == RUNNING) return; // task already current
    last->state = READY;
    runningTask->state = RUNNING;
    switchTask(&last->regs, &runningTask->regs);
}