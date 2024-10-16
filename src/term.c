#include "kernel.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

t_xy cursor_pos = {0, 0};

size_t terminal_row;
size_t terminal_column;
unsigned int terminal_color;
unsigned int* terminal_buffer;

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_MAGENTA, VGA_COLOR_BLACK);
	terminal_buffer = (unsigned int*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(unsigned int color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, unsigned int color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c)
{
	if (c == '\n')
	{
		terminal_column = 0;
		terminal_row++;
		cursor_pos.x = 0;
		cursor_pos.y++;
		return;
	}
	cursor_pos.x++;
	if (!(cursor_pos.x % 2))
		cursor_pos.x++;
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}

void terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
	terminal_set_cursor(cursor_pos.x, cursor_pos.y);
}

void terminal_set_cursor (int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x + 1;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}
