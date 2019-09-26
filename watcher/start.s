
.text

.type _start,@function
.align 4
.global _start
_start:
    .cfi_startproc
    .cfi_undefined rip
    xorl %ebp, %ebp

    mov %rdx, %r9
    popq %rdi

    mov %rsp, %rsi
    and $~15, %rsp
    pushq %rax
    pushq %rsp

    call main
    // program never exits normally it will always be killed, don't bother
    // calling sys_exit
    .cfi_endproc
