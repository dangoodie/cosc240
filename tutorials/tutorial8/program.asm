bits 32  ; Use 32-bit code
mov eax, 0xFEEDBEEF  ; Set eax to an easily recognisable value
jmp $  ; Cause an infinite loop by jumping to the beginning of this line (i.e. continually jumping to the start of the jump instruction)

