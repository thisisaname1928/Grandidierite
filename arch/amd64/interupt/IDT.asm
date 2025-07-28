bits 64
global loadIDT
section .text
;rdi
loadIDT:
    lgdt [rdi]

.reload:
    ret