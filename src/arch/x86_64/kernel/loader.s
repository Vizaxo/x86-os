extern print_error

section .text
global load
load:
        ;; Check that this computer supports multiboot
        cmp eax, 0x36d76289
        je multiboot_supported
        mov al, "0"
        call print_error

multiboot_supported:
        mov esp, stack_top      ; Setup stack

        extern long_mode
        call long_mode

halt:   cli
loop:   hlt
        jmp loop


;;; The stack
section .bss
align 16
stack_bottom:
resb 16384                      ; 16 KiB stack
stack_top:
