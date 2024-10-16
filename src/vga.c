#include "kernel.h"

unsigned int vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}

unsigned int vga_entry(unsigned char uc, unsigned int color) 
{
	return (unsigned int) uc | (unsigned int) color << 8;
}