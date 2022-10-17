#ifndef __TASK_H__
#define __TASK_H__
 
#include <stdint.h>
 
extern void initTasking();
 
typedef struct {
    uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
} Registers;
 
enum TaskState {
    RUNNING,
    READY,
    WAITING
};

typedef struct Task {
    Registers regs;
    struct Task *next;
    bool running;
    TaskState state;
    uint64_t tid;
    uint64_t ptid;
    uint64_t time_used;
    uint8_t quantum;
    char current_quantum;
    bool locked;
    bool onHeap;
} Task;

typedef struct SHM {
    uint64_t key;
    void* ptr;
    SHM* next;
} SHM;

uint64_t newest_tid = 1; // Newest Task ID

static Task *runningTask;
static Task mainTask;
static SHM shared_mem;
uint64_t irq_lock_counter = 0;

extern void createTask(Task*, void(*)(), uint32_t, uint32_t*);
extern void fork(Task* task, void(*func)());
extern Task* fork(void(*func)());
extern void wait();
extern void signal(uint64_t tid);
extern void quit();
extern void shmcreat(uint64_t key, size_t size);
extern void* shmget(uint64_t key);
extern void shmdel(uint64_t key);
 
void lock_scheduler();
void unlock_scheduler();

extern void yield(); // Switch task frontend
extern void yield(TaskState);
extern "C" void switchTask(Registers*, Registers*); // The function which actually switches
extern void schedule();

void doIt();
 
#endif /* __TASK_H__ */