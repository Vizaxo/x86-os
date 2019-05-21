section .text
bits 64

extern term_putstr
extern kb_get_key
extern idtr
extern pic_send_eoi

global no_interrupt_handler
no_interrupt_handler:
        mov rdi, err_str
        call term_putstr
        iretq

global load_idt
load_idt:
        lidt [idtr]
        ret

global general_protection_fault
general_protection_fault:
        mov rdi, general_protection_fault_str
        call term_putstr
        add esp, 8
        hlt
        iretq

global handler_double_fault
handler_double_fault:
        mov rdi, double_fault_str
        call term_putstr
        add esp, 8
        iretq

global timer_interrupt
timer_interrupt:
        mov rdi, timer_str
        call term_putstr
        mov rdi, 0
        call pic_send_eoi
        iretq

global keyboard_interrupt
keyboard_interrupt:
        call kb_get_key
        mov rdi, 1
        call pic_send_eoi
        iretq

global page_fault
page_fault:
        mov rdi, page_fault_str
        call term_putstr
        add esp, 8
        iretq

section .rodata
err_str:
        db "ERROR: no interrupt handler specified",10,0
general_protection_fault_str:
        db "EXCEPTION: general protection fault",10,0
double_fault_str:
        db "EXCEPTION: duoble fault",10,0
timer_str:
        db "IRQ0: Timer interrupt received",10,0
page_fault_str:
        db "EXCEPTION: page fault",10,0
