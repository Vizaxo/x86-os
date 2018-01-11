#include "kmain.h"
#include "vga.h"

void kmain(void)
{
	vga_print_char(0, 0x2f, 'a');
}
