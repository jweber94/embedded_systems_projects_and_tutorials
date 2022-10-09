.global _start

_start:
    # mov r0, #0xffffffff
    mov r0, #0x7
    mov r1, #0x1

    mul r2, r1, r0
    add r3, r1, r0
    sub r4, r1, r0

    # example of a cpsr indicated negative number if you comment line 4 in and line 5 out
    # subs r5, r0, r1
