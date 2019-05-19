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

void vga_copy_line(size_t line, size_t newpos) {
	uint16_t *buffer = (uint16_t *)VGA_BUFFER;
	for(size_t i = 0; i <= VGA_WIDTH; i++)
		buffer[(newpos * VGA_WIDTH) + i] = buffer[(line * VGA_WIDTH) + i];
}

void vga_clear_line(size_t line) {
	uint16_t *buffer = (uint16_t *)VGA_BUFFER;
	for(size_t i = 0; i <= VGA_WIDTH; i++)
		buffer[(line * VGA_WIDTH) + i] = 0x00;
}
