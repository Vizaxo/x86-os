#ifndef __VGA_H__
#define __VGA_H__

#include <stdint.h>
#include <stddef.h>

void vga_print_char(uint64_t offset, uint8_t color, uint8_t character);

#endif //__VGA_H__
