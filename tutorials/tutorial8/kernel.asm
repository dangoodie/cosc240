;; A simple kernel that is the start of our own OS!

bits 32  ; We want to write 32-bit code

section .text
    ;; The following is required to meet the multiboot spec
    align 4
    dd 0x1BADB002  ; Magic number to specify we support multiboot
    dd 0x01                  ;flags
    dd - (0x1BADB002 + 0x01) ;checksum. m+f+c should be zero
    
global start  ; Export the program entry point
global in_port  ; Export the in_port function
global out_port  ; Export the out_port function
global gdt_flush  ; Export the function to flush the GDT
global load_idt  ; Export the function to load the IDT
global keyboard_handler  ; Export the keyboard interrupt handler

extern kmain  ; Import the kmain function
extern gp  ; Import the GDT pointer
extern keyboard_handler_main  ; Import the real keyboard interrupt handler

gdt_flush:  ; Load our new GDT and reset all the segment registers
        lgdt [gp]
        mov ax, 0x10
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax
        jmp 0x08:flush ; A long jump is required to reset cs
flush:
        ret

in_port:
    mov edx, [esp + 4] ; move the parameter to edx
            ;al is the lower 8 bits of eax
    in al, dx   ;dx is the lower 16 bits of edx
    ret

out_port:
    mov   edx, [esp + 4]    
    mov   al, [esp + 4 + 4]  
    out   dx, al  
    ret
    
load_idt:
    mov edx, [esp + 4]
    lidt [edx]
    sti
    ret
    
keyboard_handler:
    call keyboard_handler_main
    iretd
    
start:
    cli                   ; Disable interrupts
    mov esp, stack_space  ; By default, we don't have any stack space - this sets some up for us
    push ebx              ; 
    call kmain
    hlt                   ; Halt the system

section .bss
resb 16384  ; Reserve 16KB for stack
stack_space:


