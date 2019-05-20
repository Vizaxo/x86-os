# my-hobby-os

This is a simple operating system I'm building, so I can learn how operating systems work and how they are made. It's written in assembly and C, targeting x86_64.

## Code overview

### Loading and set-up (src/arch/x86_64/loader/)

The bootloader is GRUB 2, configured in `grub.cfg`. The kernel is linked together using the `kernel.ld` linker script.

Execution begins in `loader.s`, which checks that multiboot is supported and sets up the stack. `long_mode.s` sets up the [paging tables](https://wiki.osdev.org/Paging) (512 2MiB tables), [Global Descriptor Table](https://wiki.osdev.org/Global_Descriptor_Table), and enables long mode (64-bit code execution).

### Kernel (src/kernel)

The kernel begins in `kmain.c`'s `kmain` function.

The [interrupt descriptor table](https://en.wikipedia.org/wiki/Interrupt_descriptor_table) is defined and set up in `idt.c`, with the interrupt handlers in `idt.s`.

`vga.c` is a basic wrapper around the [VGA text mode](https://wiki.osdev.org/VGA_Hardware), allowing characters to be drawn on the screen. `term.c` extends this to provide simple terminal-like printing, where characters or strings can be printed in sequence.

## Building and running

### Requirements

- [GCC cross compiler](https://wiki.osdev.org/GCC_Cross-Compiler) (including binutils and [libgcc without red zone](https://wiki.osdev.org/Libgcc_without_red_zone)) targeting `x86_64-elf` are required for building.

  The makefile expects the cross-compiler to be in `$HOME/opt/cross/bin`; a different directory can be specified with the `cross_dir` variable:

  ```
  export cross_dir=/my/cross/compiler/directory/bin
  ```
- [GRUB 2](https://www.gnu.org/software/grub/) is used as the bootloader; make sure you have the `grub2-mkrescue` command
- [QEMU](https://www.qemu.org/) is needed if the OS is emulated (rather than being run on real hardware). The command `qemu-system-x86_64` is needed.
- [xorriso](https://www.gnu.org/software/xorriso/) for building the ISO image
- [nasm](https://www.nasm.us/) to assemble the assembly files

### Building

- `make kernel` will build the kernel binary
- `make iso` will build an ISO image

### Running

This project is tested by running it in a QEMU virtual machine. `make run` will run the OS in the virtual machine:

```sh
qemu-system-x86_64 -cdrom build/os-x86_64.iso
```

### Debugging

[GDB](https://www.gnu.org/software/gdb/) can be connected to QEMU for debugging. `make debug` will launch QEMU in debug mode, and the OS will wait for GDB before starting:

```sh
qemu-system-x86_64 -s -S -cdrom build/os-x86_64.iso
```

The `.gdbinit` file will set up GDB to debug the kernel binary and connect to QEMU. You may need to allow GDB to load this file by adding the following line to `$HOME/.gdbinit` (substituting in the path to this directory):

```
add-auto-load-safe-path $path-to-my-hobby-os/.gdbinit
```

Alternatively, the GDB commands can be run manually:

```
file build/kernel-x86_64.bin
target remote localhost:1234
```

## Resources

- [OSDev Wiki](https://wiki.osdev.org/): a comprehensive collection of articles on many topics relating to OS development
- [Writing an OS in Rust](https://os.phil-opp.com/) by Philipp Oppermann: despite being primarily in Rust, this book (including the [first edition](https://os.phil-opp.com/first-edition/) was very useful for getting the OS to the point where it can run 64-bit C code
- [Intel Software Developer's Manual](https://software.intel.com/en-us/articles/intel-sdm): comprehensive reference for x86_64 CPUs
