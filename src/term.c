#include "kernel.h"

const uint32_t VGA_WIDTH = 80;
const uint32_t VGA_HEIGHT = 25;

uint16_t screen_1[2001] = {0};
uint16_t screen_2[2001];

uint8_t screen_selected = 1;

t_xy cursor_pos = {0, 0};

uint8_t terminal_row;
uint8_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void term_init()
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_MAGENTA, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*)0xB8000;
	for (int i = 0; i != 2000; i++)
	{
		terminal_buffer[i] = vga_entry(0, terminal_color);
		screen_1[i] = vga_entry(0, terminal_color);
		screen_2[i] = vga_entry(0, terminal_color);
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, uint32_t x, uint32_t y)
{
	int index = y * VGA_WIDTH + x;
	if (screen_selected == 1 && !lock)
		screen_1[index] = vga_entry(c, color);
	else if (!lock)
		screen_2[index] = vga_entry(c, color);
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_delchar()
{
	if (!terminal_column && !terminal_row)
		return;
	if (!terminal_column)
	{
		terminal_column = VGA_WIDTH;
		terminal_row--;
	}
	else
		terminal_column--;
	terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
}

void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void update_current_screen()
{
	uint16_t *tab;

	if (screen_selected == 1)
		tab = screen_1;
	else
		tab = screen_2;
	for (int i = 0; i != 2000; i++)
		tab[i] = vga_entry(terminal_buffer[i] & 0x00FF, terminal_color);
}

void terminal_scroll(char c)
{
	int pos = 79;

	while (++pos != 2000)
		terminal_buffer[pos - 80] = terminal_buffer[pos];

	pos -= VGA_WIDTH + 1;
	while (++pos != 2000)
		terminal_buffer[pos] = vga_entry(0, terminal_color);

	terminal_column = 0;
	terminal_row = 24;
	if (c != '\n')
	{
		terminal_putchar(c);
		terminal_set_cursor(1, 24);
	}
	else
		terminal_set_cursor(0, 24);
	update_current_screen();
}

void terminal_refresh()
{
	uint16_t *tab;

	if (screen_selected == 1)
		tab = screen_1;
	else
		tab = screen_2;
	for (int i = 0; i != 2000; i++)
		terminal_buffer[i] = vga_entry(tab[i] & 0x00FF, terminal_color);
}

void terminal_switch()
{
	uint16_t *tab;
	t_xy saved_cursor = {terminal_column, terminal_row};
	terminal_column = cursor_pos.x;
	terminal_row = cursor_pos.y;
	cursor_pos = saved_cursor;
	screen_selected = (screen_selected == 1) ? 2 : 1;
	terminal_refresh();
}

void terminal_clear(uint8_t screen)
{
	uint16_t *tab;

	if (screen == 1)
		tab = screen_1;
	if (screen == 2)
		tab = screen_2;
	if (screen)
	{
		for (int i = 0; i != 2000; i++)
			tab[i] = vga_entry(0, terminal_color);
		terminal_column = 0;
		terminal_row = 0;
	}
	for (int i = 0; i != 2000; i++)
		terminal_buffer[i] = vga_entry(0, terminal_color);
}

void terminal_putchar(char c)
{
	int index = terminal_row * VGA_WIDTH + terminal_column;
	if (index >= 2000)
	{
		terminal_scroll(c);
		return;
	}
	if (c == '\n')
	{
		terminal_column = 0;
		if (++terminal_row == 25)
			terminal_scroll('\n');
		return;
	}
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (terminal_column == VGA_WIDTH - 1)
	{
		terminal_column = 0;
		terminal_row++;
	}
	else
		terminal_column++;
}

void writek(const char* data, uint32_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_set_cursor(uint8_t x, uint8_t y)
{
	uint16_t position = y * VGA_WIDTH + x;
	if (position == 2000)
		return;
	if (position < 0 || position > 2000)
	{
		terminal_column = 0;
		terminal_row = 0;
		position = 0;
	}
	outb(P_VGA_COMMAND, 0x0E);
	outb(P_VGA_DATA, (position >> 8) & 0xFF);
	outb(P_VGA_COMMAND, 0x0F);
	outb(P_VGA_DATA, position & 0xFF);
}
