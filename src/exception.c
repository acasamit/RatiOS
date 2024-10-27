#include "kernel.h"

void default_exception_handler() { cli();printk("\nException: Default exception handler called!\n");prints("\nException: Default exception handler called!\n");while(1); }

void handle_zero_division() { cli();printk("\nException: Dizide by zero!\n");prints("\nException: Dizide by zero!\n");while(1); }

void handle_overflow() { cli();printk("\nException: Overflow!\n");prints("\nException: Overflow!\n");while(1); }

void handle_ofb() { cli();printk("\nException: Out of Bounds\n");prints("\nException: Out of Bounds\n");while(1); }

void handle_invalid_opcode() { cli();printk("\nException: Invalid Opcode\n");prints("\nException: Invalid Opcode\n");while(1); }

void handle_segment_not_presend() { cli();printk("\nException: Segment Not Present\n");prints("\nException: Segment Not Present\n");while(1); }

void handle_stack_fault() { cli();printk("\nException: Stack Fault\n");prints("\nException: Stack Fault\n");while(1); }

void handle_gpf(uint32_t error_code)
{
	cli();
	uint32_t esp, cs, eip;
	asm volatile (
		"movl %%esp, %0\n"
		"movl %%cs, %1\n"
		"call 1f\n"
		"1: popl %2\n"
		: "=r"(esp), "=r"(cs), "=r"(eip)
	);
	printk("\nException: General Protection Fault! Error: %x\n", error_code);
	printk("ESP: %x CS: %x EIP: %x\n", esp ,cs, eip);
	prints("\nException: General Protection Fault! Error: %x\n", error_code);
	prints("ESP: %x CS: %x EIP: %x\n", esp ,cs, eip);
	while(1);
}
