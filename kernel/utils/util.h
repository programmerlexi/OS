#pragma once
#include <stdint.h>
extern uint32_t strlen(const char*);
extern uint32_t digit_count(int);
extern void itoa(int, char *);
extern uint8_t inb(uint16_t);
extern void outb(uint16_t, uint8_t);
extern void outw(uint16_t, uint8_t);
char* num_to_char(uint8_t num);
void io_wait();
bool strcmp(const char*, const char*);
char* strcpy(char*, const char*, uint32_t);
char* strcat(char*, const char*, uint32_t);