section .bss
align 16
global kernel_stack_bottom
kernel_stack_bottom:
        resb 16384
kernel_stack_top:
