

.text

# this is taken from glibc's syscall for x86
.type syscall,@function
.align 4
.global syscall
syscall:
    movq %rdi, %rax
	movq %rsi, %rdi
	movq %rdx, %rsi
	movq %rcx, %rdx
	movq %r8, %r10
	movq %r9, %r8
	movq 8(%rsp),%r9
	syscall
    ret
