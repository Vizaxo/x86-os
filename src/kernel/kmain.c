#include "kmain.h"
#include "vga.h"
#include "term.h"
#include "idt.h"

void kmain(void)
{
	setup_idt();
	term_putstr("Hi there!\nHello!\n");
	asm("int3"); //fire interrupt
	term_putstr("Didn't crash");
}
