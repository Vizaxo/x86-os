#include "term.h"
#include "vga.h"

struct term_state {
	size_t col;
	size_t row;
	uint8_t color;
};

//Terminal state variable. This is modified by the terminal functions.
struct term_state state = {0, 0, 0x0f};

uint8_t kernel_quit = 0;


void term_putchar(char c) {
	switch (c) {
	case '\n':
		term_newline();
		break;
	case '\b':
		term_backspace();
		break;
	case 'q':
		kernel_quit = 1;
		break;
	default:
		term_normalchar(c);
	}
}

void term_newline(void) {
	state.col = 0;
	if (++state.row >= VGA_HEIGHT) {
		for (size_t i = 1; i <= VGA_HEIGHT; i++)
			vga_copy_line(i, i-1);
		state.row = VGA_HEIGHT - 1;
		vga_clear_line(VGA_HEIGHT - 1);
	}
}

void term_backspace(void) {
	if (state.col != 0) --state.col;

	size_t offset = state.row * VGA_WIDTH + state.col;
	vga_print_char(offset, state.color, (uint8_t)' ');
}

void term_normalchar(char c) {
	size_t offset = (state.row * VGA_WIDTH) + state.col;
	vga_print_char(offset, state.color, (uint8_t)c);

	if (++state.col >= VGA_WIDTH) term_newline();
}

void term_putstr(const char* str) {
	while(*str != '\0')
		term_putchar(*(str++));
}
