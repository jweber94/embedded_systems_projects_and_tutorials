.global _start

_start:
    mov r0, #0x1    ; ... 0001 in binary
    mov r1, #0x2    ; ... 0010 in binary
    and r3, r0, r1  ; should be ... 0000 since there is no match in the binary format

    # clearing bits on registers
    mov r4, #0xAA
    mvn r4, r4 ; negate the bits
    and r0, r0, #0x000000ff ; clear all bits until the 8 left most bits - the hex value acts as a binary mask
    