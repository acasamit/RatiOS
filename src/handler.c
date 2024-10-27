#include "kernel.h"

#define UP_ARROW 72
#define RIGHT_ARROW 77
#define LEFT_ARROW 80
#define DOWN_ARROW 75
#define CTRL 29
#define RELEASE_CTRL 0x9D

unsigned const char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    29,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    LEFT_ARROW,	/* Left Arrow */
    0,
    RIGHT_ARROW,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};		

void handle_arrow(char c)
{
	if (c == RIGHT_ARROW)
	{
		if (terminal_row * VGA_WIDTH + terminal_column == 2000)
			return;
		if (terminal_buffer[terminal_row * VGA_WIDTH + terminal_column] & 0xFF)
		{
			if (terminal_column == VGA_WIDTH - 1 && terminal_row != VGA_HEIGHT - 1)
			{
				terminal_column = 0;
				terminal_row++;
			}
			else if (terminal_column != VGA_WIDTH)
				terminal_column++;
		}
	}
	else if (c == LEFT_ARROW)
	{
		if (terminal_row || terminal_column)
		{
			if (!terminal_column)
			{
				terminal_row--;
				terminal_column = VGA_WIDTH - 1;
			}
			else
				terminal_column--;
		}
	}
}

void hanlde_control(char c)
{
	static uint8_t color = 5;
	if (c == 'l')
		lock = 1;
	else if (c == 'c')
	{
		if (++color == 16)
			color = 1;
		terminal_color = vga_entry_color(color, VGA_COLOR_BLACK);
		terminal_refresh();
	}
	else if (c == 's')
		terminal_switch();
}

char passwd[6] = {0};

void handle_lock(char c)
{
	static pos = 0;

	passwd[pos] = c;
	terminal_putentryat('*', terminal_color, 37 + pos, 12);
	pos++;
	if (pos == 5)
	{
		pos = 0;
		if (!strcmp(passwd, "ratio"))
		{
			memset(passwd, 0, 6);
			lock = 0;
		}
		else
		{
			memset(passwd, 0, 6);
			terminal_column = 37;
			terminal_row = 12;
			printk("_____");
			terminal_column = 37;
			terminal_row = 12;
		}
	}
}

void keyboard_handler()
{
	unsigned char scancode = inb(0x60);
	static int extended = 0;
	static uint8_t ctrl = 0;
	if (scancode == 0xE0)
	{
		extended = 1;
		return;
	}
	else if (scancode & 0x80)
	{
		if (scancode == RELEASE_CTRL)
			ctrl = 0;
	}
	else if (scancode > 127 || !kbdus[scancode])
		return;
	else if (lock)
		handle_lock(kbdus[scancode]);
	else
	{
		if (extended)
		{
			if (kbdus[scancode] == RIGHT_ARROW || kbdus[scancode] == LEFT_ARROW)
				handle_arrow(kbdus[scancode]);
			else if (kbdus[scancode])
				terminal_putchar(kbdus[scancode]);
			extended = 0;
		}
		else if (ctrl)
			hanlde_control(kbdus[scancode]);
		else if (kbdus[scancode] == CTRL)
			ctrl = 1;
		else if (kbdus[scancode] == '\b')
			terminal_delchar();
		else
			terminal_putchar(kbdus[scancode]);
		terminal_set_cursor(terminal_column, terminal_row);
	}
}
