#ifndef __TERM_H__
#define __TERM_H__

#include <stdint.h>

void term_putchar(char c);
void term_newline(void);
void term_putstr(const char* str);

extern uint8_t kernel_quit;

#endif //__TERM_H__
