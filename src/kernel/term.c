#include "term.h"
#include "vga.h"

struct term_state {
	size_t col;
	size_t row;
	uint8_t color;
};

//Terminal state variable. This is modified by the terminal functions.
struct term_state state = {0, 0, 0x0f};

void term_putchar(char c) {
	if (c == '\n') {
		term_newline();
	} else {
		size_t offset = (state.row * VGA_WIDTH) + state.col;
		vga_print_char(offset, state.color, (uint8_t)c);
		if (++state.col >= VGA_WIDTH)
			term_newline();
	}
}

void term_newline(void) {
	state.col = 0;
	if (++state.row >= VGA_HEIGHT)
		state.row = 0;
}

void term_putstr(const char* str) {
	while(*str != '\0')
		term_putchar(*(str++));
}
