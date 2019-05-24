ENTRY(load)
KERNEL_OFFSET = 0xffffffffff000000;

SECTIONS {
    . = 1M;

    .boot :
    {
        *(.multiboot_header)
    }

    . = ALIGN(4096);
    .bootstrap :
    {
        build/arch/x86_64/loader/*.s.o (.text)
        build/arch/x86_64/loader/*.s.o (.rodata)
        build/arch/x86_64/loader/*.s.o (.bss)
    }

    . = KERNEL_OFFSET;

    .text : AT(ADDR(.text) - KERNEL_OFFSET)
    {
        *(EXCLUDE_FILE(build/arch/x86_64/loader/*.o) .text)
        . = ALIGN(4096);
    }

    .rodata : AT(ADDR(.rodata) - KERNEL_OFFSET)
    {
        *(EXCLUDE_FILE(build/arch/x86_64/loader/*.o) .rodata)
        . = ALIGN(4096);
    }

    .bss : AT(ADDR(.bss) - KERNEL_OFFSET)
    {
        *(EXCLUDE_FILE(build/arch/x86_64/loader/*.o) .bss)
        . = ALIGN(4096);
    }

}
