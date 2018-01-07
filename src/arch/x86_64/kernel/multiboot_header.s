section .multiboot_header

MAGIC_NUMBER equ 0xE85250D6
FLAGS        equ 0
HEADER_SIZE  equ header_end - header_start

header_start:
    dd MAGIC_NUMBER
    dd FLAGS
    dd HEADER_SIZE
    dd 0x100000000 - (MAGIC_NUMBER + FLAGS + HEADER_SIZE) ;checksum

    ; end tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end:
