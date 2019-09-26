
.text

.type _start,@function
.align 4
.global _start
_start:
    xorl %ebp, %ebp

    mov %rdx, %r9
    popq %rdi

    mov %rsp, %rsi

    call main
    // program never exits normally it will always be killed, don't bother
    // calling sys_exit
