extern print_error

section .text
global long_mode
bits 32
long_mode:
        call check_cpuid
        call check_long_mode
        call identity_map_lower
        call enable_paging

        lgdt [gdt64.pointer]
        jmp gdt64.code:long_mode_code


bits 64
long_mode_code:
        call map_higher_kernel_pages

        mov ax, 0
        mov ss, ax
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax

        ;; Use the high-address stack
        extern kernel_stack_bottom
        mov rsp, kernel_stack_bottom

        ; Long jump to kmain
        extern kmain
        call kmain

        hlt
        jmp 1b


;;; Map the kernel into a high address
map_higher_kernel_pages:
        ;; Kernel offset = ff ff ff ff ff 00 00 00
        mov eax, p3_table_high
        or eax, 0x11
        mov [p4_table + 511*8], eax          ; bits 111111111

        mov eax, p2_table_high
        or eax, 0b11
        mov [p3_table_high + 511*8], eax     ; bits 111111111

        ;; map first P2 entry to P1 table
        mov eax, p1_table_high
        or eax, 0b11
        mov [p2_table_high + 504*8], eax     ; bits 111111000

        mov ecx, 0

.map_p1_table:
        ;;  map ecx-th P2 entry to huge page starting at 4KiB*ecx
        mov eax, 0x1000         ; 4KiB
        mul ecx                 ; start address of ecx-th page = ecx*4KiB
        or eax, 0b11            ; present + writable
        mov [p1_table_high + ecx * 8], eax ; map ecx-th entry

        inc ecx
        cmp ecx, 512            ; map 512 pages
        jne .map_p1_table       ; map next entry


        ;; reload cr3 to force TLB flush
        mov rcx, cr3
        mov cr3, rcx

        ret



bits 32
check_cpuid:
        pushfd
        pop eax
        mov ecx, eax
        xor eax, 1 << 21
        push eax
        popfd
        pushfd
        pop eax
        push ecx
        popfd
        xor eax, ecx
        jz .no_cpuid
        ret
.no_cpuid:
        mov al, "1"
        call print_error


check_long_mode:
        mov eax, 0x80000000
        cpuid
        cmp eax, 0x80000001
        jb .no_long

        mov eax, 0x80000001
        cpuid
        test edx, 1 << 29
        jz .no_long
        ret
.no_long:
        mov al, "2"
        call print_error



identity_map_lower:

        ;;  map first P4 entry to P3 table
        mov eax, p3_table
        or eax, 0b11
        mov [p4_table], eax

        ;; map first P3 entry to P2 table
        mov eax, p2_table
        or eax, 0b11
        mov [p3_table], eax

        ;; map first P2 entry to P1 table
        mov eax, p1_table
        or eax, 0b11
        mov [p2_table], eax

        ;; Map each P2 table into a 2MiB (huge) page
        mov ecx, 0

.map_p1_table:
        ;;  map ecx-th P2 entry to 4KiB pages starting at 4KiB*ecx
        mov eax, 0x1000         ; 4KiB
        mul ecx                 ; start address of ecx-th page = ecx*4KiB
        or eax, 0b11            ; present + writable
        mov [p1_table + ecx * 8], eax ; map ecx-th entry

        inc ecx
        cmp ecx, 512            ; map 512 pages
        jne .map_p1_table

        ret



enable_paging:
        ;;  load P4 to cr3 register
        mov eax, p4_table
        mov cr3, eax

        ;; enable PAE-flag in cr4
        mov eax, cr4
        or eax, 1<<5
        mov cr4, eax

        ;; set long-mode bit in EFER MSR
        mov ecx, 0xC0000080
        rdmsr
        or eax, 1<<8
        wrmsr

        ;; enable paging in cr0 register
        mov eax, cr0
        or eax, 1<<31
        mov cr0, eax

        ret



;;; Page tables
section .bss
align 4096
global p4_table
p4_table:
        resb 4096
p3_table:
        resb 4096
p2_table:
        resb 4096
p1_table:
        resb 4096
p3_table_high:
        resb 4096
p2_table_high:
        resb 4096
p1_table_high:
        resb 4096


;;; GDT
section .rodata
gdt64:
        dq 0                    ; zero entry
.code: equ $ - gdt64
        dq (1<<43) | (1<<44) | (1<<47) | (1<<53)
.data: equ $ - gdt64
        dq (0<<43) | (1<<44) | (1<<47) | (1<<53)
.pointer:
        dw $ - gdt64 - 1
        dq gdt64
