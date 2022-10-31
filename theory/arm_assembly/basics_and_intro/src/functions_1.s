.global _start:

_start:
    mov r0, #0x1
    mov r1, #0x2

    bl add2

    mov r4, #0x6

add2:
    add r2, r0, r1 ; in arm assembly there is no need for function arguments, since the registers stay the same even after a branching in the source code
    bx lr ; this makes it a real function
