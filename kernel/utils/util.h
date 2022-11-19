#pragma once
#include <stdint.h>
uint32_t strlen(const char*);
uint32_t digit_count(int);
void itoa(int, char *);
uint8_t inb(uint16_t);
uint16_t inw(uint16_t);
uint32_t inl(uint16_t);
void outb(uint16_t, uint8_t);
void outw(uint16_t, uint16_t);
void outl(uint16_t, uint32_t);
char* num_to_char(uint32_t num);
void io_wait();
bool strcmp(const char*, const char*);
char* strcpy(char*, const char*, uint32_t);
char* strcat(char*, const char*, uint32_t);
uint32_t rdtsc();