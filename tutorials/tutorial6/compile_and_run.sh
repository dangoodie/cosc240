#!/bin/bash
nasm -f elf32 -g kernel.asm -o kasm.o
gcc -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -g -c kernel.c -o kc.o
ld -m elf_i386 -T link.ld -o kernel kasm.o kc.o
if [ $# -gt 0 ]
then
  qemu-system-i386 -machine q35 -kernel kernel -gdb tcp::$1 -S
else
  qemu-system-i386 -machine q35 -kernel kernel
fi

