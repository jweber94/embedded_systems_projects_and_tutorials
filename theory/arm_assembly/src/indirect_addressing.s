.global _start

_start:
    ldr r0, =list
    ldr r1, [r0]
    ldr r2, [r0,#3]

    # Caution: we need to swi 0 and the exit syscall to avoid the syscall

.data
    my_str:
        .ascii "Hello World"

    list:
        .word 4,5,-1,44,-8
