#include "idt.h"
#include "term.h"

struct __attribute__((__packed__)) idtr {
	uint16_t limit;
	uint64_t offset;
};

struct __attribute__((__packed__)) idt_descriptor {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t ist;
	uint8_t type_attr;
	uint16_t offset_mid;
	uint32_t offset_high;
	uint32_t zero;
};

struct idt_descriptor idt[256];

struct idtr idtr = {
	.limit = sizeof(idt)-1,
	.offset = (uint64_t)idt
};

struct idt_descriptor make_idt_descriptor(
	uint64_t offset, uint16_t selector, uint8_t ist, uint8_t type_attr) {
	return (struct idt_descriptor){(uint16_t)offset, selector,
			ist, type_attr, (uint16_t)(offset >> 16),
			(uint32_t)(offset >> 32), 0x00000000};
}

void setup_idt(void) {
	for (int i = 0; i < 256; i++) {
		idt[i] = make_idt_descriptor((uint64_t)&no_interrupt_handler, (1 << 3), 0, 0x8e);
	}

	idt[8] = make_idt_descriptor((uint64_t)&handler_double_fault, (1 << 3), 0, 0x8e);
	idt[13] = make_idt_descriptor((uint64_t)&general_protection_fault, (1 << 3), 0, 0x8e);
	idt[14] = make_idt_descriptor((uint64_t)&page_fault, (1 << 3), 0, 0x8e);
	idt[32] = make_idt_descriptor((uint64_t)&timer_interrupt, (1 << 3), 0, 0x8e);
	idt[33] = make_idt_descriptor((uint64_t)&keyboard_interrupt, (1 << 3), 0, 0x8e);

	load_idt();
}
