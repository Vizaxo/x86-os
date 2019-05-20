AUXFILES := Makefile
PROJDIRS := src

ASM_SRCFILES := $(shell find $(PROJDIRS) -type f -name "*.s")
C_SRCFILES := $(shell find $(PROJDIRS) -type f -name "*.c")
SRCFILES := $(ASM_SRCFILES) $(C_SRCFILES)
HDRFILES := $(shell find $(PROJDIRS) -type f -name "*.h")
OBJFILES := $(patsubst src%,build%,$(patsubst %.c,%.o,$(patsubst %.s,%.s.o,$(SRCFILES))))

ALLFILES = $(SRCFILES) $(HDRFILES) $(AUXFILES)

arch ?= x86_64
target := $(arch)-elf
kernel := build/kernel-$(arch).bin
iso := build/os-$(arch).iso
linker_script := src/arch/$(arch)/loader/kernel.ld
grub_cfg := src/arch/$(arch)/loader/grub.cfg

WARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
            -Wconversion -Wstrict-prototypes
CFLAGS := -m64 -g -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 $(WARNINGS)

cross_dir ?= $(HOME)/opt/cross/bin
CC_DIR ?= $(cross_dir)
LD_DIR ?= $(cross_dir)
AS_DIR ?= $(cross_dir)
CC := $(CC_DIR)/$(target)-gcc
LD := $(LD_DIR)/$(target)-ld
AS := nasm

.PHONY: all clean run iso kernel debug

all: $(kernel)

clean:
	-@$(RM) -r build

run: $(iso)
	@qemu-system-$(arch) -cdrom $(iso)

debug: $(iso)
	@qemu-system-$(arch) -s -S -cdrom $(iso)

iso: $(iso)

$(iso): $(kernel) $(grub_cfg)
	@mkdir -p build/isofiles/boot/grub
	@cp $(kernel) build/isofiles/boot/kernel.bin
	@cp $(grub_cfg) build/isofiles/boot/grub
	@grub2-mkrescue -o $(iso) build/isofiles 2> /dev/null
	-@$(RM) -r build/isofiles

kernel: $(kernel)

$(kernel): $(OBJFILES) $(linker_script)
	@$(LD) $(LDFLAGS) -n -T $(linker_script) -o $(kernel) $(OBJFILES)

build/%.s.o: src/%.s
	@mkdir -p $(shell dirname $@)
	@$(AS) -felf64 $< -o $@

build/%.o: src/%.c
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -c $< -o $@
