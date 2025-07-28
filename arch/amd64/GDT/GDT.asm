bits 64

section .text
global loadGDT
loadGDT:
    ;jmp $
    lgdt [GDTR]
    mov rax, 0x8
    push rax
    lea rax, [rel .reload]
    push rax
    retfq
.reload:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret

section .rodata
GDTR:
    dw GDT64.end - GDT64
    dq GDT64
GDT64:
    .NULL:
        dw 0
        dw 0
        db 0
        db 0
        db 0
        db 0
    .CODE:
        dw 0xffff
        dw 0
        db 0
        db 0b10011010
        db 0b10101111
        db 0
    .DATA:
        dw 0xffff
        dw 0
        db 0
        db 0b10010010
        db 0b11001111
        db 0
    .end:
