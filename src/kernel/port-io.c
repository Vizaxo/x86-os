#include "port-io.h"

void outb(uint16_t port, uint8_t x) {
	asm volatile ("outb %0, %1" : : "a"(x), "Nd"(port));
}

uint8_t inb(uint16_t port) {
	uint8_t ret;
	asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

void io_wait(void) {
	asm volatile (
		"jmp 1f\n\t"
		"1: jmp 2f\n\t"
		"2:");
}
