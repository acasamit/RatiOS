#pragma once

typedef unsigned long long	size_t;
typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int		uint32_t;
typedef unsigned long long	uint64_t;

typedef signed char			int8_t;
typedef signed short		int16_t;
typedef signed int			int32_t;
typedef signed long long	int64_t;

typedef struct	s_xy
{
	int	x;
	int	y;
}				t_xy;

/*VGA variables*/
#define VGA_CMD_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

/*VGA functions*/
unsigned int vga_entry_color(enum vga_color fg, enum vga_color bg);
unsigned int vga_entry(unsigned char uc, unsigned int color);

/*Terminal functions*/
void terminal_initialize(void);
void terminal_setcolor(unsigned int color);
void terminal_putentryat(char c, unsigned int color, size_t x, size_t y);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_set_cursor (int x, int y);

/*Feurlib functions*/
void outb(uint16_t port, uint8_t value);
size_t strlen(const char* str);
