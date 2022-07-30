#include "util.h"
uint32_t strlen(const char* str)
{
  uint32_t length = 0;
  while(str[length] != '\0')
    length++;
  return length;
}

bool strcmp(const char* str1, const char* str2)
{
  uint32_t length = strlen(str1);
  uint32_t length2 = strlen(str2);
  if (length != length2)
    return false;
  for (uint32_t i = 0; i < length; i++)
    if (str1[i] != str2[i])
      return false;
  return true;
}

uint32_t digit_count(int num)
{
  uint32_t count = 0;
  if(num == 0)
    return 1;
  while(num > 0){
    count++;
    num = num/10;
  }
  return count;
}

void itoa(int num, char *number)
{
  int dgcount = digit_count(num);
  int index = dgcount - 1;
  char x;
  if(num == 0 && dgcount == 1){
    number[0] = '0';
    number[1] = '\0';
  }else{
    while(num != 0){
      x = num % 10;
      number[index] = x + '0';
      index--;
      num = num / 10;
    }
    number[dgcount] = '\0';
  }
}

uint8_t inb(uint16_t port)
{
  uint8_t data;
  asm volatile("inb %1, %0" : "=a"(data) : "Nd"(port));
  return data;
}

void outb(uint16_t port, uint8_t data)
{
  asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

void outw(uint16_t port, uint16_t data)
{
  asm volatile("outw %0, %1" : : "a"(data), "Nd"(port));
}

uint16_t inw(uint16_t port)
{
  uint16_t data;
  asm volatile("inw %1, %0" : "=a"(data) : "Nd"(port));
  return data;
}

char* num_to_char(uint8_t num) {
  char* c = 0;
  itoa(num, c);
  return c;
}

void io_wait(){
    asm volatile ("outb %%al, $0x80" : : "a"(0));
}

char* strcpy(char* dest, const char* src, uint32_t length)
{
  for (uint32_t i = 0; i < length; i++)
    dest[i] = src[i];
  return dest;
}

char* strcat(char* dest, const char* src, uint32_t length)
{
  int dest_length = strlen(dest);
  for (uint32_t i = 0; i < length; i++)
    dest[i+dest_length] = src[i];
  return dest;
}