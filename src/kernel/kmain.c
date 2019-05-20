#include "kmain.h"
#include "vga.h"
#include "term.h"
#include "idt.h"
#include "pic.h"

void kmain(void)
{
	setup_idt();
	pic_remap();
	term_putstr("Hi there!\nHello!\n");
	asm("sti"); //enable hardware interrupts
	asm("int3"); //fire interrupt
	term_putstr("Didn't crash");
}
