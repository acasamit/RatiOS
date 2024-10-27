#include "kernel.h"

uint8_t lock = 0;

void lock_screen()
{
	t_xy pos_save;
	pos_save.x = terminal_column;
	pos_save.y = terminal_row;

	terminal_clear();
	terminal_column = 35;
	terminal_row = 11;
	printk("Password:");
	terminal_column = 37;
	terminal_row = 12;
	printk("_____");
	terminal_column = 37;
	terminal_row = 12;
	disable_cursor();
	while(lock);
	terminal_column = pos_save.x;
	terminal_row = pos_save.y;
	terminal_refresh();
	enable_cursor(14, 15);
}

void kernel_main()
{
	kinit();
	printk("Welcome to RatiOS! %d\n", 42);
	terminal_set_cursor(terminal_column, terminal_row);
	while(1)
	{
		if (lock == 1)
			lock_screen();
	}
}
