# Boots up gdb to debug the kernel
# QEMU must be run in debugging mode; use make debug for this
file build/kernel-x86_64.bin
target remote localhost:1234
