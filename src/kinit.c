#include "kernel.h"

struct gdt_entry gdt[SEGMENT_NUMBER] __attribute__((section(".gdt")));
struct gdt_ptr gdtp;
struct idt_entry idt[48] = {0};
struct idt_ptr idtp;
unsigned int handler[48] = {0};

extern void irq1();
extern void gdt_load();

void gdt_set()
{
	gdtp.limit = (sizeof(struct gdt_entry) * SEGMENT_NUMBER) - 1;
	gdtp.base = (uint32_t)&gdt;

	struct gdt_entry null_seg = {0};
	gdt[0] = null_seg;

	gdt[1] = (struct gdt_entry){
	.limit_low = 0xFFFF,
	.base_low = 0x0000,
	.base_middle = 0x00,
	.access = 0x9A,
	.granularity = 0xCF, // 32b
	.base_high = 0x00};

	gdt[2] = (struct gdt_entry){
	.limit_low = 0xFFFF,
	.base_low = 0x0000,
	.base_middle = 0x00,
	.access = 0x92,
	.granularity = 0xCF, // 32b
	.base_high = 0x00};

	gdt_load();
}

void idt_set_gate()
{
	static uint8_t num = 0;
	unsigned int base = (unsigned)default_exception_handler;

	if (handler[num])
		base = handler[num];

	idt[num].base_low = (base & 0xFFFF);
	idt[num].base_high = (base >> 16) & 0xFFFF;
	idt[num].selector = 0x8;
	idt[num].always0 = 0;
	idt[num].flags = 0b10001110;

	num++;
}

void pic_remap(int offset1, int offset2)
{
	uint8_t a1, a2;
	
	a1 = inb(PIC1_DATA);
	a2 = inb(PIC2_DATA);
	
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);
	io_wait();
	outb(PIC2_DATA, offset2);
	io_wait();
	outb(PIC1_DATA, 4);
	io_wait();
	outb(PIC2_DATA, 2);
	io_wait();
	
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
	
	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}

void set_handler_table()
{
	handler[0] = (unsigned)handle_zero_division;
	handler[4] = (unsigned)handle_overflow;
	handler[5] = (unsigned)handle_ofb;
	handler[6] = (unsigned)handle_invalid_opcode;
	handler[11] = (unsigned)handle_segment_not_presend;
	handler[12] = (unsigned)handle_stack_fault;
	handler[13] = (unsigned)handle_gpf;
	handler[33] = (unsigned)irq1;
}

void IRQ_set_mask(uint8_t IRQline)
{
	uint16_t port;
	uint8_t value;

	if(IRQline < 8) {
		port = PIC1_DATA;
	} else {
		port = PIC2_DATA;
		IRQline -= 8;
	}
	value = inb(port) | (1 << IRQline);
	outb(port, value);
}

void kinit()
{
	gdt_set();

	idtp.limit = (sizeof (struct idt_entry) * ITR_NUMBER) - 1;
	idtp.base = (uint32_t)&idt;
	set_handler_table();
	pic_remap(32, 40);

	for(int i = 0; i != 48; i++)
		idt_set_gate();

	asm volatile("lidt %0" : : "m" (idtp));

	for (uint8_t i = 0; i != 16; i++)
	{
		if (!handler[i + 32])
			IRQ_set_mask(i);
	}

	asm volatile("sti");
	term_init();
}
