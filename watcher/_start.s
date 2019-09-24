
.text

.global _start
_start:
    call _main
    movl %edi, %eax
    call _exit
