#include "kmain.h"
#include "vga.h"
#include "term.h"

void kmain(void)
{
	for (int i = 0; i <= 40; i++) {
		term_putchar('a' + i);
		term_putchar('b');
		term_putchar('\n');
	}
	term_putstr("Hi there!\nHello!");
}
