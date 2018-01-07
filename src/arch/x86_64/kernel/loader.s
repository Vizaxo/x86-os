;;; The stack
section .bss
align 16
stack_bottom:
resb 16384                      ; 16 KiB stack
stack_top:


section .text
global load
load:
        ;; Check that this computer supports multiboot
        cmp eax, 0x36d76289
        je multiboot_supported
        ;; TODO: print an error message if multiboot is not supported
        jmp halt
multiboot_supported:
        mov esp, stack_top      ; Setup stack

        ;; TODO: call kernel

halt:   cli
loop:   hlt
        jmp loop
