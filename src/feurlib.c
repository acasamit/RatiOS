#include "kernel.h"

void outb(uint16_t port, uint8_t value)
{
	__asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port)
{
	uint8_t result;
	__asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
	return result;
}

void cli()
{
	asm volatile("cli");
}

void sti()
{
	asm volatile("sti");
}

void io_wait()
{
	outb(0x80, 0);
}

uint32_t strlen(const char* str)
{
	uint32_t len = -1;
	while (str[++len]) {}
	return len;
}

void putnbr(int num, int serial)
{
	if (num < 0)
	{
		if (serial)
			outb(P_SERIAL, '-');
		else
			terminal_putchar('-');
		num *= -1;
	}
	if (num > 9)
	{
		putnbr(num / 10, serial);
		num %= 10;
	}
	if (serial)
		outb(P_SERIAL, num + '0');
	else
		terminal_putchar(num + '0');
}

void puthex(int num, int serial)
{
	char buffer[11];
	const char *hex_digits = "0123456789ABCDEF";
	int i, temp;

	buffer[0] = '0';
	buffer[1] = 'x';

	for (i = 0; i < 8; i++)
	{
		temp = (num >> (28 - i * 4)) & 0xF;
		buffer[2 + i] = hex_digits[temp];
	}
	buffer[10] = '\0';
	if (serial)
		prints("%s", buffer);
	else
		printk("%s", buffer);
}

void printk(const char* str, ...)
{
	int i = -1; 
	int* arg_ptr = (int*)(&str + 1);

	while(str[++i])
	{
		if (str[i] == '%')
		{
			if (str[i + 1] == 'd')
				putnbr(*arg_ptr, 0);
			else if (str[i + 1] == 'x')
				puthex(*arg_ptr, 0);
			else if (str[i + 1] == 'c')
				terminal_putchar(*arg_ptr);
			else if (str[i + 1] == 's')
			{
				if (!*arg_ptr)
					writek("(null)", 5);
				else
					writek((const char *)*arg_ptr, strlen((const char *)*arg_ptr));
			}
			if (str[i + 1] == 'd' || str[i + 1] == 's' || str[i + 1] == 'c' || str[i + 1] == 'x')
			{
				arg_ptr++;
				i++;
				continue;
			}
		}
		terminal_putchar(str[i]);
	}
}

void write_serial(char *str)
{
	for (int i = 0; str[i]; i++)
		outb(P_SERIAL, str[i]);
}

void prints(const char* str, ...)
{
	int i = -1; 
	int* arg_ptr = (int*)(&str + 1);

	while(str[++i])
	{
		if (str[i] == '%')
		{
			if (str[i + 1] == 'd')
				putnbr(*arg_ptr, 1);
			else if (str[i + 1] == 'x')
				puthex(*arg_ptr, 1);
			else if (str[i + 1] == 'c')
				outb(P_SERIAL, *arg_ptr);
			else if (str[i + 1] == 's')
			{
				if (!*arg_ptr)
					write_serial("(null)");
				else
					write_serial((char *)*arg_ptr);
			}
			if (str[i + 1] == 'd' || str[i + 1] == 's' || str[i + 1] == 'c' || str[i + 1] == 'x')
			{
				arg_ptr++;
				i++;
				continue;
			}
		}
		outb(P_SERIAL, str[i]);
	}
}

void *memset(void *dest, int val, uint32_t len)
{
	unsigned char *ptr = dest;
	while (len-- > 0)
		*ptr++ = val;
	return dest;
}

void *memmove(void *dest, const void *src, uint32_t len)
{
	unsigned char *d = dest;
	const unsigned char *s = src;
	if (d < s) {
		while (len--)
			*d++ = *s++;
	} else {
		const unsigned char *lasts = s + (len - 1);
		unsigned char *lastd = d + (len - 1);
		while (len--)
			*lastd-- = *lasts--;
	}
	return dest;
}

void PIC_sendEOI(uint8_t irq)
{
	if(irq >= 8)
		outb(PIC2_COMMAND,PIC_EOI);
	outb(PIC1_COMMAND,PIC_EOI);
}

int strcmp(char *str1, char *str2)
{
	int i = 0;

	while ((str1[i] == str2[i]) && str1[i])
		i++;
	return (str1[i] - str2[i]);
}
