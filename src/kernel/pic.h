#ifndef __PIC_H__
#define __PIC_H__

#include <stdint.h>

void pic_remap(void);
void pic_send_eoi(uint8_t irq);

#endif //__PIC_H__
