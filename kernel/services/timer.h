#pragma once
#include <stdint.h>
#include "../utils/util.h"
#include "../gatr/vga.h"
#include "../interrupts/irq.h"
#include "mouse.h"
#include "proc.h"
void timer_phase(int hz);
void timer_install();
void sleep(int time_to_wait_seconds);
void timer_handler(struct regs *r);
int timer_ticks = 0;
int seconds = 0;