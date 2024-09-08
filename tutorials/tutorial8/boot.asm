;; A very simple bootloader that just halts the machine
org 0x7C00      ; the code will be loaded at this address, let the assembler know that
bits 16         ; this code is in real mode, so only 16 bit

start:          ; the start of code
  cli           ; disable interrupts
  cld           ; clear direction flag is all we need to change
  hlt           ; halt the system
  
  ; To be bootable, we need to be 512 bytes, with the last two bytes being 0xAA55
times 510 - ($-$$) db 0 ; fill rest of memory with 0s
dw 0xAA55       ; ensure we're seen as bootable
