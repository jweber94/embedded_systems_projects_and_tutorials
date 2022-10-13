.global _start

_start:
    mov r0, #1
    mov r1, #2
    push {r0, r1}
    bl get_value
    mov r5, #42
    pop {r0, r1}
    b end

get_value:
    mov r0, #4
    mov r1, #3
    add r4, r0, r1
    bx lr

end:
    mov r7, #1