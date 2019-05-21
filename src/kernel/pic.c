#include "pic.h"
#include "port-io.h"

#define PIC1_CMD 0x20
#define PIC1_DATA 0x21
#define PIC2_CMD 0xA0
#define PIC2_DATA 0xA1

#define ICW4 0x01
#define INIT 0x10
#define PIC_8086 0x01
#define EOI 0x20

void pic_remap(void) {
	uint8_t mask1, mask2;
	mask1 = inb(PIC1_DATA);
	mask2 = inb(PIC2_DATA);

	outb(PIC1_CMD, INIT | ICW4);
	io_wait();
	outb(PIC2_CMD, INIT | ICW4);
	io_wait();
	outb(PIC1_DATA, 32); //PIC1 interrupts 32-39
	io_wait();
	outb(PIC2_DATA, 40); //PIC2 interrupts 40-47
	io_wait();
	outb(PIC1_DATA, 4); //Slave at IRQ2
	io_wait();
	outb(PIC2_DATA, 2); //PIC2 cascade identity
	io_wait();
	outb(PIC1_DATA, PIC_8086); //8086 mode
	io_wait();
	outb(PIC2_DATA, PIC_8086); //8086 mode
	io_wait();

	outb(PIC1_DATA, mask1 | (uint8_t)~(1 << 1)); //temoporarily disable timer
	io_wait();
	outb(PIC2_DATA, mask2);
}

/* Send end-of-interrupt signal to the PIC once the interrupt has been handled */
void pic_send_eoi(uint8_t irq) {
	if(irq >= 8)
		outb(PIC2_CMD, EOI);
	outb(PIC1_CMD, EOI);
}
