#include "timer.h"
#include "../ring3/tss.h"

void timer_phase(int hz)
{
	int divisor = 1193180 / hz;	   /* Calculate our divisor */
	outb(0x43, 0x36);			 /* Set our command byte 0x36 */
	outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
	outb(0x40, divisor >> 8);	 /* Set high byte of divisor */
}

void timer_handler(struct regs *r)
{
    //set_kernel_stack(r->esp);
    /* Increment our 'tick count' */
    timer_ticks++;
    swap_buffers();
    proc_cycle();

    /* If the next tick is going to be beyond the next second,
        * we need to switch to the next second. */
    if (timer_ticks % 18 == 0)
    {
    	seconds++;
        /*print_string("The System is running for ");
	    print_string(num_to_char(seconds));
        print_string(" Seconds");
        set_cursor_pos(vga_pos - (26 + strlen(num_to_char(seconds)) + 8));*/
    }
}

void sleep(int time_to_wait_seconds) {
    int start = seconds;
    while (seconds < time_to_wait_seconds + start) {
        
    }
}

void timer_install()
{
    print_string("Installing timer...      \r");
    irq_install_handler(0, timer_handler);
    timer_phase(100);
}