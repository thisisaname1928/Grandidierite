bits 64
section .text
global getCr3
;uint64_t getCr3();
getCr3:
    mov rax, cr3
    ret

global reloadTLB
reloadTLB:
    invlpg [rdi]
    ret
