bits 64
section .text
global kprintf
global args
extern kprintfReal
extern saveArgs

;helper
kprintf:
    mov rax, .realOne
    mov qword [saveArgs.returnAddress], rax
    jmp saveArgs
.realOne:
    call kprintfReal

    ret

; this function isn't safe for multithreading...
saveArgs:
    mov qword [args.arg1], rdi
    mov qword [args.arg2], rsi
    mov qword [args.arg3], rdx
    mov qword [args.arg4], rcx
    mov qword [args.arg5], r8
    mov qword [args.arg6], r9

    jmp qword [.returnAddress]
.returnAddress:
    dq 0

section .data
args:
.arg1:
    dq 0
.arg2:
    dq 0
.arg3:
    dq 0
.arg4:
    dq 0
.arg5:
    dq 0
.arg6:
    dq 0