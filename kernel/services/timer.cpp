#include "timer.h"
#include "../ring3/tss.h"
#include "../proc/task.h"

void timer_phase(int hz)
{
	int divisor = 1193180 / hz;	   /* Calculate our divisor */
	outb(0x43, 0x36);			 /* Set our command byte 0x36 */
	outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
	outb(0x40, divisor >> 8);	 /* Set high byte of divisor */
}

void timer_handler(struct regs *r)
{
    //set_kernel_stack(r->esp); // Eventually i will add this
    /* Increment our 'tick count' */
    timer_ticks++;
    swap_buffers();
    end_interrupt(r);
    if (tasking) schedule();
}

void sleep (int ticks){
    int startTicks = timer_ticks;
    while(timer_ticks < startTicks + ticks){
        asm("nop");
    }
    return;
}

void timer_install()
{
    print_string("Installing timer...      \r");
    irq_install_handler(0, timer_handler);
    timer_phase(100);
}

uint64_t read_counter() {
    return timer_ticks;
}