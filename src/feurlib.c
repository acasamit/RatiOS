#include "kernel.h"

void outb(uint16_t port, uint8_t value)
{
	__asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

size_t strlen(const char* str) 
{
	size_t len = -1;
	while (str[++len]) {}
	return len;
}