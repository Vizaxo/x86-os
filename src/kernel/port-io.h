#ifndef __PORT_IO_H__
#define __PORT_IO_H__

#include <stdint.h>

void outb(uint16_t port, uint8_t x);
uint8_t inb(uint16_t port);
void io_wait(void);

#endif //__PORT_IO_H__
