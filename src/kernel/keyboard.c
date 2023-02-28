#include <stdint.h>

#include "keyboard.h"
#include "port-io.h"
#include "term.h"

#define KEY_ALT 0
#define KEY_CAPSLOCK 0
#define KEY_ESC 0
#define KEY_LCTRL 0
#define KEY_LSHIFT 0
#define KEY_PTSCR 0
#define KEY_RSHIFT 0

char kb_us_dvorak[256] =
{
	0, KEY_ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '[', ']', '\b',
	'\t', '\'', ',', '.', 'p', 'y', 'f', 'g', 'c', 'r', 'l', '/', '=', '\n',
	KEY_LCTRL, 'a', 'o', 'e', 'u', 'i', 'd', 'h', 't', 'n', 's', '-', '\\',
	KEY_LSHIFT, '<', ';', 'q', 'j', 'k', 'x', 'b', 'm', 'w', 'v', 'z', KEY_RSHIFT, KEY_PTSCR,
	KEY_ALT, ' ', KEY_CAPSLOCK,
};

char kb_us_qwerty[256] =
{
	0, KEY_ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '[', ']', '\b',
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
	KEY_LCTRL, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '\\',
	KEY_LSHIFT, '<', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', KEY_RSHIFT, KEY_PTSCR,
	KEY_ALT, ' ', KEY_CAPSLOCK,
};

char kb_get_key(void) {
	uint8_t scancode = inb(0x60);
	char c = 0;
	if (scancode & 0x80) {
		//Key up
	} else {
		c = kb_us_qwerty[scancode];
		if (c != 0)
			term_putchar(c);
	}
	return c;
}
