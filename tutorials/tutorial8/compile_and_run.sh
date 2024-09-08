#!/bin/bash
nasm -f elf32 -g kernel.asm -o kasm.o
gcc -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -g -c kernel.c -o kc.o
ld -m elf_i386 -T link.ld -o kernel kasm.o kc.o
cp kernel iso/boot
mkisofs -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -A cosc240 -input-charset utf8 -quiet -boot-info-table -o cosc240.iso iso
if [ $# -gt 0 ]
then
  qemu-system-i386 -machine q35 -cdrom cosc240.iso -gdb tcp::$1 -S
else
  qemu-system-i386 -machine q35 -cdrom cosc240.iso
fi

