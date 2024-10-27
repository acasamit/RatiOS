#pragma once

typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int		uint32_t;
typedef unsigned long long	uint64_t;
typedef unsigned long long	size_t;

typedef signed char			int8_t;
typedef signed short		int16_t;
typedef signed int			int32_t;
typedef signed long long	int64_t;
typedef signed long long	ssize_t;

#define NULL ((void*)0)
extern uint8_t lock;

typedef struct	s_xy
{
	uint8_t	x;
	uint8_t	y;
}				t_xy;

/*Ports defines*/
#define P_SERIAL 0x3F8
#define P_VGA_COMMAND 0x3D4
#define P_VGA_DATA 0x3D5
#define P_PS2_KBD 0x60

/*PIC defines*/
#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	0x20
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	0xA0
#define PIC2_DATA	(PIC2+1)
#define PIC_EOI		0x20

#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

/*Kernel Initialisation Structures*/
#define SEGMENT_NUMBER 7
#define ITR_NUMBER 256

struct gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

struct idt_entry {
	uint16_t base_low;
	uint16_t selector;
	uint8_t always0;
	uint8_t flags;
	uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

void kinit();

/*Interruption handlers*/
struct interrupt_frame;
__attribute__ ((interrupt)) void irq0(struct interrupt_frame* frame);
void keyboard_handler();
__attribute__ ((interrupt)) void irq2(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void irq3(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void irq4(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void irq5(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void irq6(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void irq7(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void irq8(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void irq9(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void irq10(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void irq11(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void irq12(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void irq13(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void irq14(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void irq15(struct interrupt_frame* frame);

/*Exception handler*/
void default_exception_handler();
void handle_zero_division();
void handle_overflow();
void handle_ofb();
void handle_invalid_opcode();
void handle_segment_not_presend();
void handle_stack_fault();
void handle_gpf(uint32_t error_code);

/*VGA variables*/
#define VGA_CMD_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5
extern const uint32_t VGA_WIDTH;
extern const uint32_t VGA_HEIGHT;

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

struct regs {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int int_nb, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
};

/*VGA functions*/
uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
uint16_t vga_entry(unsigned char uc, uint8_t color);

/*Terminal functions*/
void term_init();
void terminal_setcolor(uint8_t color);
void terminal_putentryat(char c, uint8_t color, uint32_t x, uint32_t y);
void terminal_putchar(char c);
void writek(const char* data, uint32_t size);
void terminal_set_cursor(uint8_t x, uint8_t y);
void terminal_delchar();
void terminal_switch();
void terminal_clear();
void disable_cursor();
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void terminal_refresh();

/*Terminal variable*/
extern uint8_t terminal_row;
extern uint8_t terminal_column;
extern uint16_t* terminal_buffer;
extern uint8_t terminal_color;
extern uint8_t screen_selected;

/*Feurlib functions*/
uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);
void cli();
void sti();
void io_wait();
uint32_t strlen(const char* str);
void putnbr(int num, int serial);
void puthex(int num, int serial);
void printk(const char* str, ...);
void prints(const char* str, ...);
void write_serial(char *str);
void *memset(void *dest, int val, uint32_t len);
void PIC_sendEOI(uint8_t irq);
int strcmp(char *str1, char *str2);
