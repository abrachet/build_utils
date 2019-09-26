
.text

.global _start
_start:
    xorl %ebp, %ebp

    mov %rdx, %r9
    popq %rsi

    mov %rsp, %rdx
    and $~15, %rsp
    pushq %rax
    pushq %rsp

    call main
    ; program never exits normally it will always be killed, don't bother
    ; calling sys_exit
