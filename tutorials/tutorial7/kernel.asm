	;; A simple kernel that is the start of our own OS!

	bits 32; We want to write 32-bit code

	section .text
	;;      The following is required to meet the multiboot spec
	align   4
	dd      0x1BADB002; Magic number to specify we support multiboot
	dd      0x00; multiboot flags - these will do us
	dd      - (0x1BADB002 + 0x00); checksum. m+f+c should be zero

	global start; Define the program entry point
	global in_port; Export the in_port function
	global out_port; Export the out_port function
    global load_idt; Export the function to load the IDT
    global keyboard_handler ; Export the keyboard interrupt handler

	extern kmain; The kernel entry function
    extern keyboard_handler_main; The keyboard handler function
start:
	cli  ; Disable interrupts
	mov  esp, stack_space; By default, we don't have any stack space - this sets some up for us
	call kmain; Call our main kernel function
	hlt  ; Halt the system

in_port:
	mov  edx, [esp + 4]; move the parameter to edx
	in   al, dx; al is the lower 8 bits of eax, dx is the lower 16 bits of edx
	ret; return

out_port:
	mov  edx, [esp + 4]; move the parameter to edx
	mov  eax, [esp + 8]; move the second parameter to eax
	out  dx, al; al is the lower 8 bits of eax, dx is the lower 16 bits of edx
	ret; return

load_idt:
    mov edx, [esp + 4]; move the parameter to edx
    lidt [edx]; load the IDT
    sti; enable interrupts
    ret; return

keyboard_handler:
    call keyboard_handler_main; call the keyboard handler
    iretd; return from interrupt

	section .bss
	resb    16384; Reserve 16KB for stack

stack_space:
