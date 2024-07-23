;; A simple kernel that is the start of our own OS!

bits 32  ; We want to write 32-bit code

section .text
    ;; The following is required to meet the multiboot spec
    align 4
    dd 0x1BADB002            ; Magic number to specify we support multiboot
    dd 0x00                  ; multiboot flags - these will do us
    dd - (0x1BADB002 + 0x00) ; checksum. m+f+c should be zero
    
global start  ; Define the program entry point

start:
    cli                   ; Disable interrupts
    mov esp, stack_space  ; By default, we don't have any stack space - this sets some up for us
    ;; TODO: Include C code here!
    hlt                   ; Halt the system
    
section .bss
resb 16384  ; Reserve 16KB for stack
stack_space:
