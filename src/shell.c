#include "kernel.h"

char command[78] = {0};

void do_test()
{
	char str[] = "ceci est un test";
	int i = 64;


	print_memory(str, strlen(str));
	terminal_putchar('\n');
	print_memory(&i, 1);
}

void check_command()
{
	int k = -1;

	prints("%s\n", command);
	while(command[++k] && command[k] != ' ');
	if (k)
		k--;
	if (!strncmp(command, "test", k))
		do_test();
	else if (!strncmp(command, "echo", k))
		printk("%s\n", command + 5);
	else if (!strncmp(command, "lock", k))
	{
		printk(">");
		lock = 1;
	}
	else if (!strncmp(command, "clear", k))
		terminal_clear(screen_selected);
	else if (!strncmp(command, "halt", k))
		halt = 0;
	else if (!strncmp(command, "reboot", k))
		outb(0x64, 0xFE);
	else if (!strncmp(command, "exit", k) || !strncmp(command, "shutdown", k))
		outw(P_ACPI, 0x2000);
	else
	{
		for(int j = 0; j != k; j++) terminal_putchar(command[j]);
		printk(" command not found\n");
	}
}

void shell(char c)
{
	static int i = 0;

	if (c == '\b' && i)
	{
		command[i - 1] = 0;
		i--;
		return;
	}
	if (c == '\n' || i == 78)
	{
		i = 0;
		terminal_putchar('\n');
		check_command();
		terminal_putchar('>');
		memset(command, 0, 78);
		return;
	}
	command[i] = c;
	i++;
}
