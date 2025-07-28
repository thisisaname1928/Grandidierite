bits 64
global loadIDT
section .text
;rdi
loadIDT:
    lidt [rdi]
    ret