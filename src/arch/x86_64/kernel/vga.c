#include <stdint.h>
#include <stddef.h>

#include "vga.h"

#define VGA_BUFFER 0xB8000;

void vga_print_char(size_t offset, uint8_t color, uint8_t c)
{
	uint16_t *buffer = (uint16_t *)VGA_BUFFER;
	buffer += offset;
	*buffer = (uint16_t)((uint16_t) color << 8 | (uint16_t) c);
}
