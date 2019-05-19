#ifndef __IDT_H__
#define __IDT_H__

#include <stdint.h>
#include <stddef.h>

void setup_idt(void);
void no_interrupt_handler(void);
struct idt_descriptor make_idt_descriptor(
	uint64_t offset, uint16_t selector, uint8_t ist, uint8_t type_attr);
void load_idt(void);

#endif //__IDT_H__
