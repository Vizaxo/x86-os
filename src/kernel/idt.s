section .text
bits 64

extern term_putstr
extern idtr

global no_interrupt_handler
no_interrupt_handler:
        mov rdi, err_str
        call term_putstr
        iretq

global load_idt
load_idt:
        lidt [idtr]
        ret

section .rodata
err_str:
        db "ERROR: no interrupt handler specified",10,0
